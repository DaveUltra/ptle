#pragma once

#include <stdint.h>

class ETypeInfo;


class EStorable
{
public:

	static const uint32_t VTABLE = 0x86DB20;


	virtual ~EStorable();

	virtual uint32_t    GetSizeInBytes() const;
	virtual ETypeInfo*  GetType() const;
	virtual bool        IsOfType(ETypeInfo*) const;
	virtual const char* GetTypeName() const;
	virtual uint32_t    GetTypeHash() const;
	virtual uint32_t    func_unknown0();
	virtual EStorable*  Destroy();
	virtual void        Load(void*);
	virtual void        Save(void*);
};
