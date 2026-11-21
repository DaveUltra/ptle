#include "types.h"

#include "ptle/EStorable.h"

#include "ptle/EIBats.h"
#include "ptle/EIBeast.h"
#include "ptle/EIBezierSpline.h"
#include "ptle/EIBreakable.h"
#include "ptle/EIBushNinja.h"
#include "ptle/EIButterfly.h"
#include "ptle/EICinematic.h"
#include "ptle/EICrocs.h"
#include "ptle/EIEffector.h"
#include "ptle/EIElectricEel.h"
#include "ptle/EIEvilHarryMM.h"
#include "ptle/EIEvilHarryP.h"
#include "ptle/EIEvilHarryS.h"
#include "ptle/EIExplorer.h"
#include "ptle/EIEyeballs.h"
#include "ptle/EIGamePlatform.h"
#include "ptle/EIGassregion.h"
#include "ptle/EIHarry.h"
#include "ptle/EIHavokBreakable.h"
#include "ptle/EIHavokCloth.h"
#include "ptle/EIHavokVine.h"
#include "ptle/EIHenchman.h"
#include "ptle/EIHowlerMonkey.h"
#include "ptle/EIMarker.h"
#include "ptle/EIMegaMonkeyChild.h"
#include "ptle/EIMegaMonkeyParent.h"
#include "ptle/EIMole.h"
#include "ptle/EINative.h"
#include "ptle/EINinjaBush.h"
#include "ptle/EINPCBeast.h"
#include "ptle/EIPenguinNest.h"
#include "ptle/EIPenguins.h"
#include "ptle/EIPiranha.h"
#include "ptle/EIPitMonster.h"
#include "ptle/EIPlant.h"
#include "ptle/EIPlantNox.h"
#include "ptle/EIPlayerMonkey.h"
#include "ptle/EIPlayerPenguin.h"
#include "ptle/EIPlayerScorpion.h"
#include "ptle/EIPlayerStart.h"
#include "ptle/EIPorcupine.h"
#include "ptle/EIPusca.h"
#include "ptle/EIPuscaMinion.h"
#include "ptle/EIQuicksandPatch.h"
#include "ptle/EIResetPlane.h"
#include "ptle/EIScorps.h"
#include "ptle/EIShadowSpot.h"
#include "ptle/EIShaman.h"
#include "ptle/EISnowBowler.h"
#include "ptle/EISnowScarab.h"
#include "ptle/EISpinja.h"
#include "ptle/EIStClair.h"
#include "ptle/EISupai.h"
#include "ptle/EITreasureIdol.h"
#include "ptle/EIUpdateRegion.h"
#include "ptle/EIUserTreasureIdol.h"
#include "ptle/EIWaterPatch.h"
#include "ptle/EIWell.h"
#include "ptle/EIYoYoMonkey.h"

#include "ptle/EResource.h"
#include "ptle/ERAnim.h"
#include "ptle/ERCharacter.h"
#include "ptle/ERDataset.h"
#include "ptle/ERFont.h"
#include "ptle/ERLevel.h"
#include "ptle/ERModel.h"
#include "ptle/ERParticleType.h"
#include "ptle/ERScript.h"
#include "ptle/ERShader.h"
#include "ptle/ERTexture.h"

#include "ptle/EScriptContext.h"
#include "ptle/EScriptData.h"
#include "ptle/ESDAnimation.h"
#include "ptle/ESDFloat.h"
#include "ptle/ESDInt.h"
#include "ptle/ESDModel.h"
#include "ptle/ESDMatrix.h"
#include "ptle/ESDParticleType.h"
#include "ptle/ESDPointer.h"
#include "ptle/ESDResource.h"
#include "ptle/ESDScript.h"
#include "ptle/ESDShader.h"
#include "ptle/ESDSound.h"
#include "ptle/ESDString.h"
#include "ptle/ESDVector.h"

#include "ptle/ESGVFloat.h"
#include "ptle/ESGVInt.h"

#include "ptle/EUITextIcon.h"


static std::map<uint32_t, type_info_t> g_types;


const type_info_t* get_object_type( EStorable* obj )
{
	uint32_t vtablePtr = *((uint32_t*) obj);
	return get_type_by_vtable( vtablePtr );
}

const type_info_t* get_type_by_vtable( uint32_t vtableAddr )
{
	auto it = g_types.find( vtableAddr );
	return (it != g_types.end()) ? &it->second : 0;
}

const std::map<uint32_t, type_info_t>& get_all_types()
{
	return g_types;
}

bool is_type_derived_from( const type_info_t* type, const type_info_t* base )
{
	const ETypeInfo* ptleType = type->ptleType;

	do {
		if ( ptleType == base->ptleType ) {
			return true;
		}
		ptleType = ptleType->m_parent;
	}
	while ( ptleType->m_parent != ptleType );

	return false;
}

bool is_type_derived_from( const ETypeInfo* type, const ETypeInfo* base )
{
	do {
		if ( type == base ) {
			return true;
		}
		type = type->m_parent;
	}
	while ( type && type->m_parent != type );

	return false;
}


static inline void register_type(uint32_t vtableAddr, uint32_t ptleTypeAddr)
{
	const type_info_t t = { (void*) vtableAddr, (const ETypeInfo*) ptleTypeAddr };
	g_types.insert( std::pair<uint32_t, type_info_t>(vtableAddr, t) );
}

template<class T>
static inline void register_type()
{
	uint32_t dummy = T::VTABLE;
	const type_info_t t = { (void*) T::VTABLE, ((const EStorable*) &dummy)->GetType() };
	g_types.insert( std::pair<uint32_t, type_info_t>(dummy, t) );
}

void register_types()
{
	register_type<EStorable>();
	register_type<EInstance>();
	register_type<EIGameInstance>();

	register_type<EIAnimLod>();
	register_type<EIBats>();
	register_type<EIBeast>();
	register_type<EIBezierSpline>();
	register_type<EIBreakable>();
	register_type<EIBushNinja>();
	register_type<EIButterfly>();
	register_type<EICharacter>();
	register_type<EICinematic>();
	register_type<EICrocs>();
	register_type<EIEffector>();
	register_type<EIElectricEel>();
	register_type<EIEvilHarryMM>();
	register_type<EIEvilHarryP>();
	register_type<EIEvilHarryS>();
	register_type<EIExplorer>();
	register_type<EIEyeballs>();
	register_type<EIFlockBeast>();
	register_type<EIGamePlatform>();
	register_type<EIGassregion>();
	register_type<EIHarry>();
	register_type<EIHavokBreakable>();
	register_type<EIHavokCloth>();
	register_type<EIHavokVine>();
	register_type<EIHenchman>();
	register_type<EIHowlerMonkey>();
	register_type<EIMarker>();
	register_type<EIMegaMonkeyChild>();
	register_type<EIMegaMonkeyParent>();
	register_type<EIMole>();
	register_type<EINative>();
	register_type<EINinjaBush>();
	register_type<EINPCBeast>();
	register_type<EIPenguinNest>();
	register_type<EIPenguins>();
	register_type<EIPiranha>();
	register_type<EIPitMonster>();
	register_type<EIPlant>();
	register_type<EIPlantNox>();
	register_type<EIPlatform>();
	register_type<EIPlayer>();
	register_type<EIPlayerBeast>();
	register_type<EIPlayerMonkey>();
	register_type<EIPlayerPenguin>();
	register_type<EIPlayerScorpion>();
	register_type<EIPlayerStart>();
	register_type<EIPorcupine>();
	register_type<EIProjectile>();
	register_type<EIPusca>();
	register_type<EIPuscaMinion>();
	register_type<EIQuicksandPatch>();
	register_type<EIResetPlane>();
	register_type<EIScorps>();
	register_type<EIShadowSpot>();
	register_type<EIShaman>();
	register_type<EISnowBowler>();
	register_type<EISnowScarab>();
	register_type<EISpinja>();
	register_type<EIStaticModel>();
	register_type<EIStClair>();
	register_type<EISupai>();
	register_type<EITreasureIdol>();
	register_type<EIUpdateRegion>();
	register_type<EIUserTreasureIdol>();
	register_type<EIWavePatch>();
	register_type<EIWaterPatch>();
	register_type<EIWell>();
	register_type<EIYoYoMonkey>();

	register_type<EResource>();
	register_type<ERAnim>();
	register_type<ERCharacter>();
	register_type<ERFont>();
	register_type<ERLevel>();
	register_type<ERModel>();
	register_type<ERScript>();
	register_type<ERShader>();
	register_type<ERTexture>();

	register_type<EScriptContext>();
	register_type<EScriptData>();
	register_type<ESDAnimation>();
	register_type<ESDFloat>();
	register_type<ESDInt>();
	register_type<ESDResource>();
	register_type<ESDVector>();
	register_type<ESDMatrix>();
	register_type<ESDModel>();
	register_type<ESDParticleType>();
	register_type<ESDPointer>();
	register_type<ESDScript>();
	register_type<ESDShader>();
	register_type<ESDSound>();
	register_type<ESDString>();

	register_type<ESaveGameVar>();
	register_type<ESGVFloat>();
	register_type<ESGVInt>();

	register_type<EUIObjectNode>();
	register_type<EUIIcon>();
	register_type<EUITextIcon>();
	register_type( 0x89DED0, 0x00912970 );   // EUIDDMenuTextItem
	register_type( 0x89F608, 0x00912BB8 );   // EUIHudCube
	register_type( 0x89F840, 0x00912BE4 );   // EUIHudInventory
	register_type( 0x8A04D8, 0x00912C6C );   // EUIHudLifeMeter
	register_type( 0x87A3D8, 0x0090F9C8 );   // EGamesUI3dScore
	register_type( 0x87A750, 0x0090F9F4 );   // EGamesUI3dTimer

	register_type( 0x8B0FD4, 0x009413C8 );   // EBoundTreeNode
}
