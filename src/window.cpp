#include "../headers/window.h"

void aa::window::fd_selected()
{
	XEvent event;
	while(XCheckIfEvent(display, &event, [] (Display *display, XEvent *event, XPointer arg) -> Bool
	{
		switch(event->type)
		{
			case Expose:
				return event->xexpose.window == *reinterpret_cast<Window *>(arg);
				break;
			case ClientMessage:
				return event->xclient.window == *reinterpret_cast<Window *>(arg);
				break;
			case DestroyNotify:
				return event->xdestroywindow.window == *reinterpret_cast<Window *>(arg);
				break;
			default:
				return false;
		}
	}, reinterpret_cast<XPointer>(&wnd)))
	{
		switch(event.type)
		{
			case Expose:
				expose_signal(event.xexpose.x, event.xexpose.y, event.xexpose.width, event.xexpose.height);
				break;
			case DestroyNotify:
				delete_window_signal();
				disconnect();
				break;
			case ClientMessage:
				if(event.xclient.data.l[0] == wm_delete_message)
				{
					XDestroyWindow(display, wnd);
				}
				break;
		}
	}
}

aa::window::window(const std::string &title, int x, int y, unsigned int width, unsigned int height)
	: display(aa::application::get_instance()->get_native_display())
{
	Window root;
	XSetWindowAttributes swa;
	root = DefaultRootWindow(display) ;
	swa.event_mask = ExposureMask | StructureNotifyMask;
	wnd = XCreateWindow(display, root, x, y, width, height, 0,
			CopyFromParent, InputOutput, CopyFromParent, CWEventMask, &swa);
	wm_delete_message = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, wnd, &wm_delete_message, 1);
	XMapWindow(display, wnd);
	XStoreName(display, wnd, title.c_str());
	XFlush(display);
}

aa::window::~window()
{
	XDestroyWindow(display, wnd);
}

int aa::window::get_fd() const
{
	return ConnectionNumber(display);
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

aa::connection<void()> aa::window::connect_delete_window(const std::function<void()> &func)
{
	return delete_window_signal.connect(func);
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

Display *aa::window::get_native_display() const
{
	return display;
}
