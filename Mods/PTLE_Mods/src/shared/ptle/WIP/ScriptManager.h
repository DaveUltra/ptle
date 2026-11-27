#pragma once

#include "ptle/containers/TreeMap/TreeMap.h"

class ScriptFunction;

class ETypeInfo;
class EInstance;
class ERScript;
class EScriptData;


class ScriptManager
{
public:

	static inline ScriptManager* getInstance() { return (ScriptManager*) 0x91F918; }


	ETypeInfo* m_scriptDataTypes[256];
	ScriptFunction* m_funcLibsPointers[32];
	int m_funcLibsNumFuncs[32];
	int m_funcLibsNum;
	TTreeMap<EInstance*, ERScript*> m_scriptsByInstance;
	TTreeMap<uint32_t, EScriptData*> m_globalScriptDatas;
	TreeMap idk2;
	TreeMap idk3;
	
	char padding0[0x78];


	virtual ~ScriptManager();
};
