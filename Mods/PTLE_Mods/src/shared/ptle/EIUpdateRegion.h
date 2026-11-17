#pragma once

#include "EInstance.h"

#include "ptle/containers/TreeMap/TreeMap.h"


struct UpdateEntry
{
	EInstance* m_instance;
	bool unknown;
	UpdateEntry* m_prev;
	UpdateEntry* m_next;
};

class EIUpdateRegion : public EInstance
{
public:

	static const uint32_t VTABLE = 0x8ADEB8;


	TreeMap m_treeMap;

	UpdateEntry* m_first;
	UpdateEntry* m_last;

	bool m_smallList;   // true = use linked list, false = use tree map.
};
