#include "../headers/window.h"

#include <iostream>
#include <cstring>

void aa::window::fd_selected()
{
	XEvent event;
	while(XCheckWindowEvent(display, wnd, ExposureMask, &event))
	{
		switch(event.type)
		{
			case Expose:
				expose_signal(event.xexpose.x, event.xexpose.y, event.xexpose.width, event.xexpose.height);
				break;
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

aa::connection<void(int x, int y, int width, int height)> aa::window::connect_expose(
					const std::function<void(int x, int y, int width, int height)> &func)
{
	return expose_signal.connect(func);
}

void aa::window::expose()
{
	XWindowAttributes gwa;
	XGetWindowAttributes(display, wnd, &gwa);
	XEvent event = { };
	event.type = Expose;
	event.xexpose.window = wnd;
	event.xexpose.x = gwa.x;
	event.xexpose.y = gwa.y;
	event.xexpose.width = gwa.width;
	event.xexpose.height = gwa.height;
	XSendEvent(display, wnd, 0, ExposureMask, &event);
	XFlush(display);
}
