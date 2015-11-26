#ifndef AA_TEXTURE_H
#define AA_TEXTURE_H

#include <GLES2/gl2.h>

namespace aa
{
	class texture
	{
		friend class egl;

		public:
			enum class texture_type {TEXTURE_2D};

		private:
			GLuint id;
			texture_type type;
			unsigned int width;
			unsigned int height;

			texture(GLuint id, unsigned int width, unsigned int height);

		public:
			texture(const texture &other) = delete;
			texture(texture &&other) = default;
			~texture();

			texture &operator=(const texture &other) = delete;
			texture &operator=(texture &&other) = default;

			void set(const void *data);
			void bind();
	};
}

#endif
