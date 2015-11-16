#include "../headers/shader.h"

aa::shader::shader(shader_type type, GLuint id) : type(type), id(id)
{
}

aa::shader::~shader()
{
	glDeleteShader(id);
}

aa::shader::shader_type aa::shader::get_type()
{
	return type;
}
