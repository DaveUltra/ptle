#include "types.h"

#include "ptle/EStorable.h"

#include <map>


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

bool is_type_derived_from( const type_info_t* type, const type_info_t* base )
{
	const ETypeInfo* ptleType = type->ptleType;

	do {
		if ( ptleType == base->ptleType ) {
			return true;
		}
	}
	while ( ptleType->m_parent != ptleType );

	return false;
}


static inline void register_type( uint32_t vtableAddr, uint32_t ptleTypeAddr )
{
	type_info_t t = { (void*) vtableAddr, (const ETypeInfo*) ptleTypeAddr };
	g_types.insert( std::pair<uint32_t, type_info_t>(vtableAddr, t) );
}

void register_types()
{
	register_type( 0x86B3C8, 0x0090D2FC );   // EIBats
	register_type( 0x86C3D0, 0x0090D36C );   // EIBeast
	register_type( 0x86CE50, 0x0090D888 );   // EIBreakable
	register_type( 0x86D1D8, 0x0090D8C0 );   // EIBushNinja
	register_type( 0x86D7A0, 0x0090D92C );   // EIButterfly
	register_type( 0x86DB20, 0x009410E4 );   // EStorable
	register_type( 0x86E660, 0x0090DA34 );   // EICinematic
	register_type( 0x86FD18, 0x0090DAC0 );   // EICrocs
	register_type( 0x873B68, 0x0090F0D0 );   // EIElectricEel
	register_type( 0x874688, 0x0090F13C );   // EIEvilHarryMM
	register_type( 0x875240, 0x0090F1A8 );   // EIEvilHarryP
	register_type( 0x875B30, 0x0090F218 );   // EIEvilHarryS
	register_type( 0x876400, 0x0090F3E8 );   // EIExplorer
	register_type( 0x8779C0, 0x0090F5D0 );   // EIGamePlatform
	register_type( 0x87D300, 0x0090FBE8 );   // EIHarry
	register_type( 0x87D618, 0x00910D30 );   // EIPlayer
	register_type( 0x881BE8, 0x0090FEB4 );   // EIHenchman
	register_type( 0x8822D0, 0x00910014 );   // EIHowlerMonkey
	register_type( 0x889700, 0x00910700 );   // EIMegaMonkeyChild
	register_type( 0x88A2C0, 0x00910774 );   // EIMegaMonkeyParent
	register_type( 0x88A668, 0x009107E0 );   // EIMole
	register_type( 0x88AFE8, 0x009108A0 );   // EINative
	register_type( 0x88B4F0, 0x00910938 );   // EINinjaBush
	register_type( 0x88B868, 0x009109A4 );   // EINPCBeast
	register_type( 0x88E518, 0x00910A7C );   // EIPenguinNest
	register_type( 0x88E8C8, 0x00910AA8 );   // EIPenguins
	register_type( 0x88F078, 0x00910B78 );   // EIPiranha
	register_type( 0x890400, 0x00910C20 );   // EIPitMonster
	register_type( 0x890840, 0x00910C8C );   // EIPlant
	register_type( 0x890A40, 0x00910CC4 );   // EIPlantNox
	register_type( 0x891358, 0x00910E7C );   // EIPlayerMonkey
	register_type( 0x8916D8, 0x00910EA8 );   // EIPlayerPenguin
	register_type( 0x891A78, 0x00910ED4 );   // EIPlayerScorpion
	register_type( 0x8925A8, 0x0091104C );   // EIPorcupine
	register_type( 0x893210, 0x0091156C );   // EIProjectile
	register_type( 0x893EA8, 0x00911648 );   // EIPusca
	register_type( 0x894398, 0x00911820 );   // EIPuscaMinion
	register_type( 0x894A48, 0x009118BC );   // EIResetPlane
	register_type( 0x896438, 0x00911CE8 );   // EIScorps
	register_type( 0x896C10, 0x00911F2C );   // EIShaman
	register_type( 0x897ED0, 0x00912184 );   // EISnowBowler
	register_type( 0x898C20, 0x00912298 );   // EISpinja
	register_type( 0x899130, 0x0091233C );   // EIStClair
	register_type( 0x8999C8, 0x009123AC );   // EISupai
	register_type( 0x89A2F8, 0x009124F0 );   // EITreasureIdol
	register_type( 0x8A2148, 0x0091FED4 );   // EIWaterPatch
	register_type( 0x8A2580, 0x00916E98 );   // EIWell
	register_type( 0x8A2C70, 0x009170DC );   // EIYoYoMonkey
	register_type( 0x8A5000, 0x009176CC );   // EICharacter
	register_type( 0x8A5198, 0x00917730 );   // EIGameInstance
	register_type( 0x8A52C0, 0x0091775C );   // EInstance
	register_type( 0x8A5710, 0x00917914 );   // EResource
	register_type( 0x8A5B4C, 0x0091AA10 );   // ERModel
	register_type( 0x8A5D08, 0x0091EA48 );   // ERLevel
	register_type( 0x8A5D64, 0x0091EA80 );   // ERShader
	register_type( 0x8A9358, 0x0092005C );   // ESaveGameVar
	register_type( 0x8A9398, 0x00920030 );   // ESGVInt
	register_type( 0x8A93DC, 0x00920008 );   // ESGVFloat
	register_type( 0x8ADEB8, 0x0092801C );   // EIUpdateRegion
	register_type( 0x8AE110, 0x0093B9EC );   // ERAnim
	register_type( 0x8B02C4, 0x0093EB70 );   // ERTexture
	register_type( 0x8B0614, 0x0093ED30 );   // ERScript
	register_type( 0x8B0FD4, 0x009413C8 );   // EBoundTreeNode
}
