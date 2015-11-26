#include "../headers/surface.h"

aa::surface::surface(EGLDisplay egl_display, EGLSurface egl_surface)
	: egl_display(egl_display),
	  egl_surface(egl_surface)
{
}

aa::surface::~surface()
{
	eglDestroySurface(egl_display, egl_surface);
}
