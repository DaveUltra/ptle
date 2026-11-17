#pragma once

#include "EStorable.h"

#include "ptle/math/Matrix4f.h"

class EInstance;
class ERScript;
class EScriptData;


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

	char padding1[0x60];

	void* m_currentFunction;

	char padding2[0x8];
};
