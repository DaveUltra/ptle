#pragma once

#include "EIBeast.h"


class EIShaman : public EIBeast
{
public:

	bool m_shamanHarryInRange;
	bool m_shamanStillThere;
	bool m_shamanTalkingToHarry;
	char padding[13];
};
