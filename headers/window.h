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
			Atom wm_delete_message;

			signal<void(int x, int y, int width, int height)> expose_signal;
			signal<void()> delete_window_signal;

			void fd_selected() final;

		public:
			window(const window &other) = delete;
			window(window &&other) = default;
			window(const std::string &title, int x, int y, unsigned int width, unsigned int height);
			virtual ~window();

			window &operator=(const window &other) = delete;
			window &operator=(window &&other) = default;

			int get_fd() const final;

			connection<void(int x, int y, int width, int height)> connect_expose(
					const std::function<void(int x, int y, int width, int height)> &func);
			connection<void()> connect_delete_window(const std::function<void()> &func);
			void expose();
			Window get_native_window_handle() const;
			Display *get_native_display() const;
	};
}

#endif
