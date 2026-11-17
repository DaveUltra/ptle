#pragma once

#include "EResource.h"

#include "ptle/containers/LinkedList/LinkedList.h"


class ERDataset : public EResource
{
public:

	static const uint32_t VTABLE = 0x8AF208;


	LinkedList unknown0;
};
