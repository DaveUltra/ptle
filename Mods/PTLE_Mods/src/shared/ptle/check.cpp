#include <stdint.h>

#include "EIExplorer.h"
#include "EIHarry.h"
#include "EINative.h"
#include "EINPCBeast.h"
#include "EIPointLight.h"
#include "EIProjectile.h"
#include "EIShaman.h"
#include "EITreasureIdol.h"
#include "ERAnim.h"
#include "ERCharacter.h"
#include "ERFont.h"
#include "ERLevel.h"
#include "ERModel.h"
#include "ERParticleType.h"
#include "ERScript.h"
#include "ERShader.h"


// Use static_assert() in C++11 and above.
#if __cplusplus <= 201103L


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
CHECK_TYPE_SIZE( EILight,        0x7C );
CHECK_TYPE_SIZE( EIPointLight,   0x94 );
CHECK_TYPE_SIZE( EIGameInstance, 0xB4 );
CHECK_TYPE_SIZE( EICharacter,    0x2F0 );
CHECK_TYPE_SIZE( EIPlayer,       0xB30 );
CHECK_TYPE_SIZE( EIHarry,        0x1A20 );
CHECK_TYPE_SIZE( EIStaticModel,  0x1E0 );
CHECK_TYPE_SIZE( EIDynamicModel, 0x1E0 );
CHECK_TYPE_SIZE( EITreasureIdol, 0x210 );
CHECK_TYPE_SIZE( EIProjectile,   0x280 );
CHECK_TYPE_SIZE( EIBeast,        0x6F0 );
CHECK_TYPE_SIZE( EIShaman,       0x700 );
CHECK_TYPE_SIZE( EINPCBeast,     0x730 );
CHECK_TYPE_SIZE( EINative,       0x740 );
CHECK_TYPE_SIZE( EIExplorer,     0x770 );
CHECK_TYPE_SIZE( EResource,      0x14 );
CHECK_TYPE_SIZE( ERShader,       0x20 );
CHECK_TYPE_SIZE( ERCharacter,    0x34 );
CHECK_TYPE_SIZE( ERScript,       0x44 );
CHECK_TYPE_SIZE( ERFont,         0x70 );
CHECK_TYPE_SIZE( ERAnim,         0x7C );
CHECK_TYPE_SIZE( ERModel,        0x98 );
CHECK_TYPE_SIZE( ERParticleType, 0x168 );
CHECK_TYPE_SIZE( ERLevel,        0x644 );

CHECK_FIELD_OFFSET( EIHarry,       m_breakdance,      0x1914 );
CHECK_FIELD_OFFSET( EIBeast,       m_beastType,       0x54C );
CHECK_FIELD_OFFSET( EIStaticModel, m_worldTransform,  0xC0 );

CHECK_FIELD_OFFSET( EICharacter,   m_transformMatrix, 0x160 );

CHECK_FIELD_OFFSET( ERLevel,       m_updateRegion,    0x440 );
