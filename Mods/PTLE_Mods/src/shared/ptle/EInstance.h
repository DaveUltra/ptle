#pragma once

#include "EStorable.h"

class ERLevel;


class EInstance : public EStorable
{
public:

	int unused0;
	int unused1;
	ERLevel* m_world;
	int unused2;

	char padding0[0x18];

	uint32_t m_flags;

	char padding1[0x34];
};
