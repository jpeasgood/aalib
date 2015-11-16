#include "../headers/window.h"

#include <iostream>
#include <cstring>

void aa::window::fd_selected()
{
	XEvent event;
	while (XPending(display))
	{
		XNextEvent(display, &event);
		if (event.type == Expose)
		{
			XFillRectangle(display, wnd, DefaultGC(display, DefaultScreen(display)), 20, 20, 10, 10);
			XDrawString(display, wnd, DefaultGC(display, DefaultScreen(display)), 10, 50, "Hello", strlen("Hello"));
		}
	}
}

aa::window::window(const std::string title, int x, int y, unsigned int width, unsigned int height)
{
	Window root;
	XSetWindowAttributes swa;
	display = XOpenDisplay(0);
	if (!display)
	{
		throw std::runtime_error("Cannot connect to X server.");
	}
	root = DefaultRootWindow(display) ;
	swa.event_mask = ExposureMask;
	wnd = XCreateWindow(display, root, x, y, width, height, 0,
			CopyFromParent, InputOutput, CopyFromParent, CWEventMask, &swa);
	XMapWindow(display, wnd);
	XStoreName(display, wnd, title.c_str());
	XFlush(display);
}

aa::window::~window()
{
	XDestroyWindow(display, wnd);
	XCloseDisplay(display);
}

int aa::window::get_fd() const
{
	return ConnectionNumber(display);
}

Display *aa::window::get_native_display() const
{
	return display;
}

Window aa::window::get_native_window_handle() const
{
	return wnd;
}
