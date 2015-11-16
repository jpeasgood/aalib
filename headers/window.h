#ifndef AA_WINDOW_H
#define AA_WINDOW_H

#include "../headers/application.h"
#include "../headers/file_descriptor_interface.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string>
#include <stdexcept>

namespace aa
{
	class window : public file_descriptor_interface
	{
		private:
			Display *display;
			Window wnd;

			void fd_selected() final;

		public:
			window(const window &other) = delete;
			window(const std::string title, int x, int y, unsigned int width, unsigned int height);
			~window();

			window &operator=(const window &other) = delete;

			int get_fd() const final;
			Display *get_native_display() const;
			Window get_native_window_handle() const;
	};
}

#endif
