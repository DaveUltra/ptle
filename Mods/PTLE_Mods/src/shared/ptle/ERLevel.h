#pragma once

#include "EResource.h"

#include "EIUpdateRegion.h"

#include "ptle/containers/HashMap/HashMap.h"


class ERLevel : public EResource
{
public:

	static const uint32_t VTABLE = 0x8A5D08;


	char padding1[0x42C];

	EIUpdateRegion m_updateRegion;
	EInstance instanceUnknown;

	char padding2[0x7C];

	HashMap m_entitiesByID;

	char padding3[0x94];
};
