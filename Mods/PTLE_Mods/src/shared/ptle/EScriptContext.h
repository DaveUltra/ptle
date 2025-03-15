#pragma once

#include "EStorable.h"


class EScriptContext : public EStorable
{
public:

	int unknown0;
	int m_stackPointer;

	char padding0[0x14];

	void* unknown1;
	EStorable* /* ( EScriptData* ) */ m_scriptStack[512];

	char padding1[0x60];

	void* m_currentFunction;

	char padding2[0x8];
};
