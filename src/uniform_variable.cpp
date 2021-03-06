#include "../headers/uniform_variable.h"

aa::uniform_variable::uniform_variable(GLuint id) : id(id)
{
}

void aa::uniform_variable::set(int v0)
{
	glUniform1i(id, v0);
}

void aa::uniform_variable::set(int v0, int v1)
{
	glUniform2i(id, v0, v1);
}

void aa::uniform_variable::set(int v0, int v1, int v2)
{
	glUniform3i(id, v0, v1, v2);
}

void aa::uniform_variable::set(int v0, int v1, int v2, int v3)
{
	glUniform4i(id, v0, v1, v2, v3);
}

void aa::uniform_variable::set(float v0)
{
	glUniform1f(id, v0);
}

void aa::uniform_variable::set(float v0, float v1)
{
	glUniform2f(id, v0, v1);
}

void aa::uniform_variable::set(float v0, float v1, float v2)
{
	glUniform3f(id, v0, v1, v2);
}

void aa::uniform_variable::set(float v0, float v1, float v2, float v3)
{
	glUniform4f(id, v0, v1, v2, v3);
}

void aa::uniform_variable::set(aa::vector<int> &v)
{
	switch(v.size())
	{
		case 1:
			glUniform1iv(id, 1, v.get_raw_ptr());
			break;
		case 2:
			glUniform2iv(id, 1, v.get_raw_ptr());
			break;
		case 3:
			glUniform3iv(id, 1, v.get_raw_ptr());
			break;
		case 4:
			glUniform4iv(id, 1, v.get_raw_ptr());
			break;
			//error
	}
}

void aa::uniform_variable::set(aa::vector<float> &v)
{
	switch(v.size())
	{
		case 1:
			glUniform1fv(id, 1, v.get_raw_ptr());
			break;
		case 2:
			glUniform2fv(id, 1, v.get_raw_ptr());
			break;
		case 3:
			glUniform3fv(id, 1, v.get_raw_ptr());
			break;
		case 4:
			glUniform4fv(id, 1, v.get_raw_ptr());
			break;
			//error
	}
}

void aa::uniform_variable::set(aa::matrix<float> &m)
{
	if(m.num_cols() == 2 && m.num_rows() == 2)
	{
		glUniformMatrix2fv(id, 1, GL_FALSE, m.get_raw_ptr());
	}
	else if(m.num_cols() == 3 && m.num_rows() == 3)
	{
		glUniformMatrix3fv(id, 1, GL_FALSE, m.get_raw_ptr());
	}
	else if(m.num_cols() == 4 && m.num_rows() == 4)
	{
		glUniformMatrix4fv(id, 1, GL_FALSE, m.get_raw_ptr());
	}
	//error
}
