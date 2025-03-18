#pragma once

#include "EStorable.h"

#include "math/AABB.h"

class ERLevel;


class EInstance : public EStorable
{
public:

	uint32_t m_uniqueID;

	int unused1;
	ERLevel* m_world;
	int unused2;

	char padding0[0x18];

	uint32_t m_flags;

	char padding1[0xC];

	AABB m_bounds;

	char padding2[0xC];

	uint32_t m_frameCount;
};
