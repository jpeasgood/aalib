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

#include <iostream>
#include "../headers/egl.h"
aa::attribute_variable aa::program::get_attribute(const std::string &name)
{
	GLuint attribute_id = glGetAttribLocation(id, name.c_str());
	std::cout << "get attribute" << std::endl;
	check_gl_error();
	return aa::attribute_variable(attribute_id);
}

aa::uniform_variable aa::program::get_uniform(const std::string &name)
{
	GLuint uniform_id = glGetUniformLocation(id, name.c_str());
	std::cout << "get uniform" << std::endl;
	check_gl_error();
	return aa::uniform_variable(uniform_id);
}
