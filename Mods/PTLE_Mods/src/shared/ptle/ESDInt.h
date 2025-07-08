#pragma once

#include "EStorable.h"


class ESDInt : public EStorable
{
public:

	unsigned int m_refCount;
	int m_value;


	virtual const char* GetSaveVarTypeString();
	virtual void func_unknown1();
	virtual void func_unknown2();
	virtual bool IsValueNotZero();
	virtual void CopyValue(ESDInt* other);
};
