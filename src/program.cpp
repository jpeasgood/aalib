#include "../headers/program.h"

aa::program::program(GLuint id) : id(id)
{
}

aa::program::~program()
{
	glDeleteProgram(id);
}
