#include "../headers/shader.h"

aa::vertex_shader::vertex_shader(GLuint id) : id(id)
{
}

aa::vertex_shader::~vertex_shader()
{
	glDeleteShader(id);
}

aa::fragment_shader::fragment_shader(GLuint id) : id(id)
{
}

aa::fragment_shader::~fragment_shader()
{
	glDeleteShader(id);
}
