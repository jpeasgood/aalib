#include "../headers/texture.h"

aa::texture::texture(GLuint id, unsigned int width, unsigned int height)
	: id(id),
	  width(width),
	  height(height),
	  type(aa::texture::texture_type::TEXTURE_2D)
{
}

aa::texture::~texture()
{
	glDeleteTextures(1, &id);
}

void aa::texture::set(const void *data)
{
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
				GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
}

void aa::texture::bind()
{
	switch(type)
	{
		case aa::texture::texture_type::TEXTURE_2D:
			glBindTexture(GL_TEXTURE_2D, id);
			break;
		//error
	}
}

unsigned int aa::texture::get_width() const
{
	return width;
}

unsigned int aa::texture::get_height() const
{
	return height;
}
