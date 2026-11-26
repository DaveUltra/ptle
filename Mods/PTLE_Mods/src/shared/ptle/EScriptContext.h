#pragma once

#include "EStorable.h"

#include "ptle/math/Matrix4f.h"

class EInstance;
class ERScript;
class EScriptData;


struct ScriptAdditionalStruct
{
	void* unknown0;
	Matrix4f unknown1;
};

struct ScriptFunction
{
	const char* m_functionName;
	const char* m_paramTypes;
	int unknown;
	void (*m_functionPtr)(class EScriptContext*);
};

class EScriptContext : public EStorable
{
public:

	static const uint32_t VTABLE = 0x8AEAF4;


	int m_bytecodePointer;
	int m_stackPointer;
	ERScript* m_script;
	EInstance* m_owningInstance;

	bool unknown0;
	bool m_sleeping;

	uint32_t m_sleepFrameStart;
	float m_sleepDuration;

	void* unknown1;
	EScriptData* m_scriptStack[512];

	ScriptAdditionalStruct unknown2;

	bool unknown3;

	ScriptFunction* m_currentFunction;
};
