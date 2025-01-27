#pragma once

#include "EStorable.h"


class EResource : public EStorable
{
public:

	char* m_name;
	void* m_archive;
	uint32_t m_crc;
	int m_refCount;
};
