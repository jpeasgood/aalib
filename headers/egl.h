#ifndef EGL_H
#define EGL_H

#include "../headers/window.h"
#include "../headers/texture.h"
#include "../headers/shader.h"
#include "../headers/program.h"
#include "../headers/buffer.h"
#include "../headers/surface.h"

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
			EGLConfig ecfg;
			EGLContext egl_context;

		public:
			egl();
			egl(const egl &other) = delete;
			~egl();

			egl &operator=(const egl &other) = delete;

			void set_viewport(int x, int y, unsigned int width, unsigned int height);
			void clear();
			void draw_elements(unsigned int count);
			void swap_buffers(surface &surf);
			void make_surface_current(surface &surf);
			surface create_window_surface(const window &wnd);
			texture create_texture(unsigned int width, unsigned int height);
			void bind_texture(const texture &tex);
			shader create_shader(shader::shader_type type, const std::string &source);
			program create_program(const shader &vertex_shader, const shader &fragment_shader);
			buffer create_buffer(buffer::buffer_type type);
	};
}

#endif
