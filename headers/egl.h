#ifndef EGL_H
#define EGL_H

#include "../headers/window.h"
#include "../headers/texture.h"
#include "../headers/shader.h"
#include "../headers/program.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <sstream>
#include <string>

namespace aa
{
	class egl
	{
		private:
			EGLDisplay egl_display;
			EGLSurface egl_surface;
			EGLContext  egl_context;

		public:
			egl(const window &wnd);
			egl(const egl &other) = delete;
			~egl();

			egl &operator=(const egl &other) = delete;

			texture create_texture(unsigned int width, unsigned int height);
			void bind_texture(const texture &tex);
			shader create_shader(shader::shader_type type, const std::string &source);
			program create_program(const shader &vertex_shader, const shader &fragment_shader);
			void use_program(const program &prog);
	};
}

#endif
