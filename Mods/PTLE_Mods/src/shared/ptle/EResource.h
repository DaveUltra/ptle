#pragma once

#include "EStorable.h"


class EResource : public EStorable
{
public:

	static const uint32_t VTABLE = 0x8A5710;


	char* m_name;
	void* m_archive;
	uint32_t m_crc;
	int m_refCount;
};
