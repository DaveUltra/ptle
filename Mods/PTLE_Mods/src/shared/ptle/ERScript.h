#pragma once

#include "EResource.h"

#include "ptle/containers/TreeMap/TreeMap.h"
#include "ptle/containers/Array/Array.h"
#include "ptle/containers/LinkedList/LinkedList.h"


class ERScript : public EResource
{
public:

	TreeMap m_aliveContexts;

	LinkedList m_scriptData;   // List of EScriptData*.
	Array      m_bytecode;     // Array of unsigned int.
	Array      m_stringArray;

	int m_count;
};
