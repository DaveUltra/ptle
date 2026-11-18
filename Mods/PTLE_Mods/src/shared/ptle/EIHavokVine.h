#pragma once

#include "EIGameInstance.h"

#include "EAnimController.h"

class EIHarry;


class EIHavokVine : public EIGameInstance
{
public:

	static const uint32_t VTABLE = 0x880AF8;


	char padding0[0xC];

	Vector3f unknown0;

	char padding1[0x38];

	EAnimController m_animController;

	char padding2[0xD8];

	EIHarry* m_harry;

	char padding3[0x70];
};
