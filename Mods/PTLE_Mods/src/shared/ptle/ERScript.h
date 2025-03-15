#pragma once

#include "EResource.h"

#include "ptle/containers/TreeMap/TreeMap.h"


class ERScript : public EResource
{
public:

	TreeMap m_aliveContexts;
	char padding1[0x20];

	int m_count;
};
