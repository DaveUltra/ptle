#pragma once

#include <stdint.h>


class EStream
{
public:

	virtual ~EStream();

	virtual uint32_t func_unknown0();
	virtual uint32_t Read( void* dst, uint32_t size );
	virtual uint32_t Write( const void* src, uint32_t size );
};
