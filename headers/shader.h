#ifndef AA_SHADER_H
#define AA_SHADER_H

#include <GLES2/gl2.h>

namespace aa
{
	class shader
	{
		friend class egl;

		public:
			enum class shader_type {VERTEX_SHADER, FRAGMENT_SHADER};

		private:
			GLuint id;
			shader_type type;

			shader(shader_type type, GLuint id);

		public:
			shader(const shader &other) = delete;
			shader(shader &&other) = default;
			~shader();

			shader &operator=(const shader &other) = delete;
			shader &operator=(shader &&other) = default;

			shader_type get_type();
	};
}

#endif
