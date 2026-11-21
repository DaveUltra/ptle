#pragma once

#include "EStorable.h"


class EScriptData : public EStorable
{
public:

	static const uint32_t VTABLE = 0x8AEB4C;


	unsigned int m_refCount;


	virtual const char* GetSaveVarTypeString();
	virtual void func_unknown1();
	virtual void func_unknown2();
	virtual bool IsValueNotZero();
	virtual void CopyValue(EScriptData* other);
};
