#ifndef AA_BUFFER_H
#define AA_BUFFER_H

#include <GLES2/gl2.h>

namespace aa
{
	class buffer
	{
		friend class egl;

		public:
			enum class buffer_type {VERTEX, INDEX};

		private:
			GLuint id;
			buffer_type type;

			buffer(GLuint id, buffer_type type);

		public:
			buffer(const buffer &other) = delete;
			buffer(buffer &&other) = default;
			~buffer();

			buffer &operator=(const buffer &other) = delete;
			buffer &operator=(buffer &&other) = default;

			void bind();
			void set(unsigned int size, const void *data);
	};
}

#endif
