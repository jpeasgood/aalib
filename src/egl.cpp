#include "../headers/egl.h"

aa::egl::egl()
{
	EGLint attr[] = { EGL_BUFFER_SIZE, 24, EGL_NONE};
	EGLint ctxattr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	if(!eglBindAPI(EGL_OPENGL_ES_API))
	{
		throw std::runtime_error("EGL failed to bind OPENGL ES.");
	}
	egl_display = eglGetDisplay(static_cast<EGLNativeDisplayType>(XOpenDisplay(0)));
	if (egl_display == EGL_NO_DISPLAY)
	{
		throw std::runtime_error("Error getting EGL display.");
	}
	if (!eglInitialize(egl_display, 0, 0))
	{
		throw std::runtime_error("Unable to initialize EGL.");
	}
	EGLint num_config;
	if(!eglChooseConfig(egl_display, attr, &ecfg, 1, &num_config))
	{
		throw std::runtime_error("Failed to choose config.");
	}
	if (num_config != 1)
	{
		std::stringstream ss;
		ss << "Didn't get exactly one EGL config, but " << num_config << ".";
		throw std::runtime_error(ss.str());
	}
	egl_context = eglCreateContext(egl_display, ecfg, EGL_NO_CONTEXT, ctxattr);
	if (egl_context == EGL_NO_CONTEXT)
	{
		throw std::runtime_error("Unable to create EGL context.");
	}
	if(!eglMakeCurrent(egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, egl_context))
	{
		throw std::runtime_error("Unable to attach the EGL rendering context to the EGL surface.");
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

aa::egl::~egl()
{
	eglDestroyContext(egl_display, egl_context);
	eglTerminate(egl_display);
}

void aa::egl::set_viewport(int x, int y, unsigned int width, unsigned int height)
{
	glViewport(x, y, width, height);
}

void aa::egl::clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void aa::egl::draw_elements(unsigned int count)
{
	glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, 0);
}

void aa::egl::swap_buffers(aa::surface &surf)
{
	eglSwapBuffers(egl_display, surf.egl_surface);
}

void aa::egl::make_surface_current(aa::surface &surf)
{
	if(!eglMakeCurrent(egl_display, surf.egl_surface, surf.egl_surface, egl_context))
	{
		throw std::runtime_error("Unable to attach the EGL rendering context to the EGL surface.");
	}
}

aa::surface aa::egl::create_window_surface(const window &wnd)
{
	EGLSurface egl_surface = eglCreateWindowSurface(egl_display, ecfg, (EGLNativeWindowType)wnd.get_native_window_handle(), 0);
	if (egl_surface == EGL_NO_SURFACE)
	{
		throw std::runtime_error("Unable to create EGL surface.");
	}
	return aa::surface(egl_display, egl_surface);
}

aa::texture aa::egl::create_texture(unsigned int width, unsigned int height)
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width,
			height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 0);

	return aa::texture(texture_id, width, height);
}

aa::shader aa::egl::create_shader(aa::shader::shader_type type, const std::string &source)
{
	GLuint shader_id;
	switch(type)
	{
		case aa::shader::shader_type::VERTEX_SHADER:
			shader_id = glCreateShader(GL_VERTEX_SHADER);
			break;
		case aa::shader::shader_type::FRAGMENT_SHADER:
			shader_id = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		//error
	}
	const char *c_source = source.c_str();
	glShaderSource(shader_id, 1, &c_source, 0);
	glCompileShader(shader_id);
	return shader(type, shader_id);
}

aa::program aa::egl::create_program(const aa::shader &vertex_shader, const aa::shader &fragment_shader)
{
	if(vertex_shader.type != aa::shader::shader_type::VERTEX_SHADER)
	{
		throw std::runtime_error("vertex_shader is not a vertex shader");
	}
	if(fragment_shader.type != aa::shader::shader_type::FRAGMENT_SHADER)
	{
		throw std::runtime_error("fragment_shader is not a fragment shader");
	}
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader.id);
	glAttachShader(program_id, fragment_shader.id);
	glLinkProgram (program_id);
	return program(program_id);
}

aa::buffer aa::egl::create_buffer(buffer::buffer_type type)
{
	GLuint buffer_id;
	glGenBuffers(1, &buffer_id);
	return aa::buffer(buffer_id, type);
}
