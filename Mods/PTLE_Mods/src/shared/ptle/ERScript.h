#pragma once

#include "EResource.h"

#include "ptle/containers/TreeMap/TreeMap.h"
#include "ptle/containers/Array/Array.h"
#include "ptle/containers/LinkedList/LinkedList.h"


class ERScript : public EResource
{
public:

	TreeMap m_aliveContexts;

	LinkedList unknown0;
	Array unknown1;
	Array unknown2;

	int m_count;
};
