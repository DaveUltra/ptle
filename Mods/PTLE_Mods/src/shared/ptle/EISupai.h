#pragma once

#include "EIBeast.h"


class EISupai : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x8999C8;


	char padding7[0x2A1];

	bool m_isTutorialSupai;

	char padding8[0x6E];
};
