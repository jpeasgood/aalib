#ifndef AA_APPLICATION_H
#define AA_APPLICATION_H

#include "../headers/signal.h"
#include "../headers/file_descriptor_interface.h"

#include <sys/select.h>
#include <map>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <memory>

namespace aa
{
	class application
	{
		private:
			bool running;
			std::map<int, signal<void()>> file_descriptors;

			void main_loop();

		public:
			application();
			application(const application &other) = delete;

			application &operator=(const application &other) = delete;

			void run();
			void stop();
			void set_file_descriptor(file_descriptor_interface &fd_class);
	};
}

#endif
