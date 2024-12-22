#include "types.h"

#include "ptle/EStorable.h"

#include <map>


static std::map<uint32_t, type_info_t> g_types;


const type_info_t* get_type_by_vtable( uint32_t vtableAddr )
{
	auto it = g_types.find( vtableAddr );
	return (it != g_types.end()) ? &it->second : 0;
}


static inline void register_type( uint32_t vtableAddr, uint32_t ptleTypeAddr )
{
	type_info_t t = { (void*) vtableAddr, (const ETypeInfo*) ptleTypeAddr };
	g_types.insert( std::pair<uint32_t, type_info_t>(vtableAddr, t) );
}

void register_types()
{
	register_type( 0x0086B3C8, 0x00090D2FC );   // EIBats
	register_type( 0x0086CE50, 0x00090D888 );   // EIBreakable
	register_type( 0x0086D1D8, 0x00090D8C0 );   // EIBushNinja
	register_type( 0x0086D7A0, 0x00090D92C );   // EIButterfly
	register_type( 0x0086D7A0, 0x00090DA34 );   // EICinematic
	register_type( 0x0086FD18, 0x00090DAC0 );   // EICrocs
	register_type( 0x00873B68, 0x00090F0D0 );   // EIElectricEel
	register_type( 0x00876400, 0x00090F3E8 );   // EIExplorer
	register_type( 0x0087D300, 0x00090FBE8 );   // EIHarry
	register_type( 0x00881BE8, 0x00090FEB4 );   // EIHenchman
	register_type( 0x008822D0, 0x000910014 );   // EIHowlerMonkey
	register_type( 0x00889700, 0x000910700 );   // EIMegaMonkeyChild
	register_type( 0x0088A2C0, 0x000910774 );   // EIMegaMonkeyParent
	register_type( 0x0088A668, 0x0009107E0 );   // EIMole
	register_type( 0x0088AFE8, 0x0009108A0 );   // EINative
	register_type( 0x0088B4F0, 0x000910938 );   // EINinjaBush
	register_type( 0x0088E518, 0x000910A7C );   // EIPenguinNest
	register_type( 0x0088E8C8, 0x000910AA8 );   // EIPenguins
	register_type( 0x0088F078, 0x000910B78 );   // EIPiranha
	register_type( 0x00890400, 0x000910C20 );   // EIPitMonster
	register_type( 0x00890840, 0x000910C8C );   // EIPlant
	register_type( 0x00890A40, 0x000910CC4 );   // EIPlantNox
	register_type( 0x00891358, 0x000910E7C );   // EIPlayerMonkey
	register_type( 0x008916D8, 0x000910EA8 );   // EIPlayerPenguin
	register_type( 0x00891A78, 0x000910ED4 );   // EIPlayerScorpion
	register_type( 0x008925A8, 0x00091104C );   // EIPorcupine
	register_type( 0x00893210, 0x00091156C );   // EIProjectile
	register_type( 0x00893EA8, 0x000911648 );   // EIPusca
	register_type( 0x00894398, 0x000911820 );   // EIPuscaMinion
	register_type( 0x00896438, 0x000911CE8 );   // EIScorps
	register_type( 0x00896C10, 0x000911F2C );   // EIShaman
	register_type( 0x00897ED0, 0x000912184 );   // EISnowBowler
	register_type( 0x00898C20, 0x000912298 );   // EISpinja
	register_type( 0x00899130, 0x00091233C );   // EIStClair
	register_type( 0x008999C8, 0x0009123AC );   // EISupai
	register_type( 0x0089A2F8, 0x0009124F0 );   // EITreasureIdol
	register_type( 0x008A2148, 0x00091FED4 );   // EIWaterPatch
	register_type( 0x008A2580, 0x000916E98 );   // EIWell
}
