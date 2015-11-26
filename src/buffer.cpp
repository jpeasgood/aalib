#include "../headers/buffer.h"

aa::buffer::buffer(GLuint id, aa::buffer::buffer_type type) : id(id), type(type)
{
}

aa::buffer::~buffer()
{
	glDeleteBuffers(1, &id);
}

void aa::buffer::bind()
{
	switch(type)
	{
		case aa::buffer::buffer_type::VERTEX:
			glBindBuffer(GL_ARRAY_BUFFER, id);
			break;
		case aa::buffer::buffer_type::INDEX:
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
			break;
		//error
	}

}

void aa::buffer::set(unsigned int size, const void *data)
{
	switch(type)
	{
		case aa::buffer::buffer_type::VERTEX:
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			break;
		case aa::buffer::buffer_type::INDEX:
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			break;
		//error
	}

}
