#pragma once

#include "EResource.h"

#include "ptle/containers/TreeMap/TreeMap.h"
#include "ptle/containers/Array/Array.h"
#include "ptle/containers/LinkedList/LinkedList.h"

class EScriptContext;
class EScriptData;


class ERScript : public EResource
{
public:

	static const uint32_t VTABLE = 0x8B0614;


	TTreeMap<EInstance*, EScriptContext*> m_aliveContexts;

	TLinkedList<EScriptData*> m_scriptData;
	TArray<uint32_t> m_bytecode;
	Array      m_stringArray;

	int m_count;
};
