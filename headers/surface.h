#ifndef AA_SURFACE_H
#define AA_SURFACE_H

#include <GLES2/gl2.h>
#include <EGL/egl.h>

namespace aa
{
	class surface
	{
		friend class egl;

		private:
			EGLDisplay egl_display;
			EGLSurface egl_surface;

			surface(EGLDisplay egl_display, EGLSurface egl_surface);

		public:
			surface(const surface &surface) = delete;
			surface(surface &&surface) = default;
			~surface();

			surface &operator=(const surface &other) = delete;
			surface &operator=(surface &&other) = default;
	};
}

#endif
