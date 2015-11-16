#ifndef AA_PROGRAM_H
#define AA_PROGRAM_H

#include <GLES2/gl2.h>

namespace aa
{
	class program
	{
		friend class egl;

		private:
			GLuint id;

			program(GLuint id);

		public:
			program(const program &other) = delete;
			program(program &&other) = default;
			~program();

			program &operator=(const program &other) = delete;
			program &operator=(program &&other) = default;
	};
}

#endif
