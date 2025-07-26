#pragma once

#include "EStorable.h"

#include <stdint.h>


class ESaveGameVar : public EStorable
{
public:

		/// 4 bytes of storage, for either ints or floats.
	uint32_t m_value;


	virtual void SetValue( uint32_t* ) = 0;
	virtual void GetValue( uint32_t* ) = 0;
	virtual void CopyValue( ESaveGameVar* ) = 0;
	virtual int func_unknown1() = 0;
};
