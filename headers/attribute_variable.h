#ifndef AA_ATTRIBUTE_VARIABLE_H
#define AA_ATTRIBUTE_VARIABLE_H

#include <GLES2/gl2.h>

namespace aa
{
	class attribute_variable
	{
		friend class program;

		private:
			GLuint id;

			attribute_variable(GLuint id);

		public:
			attribute_variable(const attribute_variable &other) = delete;
			attribute_variable(attribute_variable &&other) = default;

			attribute_variable &operator=(const attribute_variable &other) = delete;
			attribute_variable &operator=(attribute_variable &&other) = default;

			void enable();
			void disable();
			void set_float_attribute_ptr(unsigned int size, unsigned int stride, unsigned int offset);
	};
}

#endif
