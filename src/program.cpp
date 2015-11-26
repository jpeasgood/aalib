#include "../headers/program.h"

aa::program::program(GLuint id) : id(id)
{
}

aa::program::~program()
{
	glDeleteProgram(id);
}

void aa::program::use()
{
	glUseProgram(id);
}

aa::attribute_variable aa::program::get_attribute(const std::string &name)
{

	GLuint attribute_id = glGetAttribLocation(id, name.c_str());
	return aa::attribute_variable(attribute_id);
}

aa::uniform_variable aa::program::get_uniform(const std::string &name)
{
	GLuint uniform_id = glGetUniformLocation(id, name.c_str());
	return aa::uniform_variable(uniform_id);
}
