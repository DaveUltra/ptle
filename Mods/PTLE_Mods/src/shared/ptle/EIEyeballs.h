#pragma once

#include "EIBeast.h"


class EIEyeballs : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x876800;


	char padding[0x80];
};
