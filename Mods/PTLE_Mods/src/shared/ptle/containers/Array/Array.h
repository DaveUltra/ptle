#pragma once

#include <stdint.h>


class Array
{
public:

	void* m_data;
	uint32_t m_size;
	uint32_t m_capacity;
};
