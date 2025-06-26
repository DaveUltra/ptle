/*
 * UI menu builder interceptor.
 *
 * The custom code below is executed straight after the creation of a
 * EUIDDMenu instance, and allows customizing items before they are
 * shown to the player.
 *
 * - On main menu ("FrontEndMain"), the NEW GAME option is replaced
 *   with a custom text and recolored.
 */

#include "injector/injector.hpp"

#include "utils/func.h"
#include "utils/log.h"

#include "ptle/EUITextIcon.h"
#include "ptle/types/types.h"
#include "ptle/math/Vector4f.h"



//
// Custom method with call to original.
//

GET_METHOD( 0x5A6FA0, bool, EUIDDMenu_Build, EUIObjectNode*, char*, int );

static wchar_t* _rando_new_game = L"PLAY RANDO";

bool _EUIDDMenu_Build_Custom( EUIObjectNode* self, char* uiType, int b )
{
	// Actually create the menu before we do stuff.
	EUIDDMenu_Build( self, uiType, b );

	// Patch "NEW GAME" with a custom display.
	if ( strcmp(uiType, "FrontEndMain") == 0 ) {
		for ( LinkedListElem* e = self->m_children.m_iterateFirst; e != 0; e = e->m_ptr1 ) {
			EUIObjectNode* c = (EUIObjectNode*) e->m_data;
			EUITextIcon* text = type_cast<EUITextIcon>( c, get_type_by_vtable(0x8A64B0) );

			// Lmao no string compare for you.
			if ( text && (*text->m_text)[0] == L'N' ) {
				text->m_text = &_rando_new_game;
				text->m_colorIndex = 3;   // Reuse yellow from the save UI.

				//Vector4f* colorTable = (Vector4f*) 0x91ED80;
				//colorTable[0] = colorTable[3];

				break;
			}
		}
	}

	return true;
}

MAKE_THISCALL_WRAPPER( EUIDDMenu_Build_Custom, _EUIDDMenu_Build_Custom );


void init_menu_patcher()
{
	// Replace the UI building method in EUIDDMenu's vtable.
	injector::WriteMemory( 0x89D254, &EUIDDMenu_Build_Custom );
}
