#pragma once

class ETypeInfo;


class EStorable
{
public:

	virtual ~EStorable();

	virtual uint32_t    GetSizeInBytes();
	virtual ETypeInfo*  GetType();
	virtual bool        IsOfType(ETypeInfo*);
	virtual const char* GetTypeName();
	virtual uint32_t    GetTypeHash();
	virtual uint32_t    func_unknown0();
	virtual EStorable*  Destroy();
	virtual void        Load(void*);
	virtual void        Save(void*);
};
