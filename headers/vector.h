#ifndef AA_MATRIX_H
#define AA_MATRIX_H

#include "../headers/vector.h"

#include <memory>

namespace aa
{
	template<typename T>
	class vector
	{
		friend class matrix<T>;

		private:
			std::shared_ptr<T> element_ptr;
			unsigned int offset;
			unsigned int length;

			vector(std::shared_ptr<T> ptr, unsigned int offset, unsigned int length);

		public:
			vector(int length);
			vector(const vector<T> &other);
			vector(vector<T> &&other) = default;

			vector<T> &operator=(const vector<T> &other);
			vector<T> &operator=(vector<T> &&other) = default;

			T operator[](unsigned int i) const;
			T &operator[](unsigned int i);

			vector<T> operator+(const vector<T> &other) const;
			vector<T> operator-(const vector<T> &other) const;

			vector<T> &operator+=(const vector<T> &other);
			vector<T> &operator-=(const vector<T> &other);

			vector<T> operator*(const T &other) const;
			vector<T> operator/(const T &other) const;

			vector<T> &operator*=(const T &other);
			vector<T> &operator/=(const T &other);

			bool operator==(const vector<T> &other) const;
			bool operator!=(const vector<T> &other) const;

			unsigned int size() const;
			T *get_raw_ptr() const;
	};

	template<typename T>
	vector<T> operator*(const T &c, const vector<T> &v);

	template<typename T>
	T dot(const vector<T> &v, const vector<T> &u);
}

template<typename T>
aa::vector<T>::vector(std::shared_ptr<T> ptr, unsigned int offset, unsigned int length)
	:  element_ptr(ptr),
	   offset(offset),
	   length(length)
{
}

template<typename T>
aa::vector<T>::vector(int length)
	: element_ptr(new T[length], std::default_delete<T[]>()),
	  offset(0),
	  length(length)
{
	T *p = element_ptr.get();
	for(unsigned int i = 0; i < length; i++)
	{
		p[i] = T();
	}
}

template<typename T>
aa::vector<T>::vector(const aa::vector<T> &other)
	: element_ptr(new T[other.length], std::default_delete<T[]>()),
	  offset(0),
	  length(other.length)
{
	T *p = element_ptr.get();
	T *q = other.element_ptr.get();
	for(unsigned int i = 0; i < length; i++)
	{
		p[i] = q[other.offset + i];
	}
}

template<typename T>
aa::vector<T> &aa::vector<T>::operator=(const aa::vector<T> &other)
{
	if(this != &other)
	{
		element_ptr = shared_ptr<T>(new T[other.length], std::default_delete<T[]>());
		offset = 0;
		length = other.length;
		T *p = element_ptr.get();
		T *q = other.element_ptr.get();
		for(unsigned int i = 0; i < length; i++)
		{
			p[i] = q[other.offset + i];
		}
	}
}

template<typename T>
T aa::vector<T>::operator[](unsigned int i) const
{
	return element_ptr.get()[offset + i];
}

T &aa::vector<T>::operator[](unsigned int i)
{
	return element_ptr.get()[offset + i];
}

template<typename T>
aa::vector<T> aa::vector<T>::operator+(const aa::vector<T> &other) const
{
	if(length != other.length)
	{
		throw std::runtime_error("Vector dimensions must be equal when adding vectors.");
	}
	vector<T> ret(*this);
	return ret += other;
}

template<typename T>
aa::vector<T> aa::vector<T>::operator-(const aa::vector<T> &other) const
{
	if(length != other.length)
	{
		throw std::runtime_error("Vector dimensions must be equal when subtracting vectors.");
	}
	vector<T> ret(*this);
	return ret -= other;
}

template<typename T>
aa::vector<T> &aa::vector<T>::operator+=(const aa::vector<T> &other)
{
	if(length != other.length)
	{
		throw std::runtime_error("Vector dimensions must be equal when adding vectors.");
	}
	T *p = element_ptr.get();
	T *q = other.element_ptr.get();
	for(unsigned int i = 0; i < length; i++)
	{
		p[offset + i] += q[other.offset + i];
	}
	return *this;
}

template<typename T>
aa::vector<T> &aa::vector<T>::operator-=(const aa::vector<T> &other)
{
	if(length != other.length)
	{
		throw std::runtime_error("Vector dimensions must be equal when subtracting vectors.");
	}
	T *p = element_ptr.get();
	T *q = other.element_ptr.get();
	for(unsigned int i = 0; i < length; i++)
	{
		p[offset + i] -= q[other.offset + i];
	}
	return *this;
}

template<typename T>
aa::vector<T> aa::vector<T>::operator*(const T &other) const
{
	vector<T> ret(*this);
	return ret *= other;
}

template<typename T>
aa::vector<T> aa::vector<T>::operator/(const T &other) const
{
	vector<T> ret(*this);
	return ret /= other;
}

template<typename T>
aa::vector<T> &aa::vector<T>::operator*=(const T &other)
{
	T *p = element_ptr.get();
	for(unsigned int i = 0; i < length; i++)
	{
		p[offset + i] *= other;
	}
	return *this;
}

template<typename T>
aa::vector<T> &aa::vector<T>::operator/=(const T &other)
{
	T *p = element_ptr.get();
	for(unsigned int i = 0; i < length; i++)
	{
		p[offset + i] /= other;
	}
	return *this;
}

template<typename T>
bool aa::vector<T>::operator==(const aa::vector<T> &other) const
{
	if(length != other.length)
	{
		return false;
	}
	T *p = element_ptr.get();
	for(unsigned int i = 0; i < length; i++)
	{
		if(p[offset + i] != q[other.offset + i])
		{
			return false;
		}
	}
	return true;
}

template<typename T>
bool aa::vector<T>::operator!=(const aa::vector<T> &other) const
{
	return !(*this == other);
}

template<typename T>
unsigned int aa::vector<T>::size() const
{
	return length;
}

template<typename T>
T *aa::vector<T>::get_raw_ptr() const
{
	return &element_ptr.get()[offset];
}

template<typename T>
aa::vector<T> operator*(const T &c, const aa::vector<T> &v)
{
	aa::vector<T> ret(v);
	for(unsigned int i = 0; i < ret.length(); i++)
	{
		ret[i] *= c;
	}
	return ret;
}

template<typename T>
T dot(const aa::vector<T> &v, const aa::vector<T> &u)
{
	if(v.length != u.length)
	{
		throw std::runtime_error("Vector dimensions must be equal for dot product.");
	}
	T ret = T();
	for(unsigned int i = 0; i < v.length(); i++)
	{
		ret += v[i] * u[i];
	}
	return ret;
}

#endif
