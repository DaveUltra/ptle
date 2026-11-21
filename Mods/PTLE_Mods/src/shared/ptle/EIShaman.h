#pragma once

#include "EIBeast.h"


class EIShaman : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x896C10;


	bool m_shamanHarryInRange;
	bool m_shamanStillThere;
	bool m_shamanTalkingToHarry;
	char padding[13];
};
