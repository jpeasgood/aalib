#include "../headers/attribute_variable.h"

aa::attribute_variable::attribute_variable(GLuint id) : id(id)
{
}

void aa::attribute_variable::enable()
{
	glEnableVertexAttribArray(id);
}

void aa::attribute_variable::disable()
{
	glDisableVertexAttribArray(id);
}

void aa::attribute_variable::set_float_attribute_ptr(unsigned int size, unsigned int stride, unsigned int offset)
{
	glVertexAttribPointer(id, size, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(offset));
}

