#include "../headers/egl.h"

aa::egl::egl(const aa::window &wnd)
{
	EGLConfig ecfg;
	EGLint attr[] = { EGL_BUFFER_SIZE, 24, EGL_NONE};
	EGLint ctxattr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	if(!eglBindAPI(EGL_OPENGL_ES_API))
	{
		std::stringstream ss;
		ss << "EGL failed to bind OPENGL ES.";
		throw std::runtime_error(ss.str());
	}
	egl_display = eglGetDisplay(static_cast<EGLNativeDisplayType>(wnd.get_native_display()));
	if (egl_display == EGL_NO_DISPLAY)
	{
		std::stringstream ss;
		ss << "Error getting EGL display.";
		throw std::runtime_error(ss.str());
	}
	if (!eglInitialize(egl_display, 0, 0))
	{
		std::stringstream ss;
		ss << "Unable to initialize EGL.";
		throw std::runtime_error(ss.str());
	}
	EGLint num_config;
	if(!eglChooseConfig(egl_display, attr, &ecfg, 1, &num_config))
	{
		std::stringstream ss;
		ss << "Failed to choose config.";
		throw std::runtime_error(ss.str());
	}
	if (num_config != 1)
	{
		std::stringstream ss;
		ss << "Didn't get exactly one EGL config, but " << num_config << ".";
		throw std::runtime_error(ss.str());
	}
	egl_surface = eglCreateWindowSurface(egl_display, ecfg, (EGLNativeWindowType)wnd.get_native_window_handle(), NULL);
	if (egl_surface == EGL_NO_SURFACE)
	{
		std::stringstream ss;
		ss << "Unable to create EGL surface.";
		throw std::runtime_error(ss.str());
	}
	egl_context = eglCreateContext(egl_display, ecfg, EGL_NO_CONTEXT, ctxattr);
	if (egl_context == EGL_NO_CONTEXT)
	{
		std::stringstream ss;
		ss << "Unable to create EGL context.";
		throw std::runtime_error(ss.str());
	}
	if(!eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context))
	{
		std::stringstream ss;
		ss << "Unable to attach the EGL rendering context to the EGL surface.";
		throw std::runtime_error(ss.str());
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

aa::egl::~egl()
{
	eglDestroyContext(egl_display, egl_context);
	eglDestroySurface(egl_display, egl_surface);
	eglTerminate(egl_display);
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
			height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, NULL);

	return texture(texture_id);
}

void aa::egl::bind_texture(const texture &tex)
{
	switch(tex.type)
	{
		case aa::texture::texture_type::TEXTURE_2D:
			glBindTexture(GL_TEXTURE_2D, tex.id);
			break;
		//error
	}

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

void aa::egl::use_program(const program &prog)
{
	glUseProgram(prog.id);
}
