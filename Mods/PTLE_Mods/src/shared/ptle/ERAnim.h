#pragma once

#include "EResource.h"


class ERAnim : public EResource
{
public:

	static const uint32_t VTABLE = 0x8AE110;


	int unknown0;

	char padding0[0x4];

	void* m_framesData;
	uint32_t m_framesCount;

	char padding1[0x58];
};
