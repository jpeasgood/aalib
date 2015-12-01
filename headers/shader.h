#ifndef AA_SHADER_H
#define AA_SHADER_H

#include "../headers/resource_manager.h"

#include <GLES2/gl2.h>

namespace aa
{
	class vertex_shader : public resource
	{
		friend class egl;

		private:
			GLuint id;

			vertex_shader(GLuint id);

		public:
			vertex_shader(const vertex_shader &other) = delete;
			vertex_shader(vertex_shader &&other) = default;
			virtual ~vertex_shader();

			vertex_shader &operator=(const vertex_shader &other) = delete;
			vertex_shader &operator=(vertex_shader &&other) = default;
	};

	class fragment_shader : public resource
	{
		friend class egl;

		private:
			GLuint id;

			fragment_shader(GLuint id);

		public:
			fragment_shader(const fragment_shader &other) = delete;
			fragment_shader(fragment_shader &&other) = default;
			virtual ~fragment_shader();

			fragment_shader &operator=(const fragment_shader &other) = delete;
			fragment_shader &operator=(fragment_shader &&other) = default;
	};
}

#endif
