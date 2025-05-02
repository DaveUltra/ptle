#pragma once

#include "EStorable.h"

class EInstance;


class EScriptContext : public EStorable
{
public:

	int unknown0;
	int m_stackPointer;
	int unknown2;

	EInstance* m_owningInstance;

	char padding0[0xC];

	void* unknown1;
	EStorable* /* ( EScriptData* ) */ m_scriptStack[512];

	char padding1[0x60];

	void* m_currentFunction;

	char padding2[0x8];
};
