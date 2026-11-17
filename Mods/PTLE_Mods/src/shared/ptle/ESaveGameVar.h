#pragma once

#include "EStorable.h"


class ESaveGameVar : public EStorable
{
public:

	static const uint32_t VTABLE = 0x8A9358;


		/// 4 bytes of storage, for either ints or floats.
	uint32_t m_value;


	virtual void SetValue( uint32_t* ) = 0;
	virtual void GetValue( uint32_t* ) = 0;
	virtual void CopyValue( ESaveGameVar* ) = 0;
	virtual int func_unknown1() = 0;
};
