#pragma once

#include "EInstance.h"

#include "ptle/containers/TreeMap/TreeMap.h"


class EIUpdateRegion : public EInstance
{
public:

	TreeMap m_treeMap;

	char padding2[0x0C];
};
