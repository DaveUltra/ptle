#pragma once

#include "EResource.h"


class ERAnim : public EResource
{
public:

	static const uint32_t VTABLE = 0x8AE110;


	int m_frames;

	char padding0[0x4];

	// Array.
	void* m_framesData;
	uint32_t m_framesCount;
	uint32_t m_framesCapacity;

	char padding1[0x4];

	void* m_arrUnknownData;
	uint32_t m_arrUnknownSize;
	uint32_t m_arrUnknownCapacity;

	int unknown0[5];

	char padding2[0x30];
};
