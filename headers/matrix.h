#ifndef AA_MATRIX_H
#define AA_MATRIX_H

#include "../headers/vector.h"

#include <memory>

namespace aa
{
	template<typename T>
	class vector;

	template<typename T>
	class matrix
	{
		private:
			std::shared_ptr<T> element_ptr;
			unsigned int rows;
			unsigned int cols;

		public:
			matrix(unsigned int rows, unsigned int cols);
			matrix(const matrix<T> &other);
			matrix(matrix<T> &&other) = default;

			matrix<T> &operator=(const matrix<T> &other);
			matrix<T> &operator=(matrix<T> &&other) = default;

			vector<T> operator[](unsigned int i) const;
			vector<T> operator[](unsigned int i);

			matrix<T> operator+(const matrix<T> &other) const;
			matrix<T> operator-(const matrix<T> &other) const;
			matrix<T> operator*(const matrix<T> &other) const;

			matrix<T> &operator+=(const matrix<T> &other);
			matrix<T> &operator-=(const matrix<T> &other);
			matrix<T> &operator*=(const matrix<T> &other);

			bool operator==(const matrix<T> &other) const;
			bool operator!=(const matrix<T> &other) const;

			unsigned int num_rows() const;
			unsigned int num_cols() const;
			matrix<T> transpose() const;
			T *get_raw_ptr();
	};

	template<typename T>
	matrix<T> ortho_matrix(T left, T right, T bottom, T top, T near, T far);
}

template<typename T>
aa::matrix<T>::matrix(unsigned int rows, unsigned int cols)
	: element_ptr(new T[rows * cols], std::default_delete<T[]>()),
	  rows(rows),
	  cols(cols)
{
	T *p = element_ptr.get();
	for(unsigned int i = 0; i < rows; i++)
	{
		for(unsigned int j = 0; j < cols; j++)
		{
			p[i * cols + j] = T();
		}
	}
}

template<typename T>
aa::matrix<T>::matrix(const aa::matrix<T> &other)
	: element_ptr(new T[other.rows * other.cols], std::default_delete<T[]>()),
	  rows(other.rows),
	  cols(other.cols)
{
	T *p = element_ptr.get();
	T *q = other.element_ptr.get();
	for(unsigned int i = 0; i < rows; i++)
	{
		for(unsigned int j = 0; j < cols; j++)
		{
			p[i * cols + j] = q[i * cols + j];
		}
	}
}

template<typename T>
aa::matrix<T> &aa::matrix<T>::operator=(const aa::matrix<T> &other)
{
	if(this != &other)
	{
		element_ptr = std::shared_ptr<T>(new T[other.rows * other.cols], std::default_delete<T[]>());
		rows = other.rows;
		cols = other.cols;
		T *p = element_ptr.get();
		T *q = other.element_ptr.get();
		for(unsigned int i = 0; i < rows; i++)
		{
			for(unsigned int j = 0; j < cols; j++)
			{
				p[i * cols + j] = q[i * cols + j];
			}
		}
	}
}

template<typename T>
aa::vector<T> aa::matrix<T>::operator[](unsigned int i) const
{
	aa::vector<T> ret(rows);
	T *p = element_ptr.get();
	for(unsigned int j = 0; j < rows; j++)
	{
		ret[j] = p[i * rows + j];
	}
	return ret;
}

template<typename T>
aa::vector<T> aa::matrix<T>::operator[](unsigned int i)
{
	return aa::vector<T>(element_ptr, i * rows, rows);
}

template<typename T>
aa::matrix<T> aa::matrix<T>::operator+(const aa::matrix<T> &other) const
{
	if(rows != other.rows || cols != other.cols)
	{
		throw std::runtime_error("Matrix dimensions must be equal when adding matrices.");
	}
	matrix<T> ret(*this);
	return ret += other;
}

template<typename T>
aa::matrix<T> aa::matrix<T>::operator-(const aa::matrix<T> &other) const
{
	if(rows != other.rows || cols != other.cols)
	{
		throw std::runtime_error("Matrix dimensions must be equal when subtracting matrices.");
	}
	matrix<T> ret(*this);
	return ret -= other;
}

template<typename T>
aa::matrix<T> aa::matrix<T>::operator*(const aa::matrix<T> &other) const
{
	if(cols != other.rows)
	{
		throw std::runtime_error("Number of rows in a matrix must equal number of columns in the other when multiplying matrices.");
	}
	matrix<T> ret(*this);
	return ret *= other;
}

template<typename T>
aa::matrix<T> &aa::matrix<T>::operator+=(const aa::matrix<T> &other)
{
	if(rows != other.rows || cols != other.cols)
	{
		throw std::runtime_error("Matrix dimensions must be equal when adding matrices.");
	}
	T *p = element_ptr.get();
	T *q = other.element_ptr.get();
	for(unsigned int i = 0; i < rows; i++)
	{
		for(unsigned int j = 0; j < cols; j++)
		{
			p[i * cols + j] += q[i * cols + j];
		}
	}
}

template<typename T>
aa::matrix<T> &aa::matrix<T>::operator-=(const aa::matrix<T> &other)
{
	if(rows != other.rows || cols != other.cols)
	{
		throw std::runtime_error("Matrix dimensions must be equal when subtracting matrices.");
	}
	T *p = element_ptr.get();
	T *q = other.element_ptr.get();
	for(unsigned int i = 0; i < rows; i++)
	{
		for(unsigned int j = 0; j < cols; j++)
		{
			p[i * cols + j] -= q[i * cols + j];
		}
	}
}

template<typename T>
aa::matrix<T> &aa::matrix<T>::operator*=(const aa::matrix<T> &other)
{
	if(cols != other.rows)
	{
		throw std::runtime_error("Number of rows in a matrix must equal number of columns in the other when multiplying matrices.");
	}
	matrix<T> m(std::move(*this));
	element_ptr = std::shared_ptr<T>(new T[m.rows * other.cols], std::default_delete<T[]>());
	rows = m.rows;
	cols = other.cols;
	T *p = element_ptr.get();
	T *q = m.element_ptr.get();
	T *r = other.element_ptr.get();
	for(unsigned int i = 0; i < rows; i++)
	{
		for(unsigned int j = 0; j < cols; j++)
		{
			p[i * cols + j] = T();
			for(unsigned int k = 0; k < m.cols; k++)
			{
				p[i * cols + j] += q[i * cols + k] * r[k * other.cols + j];
			}
		}
	}
	return *this;
}

template<typename T>
bool aa::matrix<T>::operator==(const aa::matrix<T> &other) const
{
	if(rows != other.rows || cols != other.cols)
	{
		return false;
	}
	T *p = element_ptr.get();
	T *q = other.element_ptr.get();
	for(unsigned int i = 0; i < rows; i++)
	{
		for(unsigned int j = 0; j < cols; j++)
		{
			if(p[i * cols + j] != q[i * cols + j])
			{
				return false;
			}
		}
	}
	return true;
}

template<typename T>
bool aa::matrix<T>::operator!=(const aa::matrix<T> &other) const
{
	return !(*this == other);
}

template<typename T>
unsigned int aa::matrix<T>::num_rows() const
{
	return rows;
}

template<typename T>
unsigned int aa::matrix<T>::num_cols() const
{
	return cols;
}

template<typename T>
aa::matrix<T> aa::matrix<T>::transpose() const
{
	matrix<T> ret(cols, rows);
	T *p = ret.element_ptr.get();
	T *q = element_ptr.get();
	for(unsigned int i = 0; i < rows; i++)
	{
		for(unsigned int j = 0; j < cols; j++)
		{
			p[j * rows + i] = q[i * cols + j];
		}
	}
	return ret;
}

template<typename T>
T *aa::matrix<T>::get_raw_ptr()
{
	return element_ptr.get();
}

template<typename T>
aa::matrix<T> aa::ortho_matrix(T left, T right, T bottom, T top, T near, T far)
{
	aa::matrix<T> res(4, 4);
	T r_l = right - left;
	T t_b = top - bottom;
	T f_n = far - near;
	T tx = - (right + left) / (right - left);
	T ty = - (top + bottom) / (top - bottom);
	T tz = (far + near) / (far - near);
	res[0][0] = 2.0f / r_l;
	res[0][1] = 0.0f;
	res[0][2] = 0.0f;
	res[0][3] = 0.0f;
	res[1][0] = 0.0f;
	res[1][1] = 2.0f / t_b;
	res[1][2] = 0.0f;
	res[1][3] = 0.0;
	res[2][0] = 0.0f;
	res[2][1] = 0.0f;
	res[2][2] = 2.0f / f_n;
	res[2][3] = 0.0;
	res[3][0] = tx;
	res[3][1] = ty;
	res[3][2] = tz;
	res[3][3] = 1.0f;
	return res;
}

#endif
