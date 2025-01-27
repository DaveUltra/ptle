#pragma once

#include "EResource.h"

#include "EIUpdateRegion.h"


class ERLevel : public EResource
{
public:

	char padding1[0x42C];

	EIUpdateRegion m_updateRegion;
	EInstance instanceUnknown;

	char padding2[0x124];
};
