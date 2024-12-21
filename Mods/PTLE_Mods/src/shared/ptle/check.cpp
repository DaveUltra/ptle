#include <stdint.h>

#include "EIHarry.h"
#include "EIProjectile.h"


// Use static_assert() in C++11 and above.
#if __cplusplus >= 201103L


#define CHECK_TYPE_SIZE( type, size ) \
	static_assert( sizeof(type) == size, "Type size mismatch" )

#define CHECK_FIELD_OFFSET( type, field, offset ) \
	static_assert( (uint32_t) &reinterpret_cast<type*>(0)->field == offset, "Field offset mismatch" )


#else  // Otherwise, resort to template magic.


template<bool> struct static_assertion;
template<>     struct static_assertion<true> { };

#define CHECK_TYPE_SIZE( type, size ) \
	static static_assertion<sizeof(type) == size> type##_sizecheck

#define CHECK_FIELD_OFFSET( type, field, offset ) \
	static static_assertion<((uint32_t) &reinterpret_cast<type*>(0)->field == offset)> type##_##field##_offsetcheck


#endif



CHECK_TYPE_SIZE( EStorable,      0x04 );
CHECK_TYPE_SIZE( EInstance,      0x64 );
CHECK_TYPE_SIZE( EIGameInstance, 0xB4 );
CHECK_TYPE_SIZE( EICharacter,    0x2F0 );
CHECK_TYPE_SIZE( EIPlayer,       0xB30 );
CHECK_TYPE_SIZE( EIHarry,        0x1A20 );
CHECK_TYPE_SIZE( EIStaticModel,  0x1E0 );
CHECK_TYPE_SIZE( EIDynamicModel, 0x1E0 );
CHECK_TYPE_SIZE( EIProjectile,   0x280 );

CHECK_FIELD_OFFSET( EIHarry,       m_breakdance,     0x1914 );
CHECK_FIELD_OFFSET( EIStaticModel, m_worldTransform, 0xC0 );
