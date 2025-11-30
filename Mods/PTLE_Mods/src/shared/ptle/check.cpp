#include <stdint.h>

#include "EIBushNinja.h"
#include "EICameraBossPusca.h"
#include "EICameraLook.h"
#include "EIEffector.h"
#include "EIEvilHarryMM.h"
#include "EIEvilHarryP.h"
#include "EIExplorer.h"
#include "EIHarry.h"
#include "EIHenchman.h"
#include "EIHowlerMonkey.h"
#include "EIMegaMonkeyChild.h"
#include "EIMegaMonkeyParent.h"
#include "EINative.h"
#include "EINPCBeast.h"
#include "EIPlant.h"
#include "EIPlayerMonkey.h"
#include "EIPlayerPenguin.h"
#include "EIPlayerScorpion.h"
#include "EIPlayerStart.h"
#include "EIPointLight.h"
#include "EIProjectile.h"
#include "EIPusca.h"
#include "EIPuscaMinion.h"
#include "EIShaman.h"
#include "EISnowBowler.h"
#include "EISnowScarab.h"
#include "EIStClair.h"
#include "EISupai.h"
#include "EITreasureIdol.h"
#include "ERAnim.h"
#include "ERCharacter.h"
#include "ERDataset.h"
#include "ERFont.h"
#include "ERLevel.h"
#include "ERModel.h"
#include "ERParticleType.h"
#include "ERScript.h"
#include "ERShader.h"
#include "EScriptContext.h"
#include "EUITextIcon.h"



// Use static_assert() in C++11 and above.
#if __cplusplus >= 201103L || _MSC_VER >= 1700


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



CHECK_TYPE_SIZE( EStorable,          0x04 );
CHECK_TYPE_SIZE( EInstance,          0x64 );
CHECK_TYPE_SIZE( EILight,            0x7C );
CHECK_TYPE_SIZE( EIPointLight,       0x94 );
CHECK_TYPE_SIZE( EIGameInstance,     0xB4 );
CHECK_TYPE_SIZE( EICamera,           0x130 );
CHECK_TYPE_SIZE( EICameraPit,        0x140 );
CHECK_TYPE_SIZE( EICameraLook,       0x1A0 );
CHECK_TYPE_SIZE( EICameraFollow,     0x7D0 );
CHECK_TYPE_SIZE( EICameraBossPusca,  0x800 );
CHECK_TYPE_SIZE( EICharacter,        0x2F0 );
CHECK_TYPE_SIZE( EIPlayer,           0xB30 );
CHECK_TYPE_SIZE( EIPlayerBeast,      0xB50 );
CHECK_TYPE_SIZE( EIPlayerPenguin,    0xBD0 );
CHECK_TYPE_SIZE( EIPlayerMonkey,     0xC00 );
CHECK_TYPE_SIZE( EIPlayerScorpion,   0xDC0 );
CHECK_TYPE_SIZE( EIHarry,            0x1A20 );
CHECK_TYPE_SIZE( EIStaticModel,      0x1E0 );
CHECK_TYPE_SIZE( EIDynamicModel,     0x1E0 );
CHECK_TYPE_SIZE( EITreasureIdol,     0x210 );
CHECK_TYPE_SIZE( EIEffector,         0x240 );
CHECK_TYPE_SIZE( EIPlayerStart,      0x240 );
CHECK_TYPE_SIZE( EIPlant,            0x260 );
CHECK_TYPE_SIZE( EIProjectile,       0x280 );
CHECK_TYPE_SIZE( EIBeast,            0x6F0 );
CHECK_TYPE_SIZE( EIShaman,           0x700 );
CHECK_TYPE_SIZE( EISnowBowler,       0x720 );
CHECK_TYPE_SIZE( EINPCBeast,         0x730 );
CHECK_TYPE_SIZE( EIPuscaMinion,      0x730 );
CHECK_TYPE_SIZE( EINative,           0x740 );
CHECK_TYPE_SIZE( EIMegaMonkeyChild,  0x760 );
CHECK_TYPE_SIZE( EIExplorer,         0x770 );
CHECK_TYPE_SIZE( EIHowlerMonkey,     0x790 );
CHECK_TYPE_SIZE( EIMegaMonkeyParent, 0x7C0 );
CHECK_TYPE_SIZE( EISnowScarab,       0x7D0 );
CHECK_TYPE_SIZE( EIHenchman,         0x7E0 );
CHECK_TYPE_SIZE( EIPusca,            0x7E0 );
CHECK_TYPE_SIZE( EIStClair,          0x7F0 );
CHECK_TYPE_SIZE( EIBushNinja,        0x850 );
CHECK_TYPE_SIZE( EIEvilHarryMM,      0x8B0 );
CHECK_TYPE_SIZE( EIEvilHarryP,       0x970 );
CHECK_TYPE_SIZE( EISupai,            0xA00 );
CHECK_TYPE_SIZE( EResource,          0x14 );
CHECK_TYPE_SIZE( ERDataset,          0x1C );
CHECK_TYPE_SIZE( ERShader,           0x20 );
CHECK_TYPE_SIZE( ERCharacter,        0x34 );
CHECK_TYPE_SIZE( ERScript,           0x44 );
CHECK_TYPE_SIZE( ERFont,             0x70 );
CHECK_TYPE_SIZE( ERAnim,             0x7C );
CHECK_TYPE_SIZE( ERModel,            0x98 );
CHECK_TYPE_SIZE( ERParticleType,     0x168 );
CHECK_TYPE_SIZE( ERLevel,            0x644 );
CHECK_TYPE_SIZE( EScriptContext,     0x890 );
CHECK_TYPE_SIZE( EUIObjectNode,      0xB0 );
CHECK_TYPE_SIZE( EUIIcon,            0xE4 );
CHECK_TYPE_SIZE( EUITextIcon,        0x110 );


CHECK_FIELD_OFFSET( EIHarry,        m_breakdance,        0x1914 );
CHECK_FIELD_OFFSET( EIBeast,        m_beastType,         0x54C );
CHECK_FIELD_OFFSET( EIStaticModel,  m_worldTransform,    0xC0 );
CHECK_FIELD_OFFSET( EIStaticModel,  m_modelResource,     0x15C );

CHECK_FIELD_OFFSET( EICharacter,    m_transformMatrix,   0x160 );

CHECK_FIELD_OFFSET( EIPlayer,       m_rigidBodyMatrix,   0x840 );

CHECK_FIELD_OFFSET( EISupai,        m_isTutorialSupai,   0x991 );

CHECK_FIELD_OFFSET( EIEffector,     m_scriptCRC0,        0x200 );

CHECK_FIELD_OFFSET( ERLevel,        m_updateRegion,      0x440 );
CHECK_FIELD_OFFSET( ERLevel,        m_entitiesByID,      0x59C );

CHECK_FIELD_OFFSET( EScriptContext, m_scriptStack,       0x24 );
CHECK_FIELD_OFFSET( EScriptContext, m_currentFunction,   0x884 );
