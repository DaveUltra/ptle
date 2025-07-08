#include <Windows.h>

#include "command_palette.h"
#include "utils/log.h"
#include "injector/injector.hpp"

#include "ptle/types/types.h"


#include "ptle/EIHarry.h"
#include "utils/func.h"
GET_METHOD( 0x547180, void, EIPlayer_Tick, EIPlayer* );
void _EIPlayerTick_custom( EIHarry* harry )
{
	EIPlayer_Tick( harry );

	extern bool g_invulnerable;
	if ( g_invulnerable ) {
		bool* inv = (bool*) (0x917059);
		*inv = true;
	}
	extern bool g_infiniteJumps;
	if ( g_infiniteJumps ) {
		harry->m_onGround = true;
	}
}
MAKE_THISCALL_WRAPPER( EIPlayerTick_custom, _EIPlayerTick_custom );



unsigned long __stdcall Thread( void* )
{
	command_create_window();
	command_process_window();

	ExitProcess( 0 );
}


#include "gizmod/GizmodPlugin.h"

class CheatLabPlugin : public GizmodPlugin
{
public:

	virtual const char* getDisplayName() const { return "CheatLab"; }
	virtual const char* getSystemName() const { return "CheatLab"; }

	virtual void onEnable()
	{
		injector::MakeCALL( 0x4B2DE1, EIPlayerTick_custom );

		register_types();

		CreateThread( 0, 0, Thread, 0, 0, 0 );
	}
};

DECLARE_PLUGIN( CheatLabPlugin );
