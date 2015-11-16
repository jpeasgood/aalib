#include "../headers/texture.h"

aa::texture::texture(GLuint id) : id(id), type(aa::texture::texture_type::TEXTURE_2D)
{
}

aa::texture::~texture()
{
	glDeleteTextures(1, &id);
}
