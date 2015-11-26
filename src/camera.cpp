#include "../headers/camera.h"

int aa::camera::xioctl(int device, int request, void *arg)
{
	int r;
	do
	{
		r = ioctl(device, request, arg);
	} while (-1 == r && EINTR == errno);
	return r;
}

void aa::camera::fd_selected()
{
	v4l2_buffer buf = {};

	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(device_fd, VIDIOC_DQBUF, &buf))
	{
		switch (errno)
		{
			case EAGAIN:
				return;
			case EIO:
				/* Could ignore EIO, see spec. */
				/* fall through */
			default:
				std::stringstream ss;
				ss << "Can not exchange buffers with " << device_name << ".";
				throw std::runtime_error(ss.str());
		}
	}

	this->process_frame_signal(buffers[buf.index].start, buf.bytesused);

	if (-1 == xioctl(device_fd, VIDIOC_QBUF, &buf))
	{
		std::stringstream ss;
		ss << "Can not exchange buffers with " << device_name << ".";
		throw std::runtime_error(ss.str());
	}
}

std::list<std::string> aa::camera::get_devices()
{
	std::list<std::string> ret;
	std::regex reg("video[0-9]+");
	typedef std::unique_ptr<DIR, std::function<decltype(closedir)>> dir_ptr;
	if(dir_ptr dir = dir_ptr(opendir("/dev/"), closedir))
	{
		dirent e;
		for(auto* p = &e; 0 == readdir_r(dir.get(), &e, &p) && p; )
		{
			std::string file_name(e.d_name);
			if(std::regex_match(file_name, reg))
			{
				std::stringstream ss;
				ss << "/dev/" << e.d_name;
				int fd = open(ss.str().c_str(), O_RDONLY);
				if(-1 != fd)
				{
					v4l2_capability cap;
					if ((xioctl(fd, VIDIOC_QUERYCAP, &cap) == 0) &&
							(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) &&
							(cap.capabilities & V4L2_CAP_STREAMING))
					{
						ret.push_back(ss.str());
					}
					close(fd);
				}
			}
		}
	}
	return ret;
}

std::list<aa::camera::camera_format> aa::camera::get_supported_formats(const std::string &device_name)
{
	std::list<camera_format> ret;
	int fd = open(device_name.c_str(),  O_RDONLY);
	if(-1 != fd)
	{
		v4l2_fmtdesc pixel_format = {};
		pixel_format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		while (xioctl(fd, VIDIOC_ENUM_FMT, &pixel_format) == 0)
		{
			aa::camera::camera_format supported_format;
			supported_format.pixel_format = pixel_format.pixelformat;
			v4l2_frmsizeenum frame_size = {};
			frame_size.pixel_format = pixel_format.pixelformat;
		    while (xioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frame_size) == 0)
		    {
				if (frame_size.type == V4L2_FRMSIZE_TYPE_DISCRETE)
				{
					supported_format.width = frame_size.discrete.width;
					supported_format.height = frame_size.discrete.height;
				}
				else
				{
					continue;
				}
				v4l2_frmivalenum frame_interval = {};
				frame_interval.pixel_format = pixel_format.pixelformat;
				frame_interval.width = frame_size.discrete.width;
				frame_interval.height = frame_size.discrete.height;
				while (xioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &frame_interval) == 0)
				{
					if (frame_interval.type == V4L2_FRMIVAL_TYPE_DISCRETE)
					{
						if (frame_interval.discrete.numerator != 0)
						{
							supported_format.frame_rate =
									static_cast<float>(frame_interval.discrete.denominator) /
									static_cast<float>(frame_interval.discrete.numerator);
						}
						else
						{
								supported_format.frame_rate = 0;
						}
					}
					else
					{
						continue;
					}
					ret.push_back(supported_format);
					++frame_interval.index;
				}
				++frame_size.index;
		    }
			++pixel_format.index;
		}
		close(fd);
	}
	return ret;
}

aa::camera::camera(const std::string &device_name, const aa::camera::camera_format &format) : device_name(device_name), format(format)
{
	device_fd = open(device_name.c_str(), O_RDWR | O_NONBLOCK, 0);
	if (-1 == device_fd)
	{
		std::stringstream ss;
		ss << "Failed to open " << device_name << ".";
		throw std::runtime_error(ss.str());
	}
	init_device();
}

aa::camera::~camera()
{
	uninit_device();
	if(-1 == close(device_fd))
	{
		std::stringstream ss;
		ss << "Failed to close " << device_name << ".";
		throw std::runtime_error(ss.str());
	}
}

void aa::camera::init_device()
{
	v4l2_capability cap;
	v4l2_format fmt;

	if (-1 == xioctl(device_fd, VIDIOC_QUERYCAP, &cap))
	{
		if (EINVAL == errno)
		{
			std::stringstream ss;
			ss << device_name << " is not a V4L2 device.";
			throw std::runtime_error(ss.str());
		}
		else
		{
			std::stringstream ss;
			ss << "Can not get the device capabilities of " << device_name << ".";
			throw std::runtime_error(ss.str());
		}
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
	{
		std::stringstream ss;
		ss << device_name << " is not a capture device.";
		throw std::runtime_error(ss.str());
	}

	if (!(cap.capabilities & V4L2_CAP_STREAMING))
	{
		std::stringstream ss;
		ss << device_name << " does not support streaming.";
		throw std::runtime_error(ss.str());
	}

	fmt = {};

	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = format.width;
	fmt.fmt.pix.height = format.height;
	fmt.fmt.pix.pixelformat = format.pixel_format;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

	if (-1 == xioctl(device_fd, VIDIOC_S_FMT, &fmt))
	{
		std::stringstream ss;
		ss << device_name << " does not support the video format.";
		throw std::runtime_error(ss.str());
	}

	init_mmap();
}

void aa::camera::init_mmap()
{
	v4l2_requestbuffers req = {};
	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(device_fd, VIDIOC_REQBUFS, &req))
	{
		if (EINVAL == errno)
		{
			std::stringstream ss;
			ss << device_name << " does not support memory mapping.";
			throw std::runtime_error(ss.str());
		}
		else
		{
			std::stringstream ss;
			ss << "Can not request buffers from " << device_name << ".";
			throw std::runtime_error(ss.str());
		}
	}

	if (req.count < 2)
	{
		std::stringstream ss;
		ss << device_name << " has insufficient buffer memory.";
		throw std::runtime_error(ss.str());
	}

	buffers = reinterpret_cast<buffer *>(calloc(req.count, sizeof(*buffers)));

	if (!buffers)
	{
		throw std::bad_alloc();
	}

	for (num_buffers = 0; num_buffers < req.count; ++num_buffers)
	{
		struct v4l2_buffer buf = {};
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = num_buffers;

		if (-1 == xioctl(device_fd, VIDIOC_QUERYBUF, &buf))
		{
			std::stringstream ss;
			ss << "Can not exchange buffers with " << device_name << ".";
			throw std::runtime_error(ss.str());
		}

		buffers[num_buffers].length = buf.length;
		buffers[num_buffers].start = mmap(0, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, device_fd, buf.m.offset);

		if (MAP_FAILED == buffers[num_buffers].start)
		{
			std::stringstream ss;
			ss << "mmap failed for " << device_name << ".";
			throw std::runtime_error(ss.str());
		}
	}
}

void aa::camera::uninit_device()
{
	for (unsigned int i = 0; i < num_buffers; ++i)
	{
		if (-1 == munmap(buffers[i].start, buffers[i].length))
		{
			std::stringstream ss;
			ss << "Can not unmap buffers for " << device_name << ".";
			throw std::runtime_error(ss.str());
		}
	}
	free(buffers);
}

void aa::camera::start_capturing()
{
	v4l2_buf_type type;

	for (unsigned int i = 0; i < num_buffers; ++i)
	{
		struct v4l2_buffer buf = {};

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (-1 == xioctl(device_fd, VIDIOC_QBUF, &buf))
		{
			std::stringstream ss;
			ss << "Can not exchange buffers with " << device_name << ".";
			throw std::runtime_error(ss.str());
		}
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == xioctl(device_fd, VIDIOC_STREAMON, &type))
	{
		std::stringstream ss;
		ss << "Failed to start streaming on " << device_name << ".";
		throw std::runtime_error(ss.str());
	}
}

void aa::camera::stop_capturing()
{
	v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == xioctl(device_fd, VIDIOC_STREAMOFF, &type))
	{
		std::stringstream ss;
		ss << "Failed to stop streaming on " << device_name << ".";
		throw std::runtime_error(ss.str());
	}
}

int aa::camera::get_fd() const
{
	return device_fd;
}

aa::connection<void(const void *, size_t)> aa::camera::connect_process_frame(const std::function<void(const void *, size_t)> &func)
{
	return process_frame_signal.connect(func);
}
