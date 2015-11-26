#ifndef AA_UNIFORM_VARIABLE_H
#define AA_UNIFORM_VARIABLE_H

#include "../headers/vector.h"
#include "../headers/matrix.h"

#include <GLES2/gl2.h>

namespace aa
{
	class uniform_variable
	{
		friend class program;

		private:
			GLuint id;

			uniform_variable(GLuint id);

		public:
			uniform_variable(const uniform_variable &other) = delete;
			uniform_variable(uniform_variable &&other) = default;

			uniform_variable &operator=(const uniform_variable &other) = delete;
			uniform_variable &operator=(uniform_variable &&other) = default;

			void set(int v0);
			void set(int v0, int v2);
			void set(int v0, int v1, int v2);
			void set(int v0, int v1, int v2, int v3);
			void set(float v0);
			void set(float v0, float v2);
			void set(float v0, float v1, float v2);
			void set(float v0, float v1, float v2, float v3);
			void set(vector<int> &v);
			void set(vector<float> &v);
			void set(matrix<float> &m);
	};
}

#endif
