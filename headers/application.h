#ifndef AA_APPLICATION_H
#define AA_APPLICATION_H

#include "../headers/signal.h"
#include "../headers/file_descriptor_interface.h"
#include "../headers/singleton.h"

#include <sys/select.h>
#include <X11/Xlib.h>
#include <map>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <memory>

namespace aa
{
	class application : public singleton<application>
	{
		friend class singleton<application>;

		private:
			Display *display;

			bool running;
			std::map<int, signal<void()>> file_descriptors;

			void main_loop();

			application();
			application(const application &other) = delete;
			application(application &&other) = delete;
			~application();

			application &operator=(const application &other) = delete;
			application &operator=(application &&other) = delete;

		public:
			void run();
			void stop();
			void set_file_descriptor(file_descriptor_interface &fd_class);
			Display *get_native_display() const;
	};
}

#endif
