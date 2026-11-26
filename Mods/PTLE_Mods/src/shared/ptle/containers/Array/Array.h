#pragma once

#include <stdint.h>


class Array
{
public:

	void* m_data;
	uint32_t m_size;
	uint32_t m_capacity;
};

template<typename T>
class TArray
{
public:

	T* m_data;
	uint32_t m_size;
	uint32_t m_capacity;


	// Range-based for loops.
	const T* begin() const { return m_data; }
	const T* end() const { return m_data + m_size; }
};
