#pragma once

#include "EIBeast.h"


class EIMegaMonkeyChild : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x889700;


	char padding7[0x70];
};
