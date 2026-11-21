#pragma once

#include "EIBeast.h"


class EINPCBeast : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x88B868;


	char padding[0x40];
};
