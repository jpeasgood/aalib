#include "../headers/application.h"

void aa::application::main_loop()
{
	fd_set fds;
	while(running)
	{
		FD_ZERO(&fds);
		auto it = file_descriptors.begin();
		int max_fd = 0;
		int r;
		while(it != file_descriptors.end())
		{
			if(it->second.empty())
			{
				it = file_descriptors.erase(it);
			}
			else
			{
				FD_SET(it->first, &fds);
				max_fd = std::max(max_fd, it->first);
				it++;
			}
		}
		r = select(max_fd + 1, &fds, 0, 0, 0);
		if (-1 == r)
		{
			if (EINTR == errno)
			{
				continue;
			}
			else
			{
				throw std::runtime_error("Select error.");
			}
		}
		for(it = file_descriptors.begin(); it != file_descriptors.end(); it++)
		{
			if(FD_ISSET(it->first, &fds))
			{
				it->second();
			}
		}
	}
}

aa::application::application()
{
	running = false;
	display = XOpenDisplay(0);
	if (!display)
	{
		throw std::runtime_error("Cannot connect to X server.");
	}
}

aa::application::~application()
{
	XCloseDisplay(display);
}

void aa::application::run()
{
	running = true;
	main_loop();
}

void aa::application::stop()
{
	running = false;
}

void aa::application::set_file_descriptor(aa::file_descriptor_interface &fd_class)
{
	if(fd_class.fd_connection)
	{
		fd_class.fd_connection->disconnect();
	}
	auto it = file_descriptors.find(fd_class.get_fd());
	if(it == file_descriptors.end())
	{
		it = file_descriptors.emplace(fd_class.get_fd(), signal<void()>()).first;
	}
	fd_class.fd_connection = std::make_unique<aa::connection<void()>>(
			std::move(it->second.connect(std::bind(std::mem_fn(&file_descriptor_interface::fd_selected), &fd_class))));
}

Display *aa::application::get_native_display() const
{
	return display;
}
