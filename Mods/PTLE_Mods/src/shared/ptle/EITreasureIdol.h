#pragma once

#include "EIDynamicModel.h"


class EITreasureIdol : public EIDynamicModel
{
public:

	char padding5[0x10];
	EInstance* m_particleEmitter;
	char padding6[0x1C];
};
