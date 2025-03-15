#include <Windows.h>

#include "ptle/types/types.h"
#include "ptle/levels/level_info.h"

#include "injector/injector.hpp"
#include "utils/log.h"
#include "utils/func.h"

#include "rando/rando.h"



void ensure_item_swap()
{
	*((bool*) 0x9106A2) = false;
}


GET_METHOD( 0x5EBA90, void, ScheduleWorldLoad, void*, uint32_t, bool );

void hijack_transition( void* globalStruct, uint32_t levelCRC, bool p2 )
{
	extern uint32_t rando_prev_area;

	ensure_item_swap();

	uint32_t* currentAreaCRC = ((uint32_t*) 0x917088);
	uint32_t target_area = level_get_by_crc( levelCRC );

	rando_prev_area = level_get_by_crc( *currentAreaCRC );

	Transition original = { rando_prev_area, target_area };
	log_printf( "Game wants to go from \"%s\" (0x%X) to \"%s\" (0x%X).\n",
		level_get_name(original.areaFromID), level_get_crc(original.areaFromID),
		level_get_name(original.areaToID), level_get_crc(original.areaToID) );

	// Where the actual hijacking takes place.
	Transition r;
	auto it = rando_transitions_map.find( original );
	if ( it == rando_transitions_map.end() ) {
		r = original;
		log_printf( "Transition is ignored. We let the game proceed like normal.\n" );
	}
	else {
		r = it->second;
		log_printf( "Redirecting to \"%s\" (0x%X).\n", level_get_name(r.areaToID), level_get_crc(r.areaToID) );
	}

	uint32_t crc = level_get_crc( r.areaToID );
	ScheduleWorldLoad( globalStruct, crc, p2 );
}
MAKE_THISCALL_WRAPPER( hijack_transition_ptr, hijack_transition );


void InitMod()
{
	log_printf( "PTLE Randomizer : Starting...\n" );

	rando_init();

	generate_randomized_map();

	randomize_shaman_shop();

	// TODO : Load world call (new game).

	// Load world call (level transition).
	injector::MakeCALL( 0x5ECC70, hijack_transition_ptr );

	// Hijack new game starting area.
	extern uint32_t starting_area;
	uint32_t startCRC = level_get_crc( starting_area );
	injector::WriteMemory( 0x5EB9E6, startCRC );

	log_printf( "PTLE Randomizer : Mod started.\n" );
}



BOOL WINAPI DllMain( HINSTANCE hinstace, DWORD reason, LPVOID )
{
	switch ( reason )
	{
	case DLL_PROCESS_ATTACH:
		InitMod();
		break;
	default:
		break;
	}

	return true;
}
