#ifndef AA_WEB_CAM_H
#define AA_WEB_CAM_H

#include "../headers/file_descriptor_interface.h"

#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/mman.h>
#include <string>
#include <sstream>
#include <list>
#include <memory>
#include <regex>
#include <functional>

namespace aa
{
	class web_cam : public file_descriptor_interface
	{
		public:
			struct web_cam_format
			{
				unsigned int pixel_format;
				unsigned int width;
				unsigned int height;
				float frame_rate;
			};

		private:
			struct buffer
			{
				void *start;
				size_t length;
			};

			static int xioctl(int device, int request, void *arg);

			int device_fd;
			std::string device_name;
			web_cam_format format;
			buffer *buffers;
			unsigned int num_buffers;
			signal<void(const void *, size_t)> process_frame_signal;

			void fd_selected() final;
			void init_device();
			void init_mmap();
			void uninit_device();

		public:
			static std::list<std::string> get_devices();
			static std::list<web_cam_format> get_supported_formats(const std::string &device_name);

			web_cam(const web_cam &other) = delete;
			web_cam(const std::string &device_name, const web_cam_format &format);
			~web_cam();

			web_cam &operator=(const web_cam &other) = delete;

			void start_capturing();
			void stop_capturing();
			int get_fd() const final;
			connection<void(const void *, size_t)> connect_process_frame(const std::function<void(const void *, size_t)> &func);
	};
}

#endif
