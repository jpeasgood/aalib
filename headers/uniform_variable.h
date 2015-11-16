#ifndef AA_UNIFORM_VARIABLE_H
#define AA_UNIFORM_VARIABLE_H

#include <GLES2/gl2.h>

namespace aa
{
	class uniform_variable
	{
		private:
			GLuint id;

			uniform_variable(GLuint id);

		public:
			uniform_variable(const uniform_variable &other) = delete;
			uniform_variable(uniform_variable &&other) = default;

			uniform_variable &operator=(const uniform_variable &other) = delete;
			uniform_variable &operator=(uniform_variable &&other) = default;

			template<typename... Args>
			void set(Args... args);
	};
}

template<>
void aa::uniform_variable::set<int>(int v0)
{
	glUniform1i(id, v0);
}

template<>
void aa::uniform_variable::set<int, int>(int v0, int v1)
{
	glUniform2i(id, v0, v1);
}

template<>
void aa::uniform_variable::set<int, int, int>(int v0, int v1, int v2)
{
	glUniform3i(id, v0, v1, v2);
}

template<>
void aa::uniform_variable::set<int, int, int, int>(int v0, int v1, int v2, int v3)
{
	glUniform4i(id, v0, v1, v2, v3);
}

template<>
void aa::uniform_variable::set<float>(float v0)
{
	glUniform1f(id, v0);
}

template<>
void aa::uniform_variable::set<float, float>(float v0, float v1)
{
	glUniform2f(id, v0, v1);
}

template<>
void aa::uniform_variable::set<float, float, float>(float v0, float v1, float v2)
{
	glUniform3f(id, v0, v1, v2);
}

template<>
void aa::uniform_variable::set<float, float, float, float>(float v0, float v1, float v2, float v3)
{
	glUniform4f(id, v0, v1, v2, v3);
}

#endif
