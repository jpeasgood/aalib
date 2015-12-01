#ifndef AA_EGL_H
#define AA_EGL_H

#include "../headers/window.h"
#include "../headers/texture.h"
#include "../headers/program.h"
#include "../headers/buffer.h"
#include "../headers/surface.h"
#include "../headers/shader.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <sstream>
#include <string>
#include <vector>

#include <iostream>
void check_gl_error();

namespace aa
{
	class vertex_shader;
	class fragment_shader;

	class egl : public singleton<egl>
	{
		friend class singleton<egl>;

		private:
			EGLDisplay egl_display;
			EGLConfig ecfg;
			EGLContext egl_context;

			egl();
			egl(const egl &other) = delete;
			egl(egl &&other) = delete;
			~egl();

			egl &operator=(const egl &other) = delete;
			egl &operator=(egl &&other) = delete;

		public:
			void set_viewport(int x, int y, unsigned int width, unsigned int height);
			void clear();
			void draw_elements(unsigned int count);
			void swap_buffers(surface &surf);
			void make_surface_current(surface &surf);
			void flush();
			void activate_texture(unsigned int i);

			surface create_window_surface(const window &wnd);
			texture create_texture(unsigned int width, unsigned int height);
			void bind_texture(const texture &tex);
			vertex_shader create_vertex_shader(const std::string &source);
			fragment_shader create_fragment_shader(const std::string &source);
			program create_program(const vertex_shader &vertex_shader, const fragment_shader &fragment_shader);
			buffer create_buffer(buffer::buffer_type type);
	};
}

#endif
