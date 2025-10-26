#include "command_palette.h"

#include "gizmod/Gizmod.h"

#include "injector/injector.hpp"

#include "ptle/ERLevel.h"
#include "ptle/EIHarry.h"
#include "ptle/EIProjectile.h"
#include "ptle/EIEvilHarryP.h"
#include "ptle/EITreasureIdol.h"
#include "ptle/EINative.h"
#include "ptle/EINPCBeast.h"
#include "ptle/math/Vector3f.h"
#include "ptle/math/Vector4f.h"

#include "ptle/levels/level_info.h"
#include "ptle/types/types.h"

#include "ptle/EBeastStateMachine.h"

#include "ptle/containers/TreeMap/TreeMap.h"

#include "utils/log.h"
#include "utils/func.h"


GET_METHOD( 0x626670, void, AddToWorld, ERLevel*, EInstance* );
GET_METHOD( 0x6265A0, void, AddToWorldFancy, ERLevel*, EInstance*, EInstance*, bool );
GET_METHOD( 0x6268D0, void, ERLevel_SetupEntity, ERLevel*, EInstance*, EInstance* );
GET_METHOD( 0x597400, void, InitIdol, EITreasureIdol*, Vector3f*, Vector4f*, uint32_t, uint32_t, uint32_t );
GET_METHOD( 0x547160, void, SetSomethingForHarry, EIHarry*, void* );
GET_METHOD( 0x4AD5E0, void, HarryInit, EIHarry*, Vector3f*, float );


GET_METHOD( 0x527ED0, void, EINPCBeast_Init, EINPCBeast* );

GET_FUNC( 0x6F8EC0, uint32_t, HashPath, char* );

GET_METHOD( 0x412850, void, EIBeast_LoadBeastAsset, EIBeast* );
GET_METHOD( 0x527E50, int,  EIBeast_Deserialize2, EIBeast* );


GET_METHOD( 0x5EBA90, void, ScheduleWorldLoad, void*, uint32_t, bool );

GET_FUNC( 0x557840, EIProjectile*, EIProjectile_Create, uint32_t );
GET_METHOD( 0x559640, void, EIProjectile_Spawn, EIProjectile*, Vector3f*, Vector3f*, EInstance*, float );



static void log_state_machine_info( EBeastStateMachine* sm )
{
	if ( !sm->m_states ) return;

	log_printf( "\"%s\"\n", sm->m_beastTypeName );
	log_printf( "%d states :\n", sm->m_numStates );
	for ( int i = 0; i < sm->m_numStates; i++ ) {
		log_printf( "- %d : \"%s\"\n", i, sm->m_states[i].m_name );
	}

	log_printf( "%d conditions :\n", sm->m_numConditions );
	for ( int i = 0; i < sm->m_numConditions; i++ ) {
		log_printf( "- %d : \"%s\" (0x%.6X)\n", i, sm->m_conditions[i].m_name, sm->m_conditions[i].m_func.m_func );
	}

	log_printf( "%d transitions :\n", sm->m_numTransitions );
	for ( int i = 0; i < sm->m_numTransitions; i++ ) {
		EBeastTransition& t = sm->m_transitions[i];
		log_printf( "- %d : %d -> %d (%d)\n", i, t.m_stateA->m_index, t.m_stateB->m_index, t.m_condition->m_index );
	}
}



// Stub functions for empty cheat slots.
static void _empty()
{
}

static void _empty_cheat( bool enable )
{
}



// --------------------------------------------------------------------------------
// Auto skip cutscenes (breaks things very slightly, but remains playable).
// --------------------------------------------------------------------------------
static void _auto_skip_cutscenes( bool enable )
{
	static char orig_42FE93[5];
	static float custom_skip_time = 0.1F;   // We need our own memory because the "two" used is referenced by other stuff, so changing it breaks everything.

	if ( enable ) {
		// Save original bytes.
		for (int i = 0; i < 5; i++) orig_42FE93[i] = ((char*) 0x42FE93)[i];

		injector::MakeJMP( 0x42FE93, 0x42FF45 );                // Jump straight to cutscene skip call when skip time is elapsed.
		injector::WriteMemory( 0x42FE84, &custom_skip_time );   // Make the code load OUR float.
		custom_skip_time = 0.1F;                                // Make cutscene skip time shorter (default = 2.0F = 0x40000000).
	}
	else {
		injector::WriteMemoryRaw( 0x42FE93, orig_42FE93, sizeof(orig_42FE93), true );
		custom_skip_time = 2.0F;
	}
}


// --------------------------------------------------------------------------------
// Effectors debug drawing. This is a flag left in the game, showing trigger
// volumes for level transitions, cutscenes, etc...
// --------------------------------------------------------------------------------
static void _effector_debug_draw( bool enable )
{
	bool* effectorDebugDraw = (bool*) (0x920F3C);
	*effectorDebugDraw = enable;
}


// --------------------------------------------------------------------------------
// Invulnerability & Infinite jumps.
// TODO : invulnerable flag is actually a "in native games" flag, and also affects
//        which pause menu is shown.
// --------------------------------------------------------------------------------
bool g_invulnerable;
bool g_infiniteJumps;

static void _invulnerable( bool enable )
{
	g_invulnerable = enable;
	if ( !enable ) {
		bool* inv = (bool*) (0x917059);
		*inv = false;
	}
}

static void _infinite_jump( bool enable )
{
	g_infiniteJumps = enable;
}


// --------------------------------------------------------------------------------
// TNT rain.
// --------------------------------------------------------------------------------
static EIProjectile* _do_tnt_rain( uint32_t projectileType )
{
	Vector3f pos, dir;
	EIHarry* harry = injector::ReadMemory<EIHarry*>( 0x917034 );

	for ( int i = -12; i <= 12; i++ ) {
		for ( int j = -12; j <= 12; j++ ) {
			dir.x = dir.y = dir.z = 0.0F;

			pos.z = 70.0F;
			pos.x = j * 16.0F;
			pos.y = i * 16.0F;

			EIProjectile* tnt = EIProjectile_Create( 0xBBCAC492 );
			EIProjectile_Spawn( tnt, &pos, &dir, harry, 1.0F );
		}
	}

	return EIProjectile_Create( projectileType );
}

static void _tnt_rain( bool enable )
{
	static char orig[5];
	if ( enable ) {
		// Save original bytes.
		for (int i = 0; i < 5; i++) orig[i] = ((char*) 0x59540E)[i];

		injector::MakeCALL( 0x59540E, _do_tnt_rain );
	}
	else {
		injector::WriteMemoryRaw( 0x59540E, orig, sizeof(orig), true );
	}
}


// --------------------------------------------------------------------------------
// Every projectile is a TNT.
// --------------------------------------------------------------------------------
static void _all_tnt( bool enable )
{
	if ( enable ) {
		// mov DWORD PTR [ebp+0x8], BBCAC492
		uint8_t bytes[] = { 0xC7, 0x45, 0x08, 0x92, 0xC4, 0xCA, 0xBB };
		injector::WriteMemoryRaw( 0x55784E, bytes, sizeof(bytes), true );
	}
	else {
		injector::MakeNOP( 0x55784E, 7 );
	}
}


// --------------------------------------------------------------------------------
// Free camera mode.
// --------------------------------------------------------------------------------
float g_freeCameraPos[4] = { 0.0F, 0.0F, 10000.0F, 1.0F };
float g_freeCameraFOV = 1.0F;
#define FOV_MIN 2.0F
#define FOV_MAX 100.0F

GET_METHOD( 0x649F80, void, ECameraManager_SetFOV, void*, float );

static void Mat4_Ortho( Matrix4f* out, float width, float height, float n, float f )
{
	memset( out->data, 0, 16 * sizeof(float) );
	out->data[0][0] = 2.0F / width;
	out->data[1][1] = 2.0F / height;
	out->data[2][2] = 1.0F / (f - n);
	out->data[3][3] = 1.0F;
	//out->data[3][2] = n / (f - n);
}

static Matrix4f* __stdcall _Mat_LookDown( Matrix4f* self, Vector3f*, Vector3f*, Vector3f* )
{
	// Hardcoded keyboard controls.
	if ( GetAsyncKeyState('I') & 0x8000 ) g_freeCameraPos[1] += 1.0F;
	if ( GetAsyncKeyState('K') & 0x8000 ) g_freeCameraPos[1] -= 1.0F;
	if ( GetAsyncKeyState('L') & 0x8000 ) g_freeCameraPos[0] += 1.0F;
	if ( GetAsyncKeyState('J') & 0x8000 ) g_freeCameraPos[0] -= 1.0F;

	if ( GetAsyncKeyState('O') & 0x8000 ) g_freeCameraPos[2] += 100.0F;
	if ( GetAsyncKeyState('U') & 0x8000 ) g_freeCameraPos[2] -= 100.0F;

	if ( GetAsyncKeyState('Y') & 0x8000 ) { g_freeCameraFOV += 0.5F; if (g_freeCameraFOV > FOV_MAX) g_freeCameraFOV = FOV_MAX; }
	if ( GetAsyncKeyState('H') & 0x8000 ) { g_freeCameraFOV -= 0.5F; if (g_freeCameraFOV < FOV_MIN) g_freeCameraFOV = FOV_MIN; }

	// Making our own transform matrix.
	memset( self->data, 0, 16 * sizeof(float) );
	self->data[0][0] = 1.0F;
	self->data[1][1] = 1.0F;
	self->data[2][2] = 1.0F;
	memcpy( self->data[3], g_freeCameraPos, 4 * sizeof(float) );

	// Custom FOV.
	uint32_t camManager = *((uint32_t*) 0x90F0AC);
	ECameraManager_SetFOV( (void*) camManager, g_freeCameraFOV );
	*((float*)(camManager + 0x5d60)) = (g_freeCameraPos[2]) - 2000.0F;
	*((float*)(camManager + 0x5d64)) = (g_freeCameraPos[2]) + 200.0F;
	//0x634B1C : This calls the function that makes the perspective matrix.

	return self;
}
MAKE_THISCALL_WRAPPER( Mat_LookDown, _Mat_LookDown );

static Matrix4f* __stdcall _Mat_Ortho( Matrix4f* self, float, float, float, float )
{
	Mat4_Ortho( self, 160.0F, 120.0F, 1.0F, 2000.0F);

	return self;
}
MAKE_THISCALL_WRAPPER( Mat_Ortho, _Mat_Ortho );

static void _free_camera( bool enable )
{
	if ( enable ) {
		injector::MakeCALL( 0x449B9A, Mat_LookDown );
		//injector::MakeCALL( 0x634B1C, Mat_Ortho );
	}
	else {
		static uint8_t origCamera[5] = { 0xE8, 0xC1, 0x49, 0x2A, 0x00 };
		static uint8_t origPersp[5] = { 0xE8, 0x6F, 0x9E, 0x0B, 0x00 };
		injector::WriteMemoryRaw( 0x449B9A, origCamera, 5, true );
		//injector::WriteMemoryRaw( 0x634B1C, origPersp, 5, true );
	}
}


// --------------------------------------------------------------------------------
// Deactivate effectors.
// --------------------------------------------------------------------------------
static void _deactivate_effectors( bool enable )
{
	static char origBytes[5];
	if ( enable ) {
		memcpy( origBytes, (void*) 0x668943, 5 );
		injector::MakeJMP( 0x668943, 0x668BC4 );
	}
	else {
		injector::WriteMemoryRaw( 0x668943, origBytes, 5, true );
	}
}


// --------------------------------------------------------------------------------
// Give useful items, abilities, and max HP.
// --------------------------------------------------------------------------------
static void _give_stuff()
{
	EIHarry* harry = injector::ReadMemory<EIHarry*>( 0x917034 );
	if ( !harry || !harry->m_itemHotbar ) {
		log_printf( "Harry's not spawned yet.\n" );
		return;
	}

	// All abilities (expect dive & super sling).
	harry->m_risingStrike = 1;
	harry->m_heroicDash = 1;
	harry->m_smashStrike = 1;
	harry->m_breakdance = 1;

	// Max HP (10).
	harry->m_healthMax = 10;
	harry->m_healthValue = 10;

	// Hotbar.
	harry->m_itemHotbar[0] = injector::ReadMemory<void*>( 0x8EEC20 );  // Raft.
	harry->m_itemHotbar[1] = injector::ReadMemory<void*>( 0x8EEBCC );  // Pickaxes.
	harry->m_itemHotbar[2] = injector::ReadMemory<void*>( 0x8EEBB0 );  // Torch.
	harry->m_itemHotbar[3] = injector::ReadMemory<void*>( 0x8EEBE8 );  // TNT.

	// Unlock all items.
	ItemStruct* items = (ItemStruct*) 0x8EEB90;
	for ( int i = 0; i < 8; i++ ) {
		items[i].m_unlocked = 1;
	}

	// Item swap (true = no swap, false = swap enabled).
	bool* itemSwapPtr = (bool*) 0x9106A2;
	*itemSwapPtr = false;
}


// --------------------------------------------------------------------------------
// Max out Harry's HP (and set it to game maximum, 10).
// --------------------------------------------------------------------------------
static void _heal()
{
	EIHarry* harry = injector::ReadMemory<EIHarry*>( 0x917034 );
	if ( !harry ) {
		log_printf( "Harry's not spawned yet.\n" );
		return;
	}

	harry->m_healthValue = harry->m_healthMax;
}


// --------------------------------------------------------------------------------
// Remove all items, skills and upgrades, useful for testing conditions.
// --------------------------------------------------------------------------------
static void _remove_stuff()
{
	EIHarry* harry = injector::ReadMemory<EIHarry*>( 0x917034 );
	if ( !harry || !harry->m_itemHotbar ) {
		log_printf( "Harry's not spawned yet.\n" );
		return;
	}

	// All abilities.
	harry->m_risingStrike = 0;
	harry->m_superSling = 0;
	harry->m_heroicDash = 0;
	harry->m_heroicDive = 0;
	harry->m_smashStrike = 0;
	harry->m_breakdance = 0;

	// Health.
	harry->m_healthMax = 5;
	harry->m_healthValue = 5;

	// Clear hotbar contents.
	harry->m_itemHotbar[0] = 0;
	harry->m_itemHotbar[1] = 0;
	harry->m_itemHotbar[2] = 0;
	harry->m_itemHotbar[3] = 0;

	// Remove all items.
	ItemStruct* items = (ItemStruct*) 0x8EEB90;
	for ( int i = 0; i < 8; i++ ) {
		items[i].m_unlocked = 0;
	}

	// Item swap (true = no swap, false = swap enabled).
	bool* itemSwapPtr = (bool*) 0x9106A2;
	*itemSwapPtr = true;
}


// --------------------------------------------------------------------------------
// Kill Harry.
// --------------------------------------------------------------------------------
static void _die()
{
	EIHarry* harry = injector::ReadMemory<EIHarry*>( 0x917034 );
	if ( !harry ) return;

	harry->m_healthValue = 0;
}


// --------------------------------------------------------------------------------
// Spawn Evil Harry (penguin temple).
// --------------------------------------------------------------------------------
static bool no()
{
	return false;
}

static void _spawn_evilharry()
{
	EIHarry** harryPtr = ((EIHarry**) 0x917034);
	EIHarry* harry = *harryPtr;

	const type_info_t* type = get_type_by_vtable( 0x875240 );
	EIEvilHarryP* evilHarry = instantiate_object<EIEvilHarryP>( type );

	evilHarry->m_position.x = harry->m_position.x;
	evilHarry->m_position.y = harry->m_position.y + 15.0F;
	evilHarry->m_position.z = harry->m_position.z;

	evilHarry->m_rotation.x = 0.0F;
	evilHarry->m_rotation.y = 0.0F;
	evilHarry->m_rotation.z = 0.0F;
	evilHarry->m_rotation.w = 1.0F;

	uint32_t hash = HashPath( "BEvilHarryP" );
	evilHarry->m_beastTypeCRC = hash;
	evilHarry->m_beastTypeName = "evil harry";
	EIBeast_LoadBeastAsset( evilHarry );

	// (This overrides our state machine modifications sadly).
	evilHarry->DeserializePart2();

	// Patch the state machine.
	{
		EBeastStateMachine* evilHarryP = evilHarry->GetStateMachine();

		// Disable dancing on victory.
		EBeastState* stateChase = evilHarryP->m_states + 3;
		EBeastCondition* condNo = evilHarryP->m_conditions + 10;
		condNo->m_func.m_func = (void(*)())no;
		evilHarryP->m_transitions[1].m_condition = condNo;
		evilHarryP->m_transitions[5].m_condition = condNo;
		evilHarryP->m_transitions[56].m_condition = condNo;
		evilHarryP->m_transitions[60].m_condition = condNo;
		evilHarryP->m_transitions[64].m_condition = condNo;
	}

	AddToWorld( harry->m_world, evilHarry );

	// WARN : The free at 0x412A32 crashes, since it tries to free static memory.
	ERLevel_SetupEntity( harry->m_world, evilHarry, 0 );
	evilHarry->Init();

	// Force Evil Harry to chase us by default.
	evilHarry->m_forceNextState = true;
	evilHarry->m_nextState = 3;
}


// --------------------------------------------------------------------------------
// Harry spawner (unstable).
// --------------------------------------------------------------------------------
static EIProjectile* _harry_spawn( uint32_t code )
{
	EIHarry** harryPtr = ((EIHarry**) 0x917034);
	EIHarry* harry = *harryPtr;

	// Disable HUD updates (maybe?)
	injector::MakeRET( 0x4B0950 );

	// Don't create global items again.
	injector::WriteMemory<uint8_t>( 0x4AEA29, 0xEB );

	// No item hotbar.
	uint8_t bytes[10] = { 0xC7, 0x81, 0x3C, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	injector::MakeRangedNOP( 0x4AEA67, 0x4AEA7E );
	injector::WriteMemoryRaw( 0x4AEA67, bytes, 10, true );

	// Instantiate a new Harry.
	const type_info_t* type = get_type_by_vtable( 0x87D300 );
	EIHarry* newHarry = instantiate_object<EIHarry>( type );

	// Init.
	{
		//void* ptr = *((void**) 0x9170A4);
		//SetSomethingForHarry( newHarry, ptr );

		// /!\ This duplicates items (CRASH)
		Vector3f pos = { 0, 0, 0 };
		HarryInit( newHarry, &pos, 0.0F );

		newHarry->m_world = 0;
		AddToWorld( harry->m_world, newHarry );
		newHarry->m_world = harry->m_world;

		newHarry->m_itemHotbar = harry->m_itemHotbar;
		//*(((uint8_t*) newHarry) + 0x3e3) = false;
	}

	return EIProjectile_Create( code );
}


// --------------------------------------------------------------------------------
// Spawn a native.
// --------------------------------------------------------------------------------
static void _netiv_spawn()
{
	EIHarry** harryPtr = ((EIHarry**) 0x917034);
	EIHarry* harry = *harryPtr;

	const type_info_t* type = get_type_by_vtable( 0x88AFE8 );
	EINative* native = instantiate_object<EINative>( type );

	native->m_position.x = harry->m_position.x;
	native->m_position.y = harry->m_position.y;
	native->m_position.z = harry->m_position.z;

	native->m_rotation.x = 0.0F;
	native->m_rotation.y = 0.0F;
	native->m_rotation.z = 0.0F;
	native->m_rotation.w = 1.0F;

	uint32_t hash = HashPath( "BNative" );
	native->m_beastTypeCRC = hash;
	native->m_beastTypeName = "native";
	EIBeast_LoadBeastAsset( native );

	native->m_projectileType = 0x1B6BFEE7;
	EIBeast_Deserialize2( native );

	if ( !native->m_beastType ) {
		log_printf( "Beast type asset load failed.\n" );
	}

	AddToWorld( harry->m_world, native );

	// WARN : The free at 0x412A32 crashes, since it tries to free static memory.
	ERLevel_SetupEntity( harry->m_world, native, 0 );
	native->Init();
}


// --------------------------------------------------------------------------------
// Spawn Micay (or any NPC).
// --------------------------------------------------------------------------------
static void _micay_spawn()
{
	EIHarry** harryPtr = ((EIHarry**) 0x917034);
	EIHarry* harry = *harryPtr;

	const type_info_t* type = get_type_by_vtable( 0x88B868 );
	EINPCBeast* native = instantiate_object<EINPCBeast>( type );

	native->m_position.x = harry->m_position.x;
	native->m_position.y = harry->m_position.y;
	native->m_position.z = harry->m_position.z;

	native->m_rotation.x = 0.0F;
	native->m_rotation.y = 0.0F;
	native->m_rotation.z = 0.0F;
	native->m_rotation.w = 1.0F;

	//"micay", "bernard", "chief", "leech", "nicoleWchute", "nicole"
	native->m_beastTypeName = "micay";
	native->m_beastTypeCRC = 0x8252A12E;
	EIBeast_LoadBeastAsset( native );

	EIBeast_Deserialize2( native );

	if ( native->m_beastType ) {
		log_printf( "Beast type asset load failed.\n" );
	}

	ERLevel_SetupEntity( harry->m_world, native, 0 );
	EINPCBeast_Init( native );

	AddToWorld( harry->m_world, native );
}


// --------------------------------------------------------------------------------
// Idol spawner.
// --------------------------------------------------------------------------------
static void _idol_spawn()
{
	const type_info_t* type = get_type_by_vtable( 0x89A2F8 );
	EITreasureIdol* idol = instantiate_object<EITreasureIdol>( type );

	EIHarry* harry = *((EIHarry**) 0x917034);

	Vector3f pos = { 0, 0, 0 };
	Vector4f rot = { 0, 0, 0, 1 };

	memcpy( &pos, &harry->m_position, sizeof(Vector3f) );

	InitIdol( idol, &pos, &rot, 0x816D97BF, 0x14F14DDC, 0x6061A9B3 );

	AddToWorld( harry->m_world, idol );
	AddToWorldFancy( harry->m_world, idol->m_particleEmitter, idol, false );
	ERLevel_SetupEntity( harry->m_world, idol, 0 );
}

static void _idol_spawn_regular()
{
	_idol_spawn();
}


// --------------------------------------------------------------------------------
// List all loaded models.
// --------------------------------------------------------------------------------
static void _list_model_assets()
{
	TreeMapNode* node = ((TreeMap*)(0x91A9B8 + 0x14))->m_iterateFirst;
	while ( node ) {
		if ( node->m_ptr ) {
			EResource* res = (EResource*) node->m_ptr;
			log_printf( "- 0x%X : %s\n", res, res->m_name );
		}

		node = node->m_iterateNext;
	}
}


static void _list_entities()
{
	ERLevel* level = Gizmod::getLevel();
	HashMap* entitiesByID = &level->m_entitiesByID;

	int i = 0;
	for ( HashMapEntry* e = entitiesByID->m_iterateFirst; e != 0; e = e->m_iterateNext ) {
		if ( !e->m_ptr ) {
			Gizmod::getInstance()->getLogger()->log_printf( "- 0x%.8X : (unknown)\n", e->m_key );
			continue;
		}

		EInstance* inst = (EInstance*) e->m_ptr;
		Gizmod::getInstance()->getLogger()->log_printf( "- 0x%.8X : %s\n", e->m_key, inst->GetTypeName() );

		i++;
	}

	Gizmod::getInstance()->getLogger()->log_printf( "Total : %d entities.\n", i );
}


#include "ptle/EUITextIcon.h"
static void _log_children( EUIObjectNode* obj, int depth )
{
	char buf[300];

	// Shift text by depth.
	int i;
	for ( i = 0; i < depth * 2; i += 2 ) {
		buf[i] = ' ';
		buf[i + 1] = ' ';
	}
	buf[i] = 0;

	for ( LinkedListElem* e = obj->m_children.m_iterateFirst; e != 0; e = e->m_ptr1 ) {
		EUIObjectNode* c = (EUIObjectNode*) e->m_data;
		EUITextIcon* t = type_cast<EUITextIcon>( c, get_type_by_vtable(0x8A64B0) );
		if ( t ) {
			sprintf_s( buf + i, sizeof(buf) - i, "- %s (0x%.8X) \"%ls\"\n", c->GetTypeName(), c, *t->m_text );
		}
		else {
			sprintf_s( buf + i, sizeof(buf) - i, "- %s (0x%.8X)\n", c->GetTypeName(), c );
		}
		log_printf( buf );

		if ( obj->m_children.m_iterateFirst ) {
			_log_children( c, depth + 1 ); 
		}
	}
}
static void _log_saveui_contents()
{
	// ESaveUi singleton.
	EUIObjectNode* root = *((EUIObjectNode**) 0x911A08);

	if ( !root ) {
		log_printf( "No active ESaveUI instance.\n" );
		return;
	}

	log_printf( "%s\n", root->GetTypeName() );
	_log_children( root, 0 );
}

static void _log_epausemain()
{
	// EFrontEndGame
	uint8_t* mainGameClass = *((uint8_t**) 0x910580);

	// EPauseMain
	EUIObjectNode* pauseMain = *((EUIObjectNode**) (mainGameClass + 0x20));
	if ( !pauseMain ) {
		return;
	}

	log_printf( "%s\n", pauseMain->GetTypeName() );
	_log_children( pauseMain, 0 );
}


// --------------------------------------------------------------------------------
// Explorer tracker & teleport to explorer.
// --------------------------------------------------------------------------------
#include "ptle/EIExplorer.h"
static Matrix4f m;

GET_METHOD( 0x414F80, void, Tick, EIBeast* );
static void _ExplorerTick( EIExplorer* self )
{
	static int prevState = -1;

	if ( self->m_stateIndexCurrent != prevState ) {
		Gizmod::getInstance()->getLogger()->log_printf( "State switch : %d to %d\n", prevState, self->m_stateIndexCurrent );
		prevState = self->m_stateIndexCurrent;
	}

	m = self->m_transformMatrix;

	Tick( self );
}
MAKE_THISCALL_WRAPPER( ExplorerTick, _ExplorerTick );

GET_METHOD( 0x4B55F0, void, EIHarry_Tp, EIHarry*, Matrix4f*, bool, float );

static void _explorer_tracker()
{
	static bool non = false;
	if ( !non ) {
		non = true;

		// Patch tick function.
		injector::WriteMemory( 0x87642C, ExplorerTick );

		// Display explorer state machine.
		void* dummy = (void*) (0x876400);
		EBeastStateMachine* sm = ((EIBeast*)(&dummy))->GetStateMachine();
		log_state_machine_info( sm );
	}

	Matrix4f mat = m;
	mat.data[3][2] += 5.0F;
	EIHarry_Tp(Gizmod::getHarry(), &mat, false, 1.0F);
}

static void _tp_to_explorer()
{
	std::vector<EIBeast*> expl;
	Gizmod::getInstance()->getWorld()->getEntitiesOfType( expl, get_type_by_vtable(0x876400)->ptleType );

	if (!expl.empty()) {
		Matrix4f mat = expl[0]->m_transformMatrix;
		mat.data[3][2] += 8.0F;
		EIHarry_Tp( Gizmod::getHarry(), &mat, false, 1.0F );
	}
	else {
		Gizmod::getInstance()->getLogger()->log( "No explorer in this level.\n" );
	}
}


// --------------------------------------------------------------------------------
// Teleport to shaman.
// --------------------------------------------------------------------------------
static void _tp_to_shaman()
{
	std::vector<EIBeast*> shams;
	Gizmod::getInstance()->getWorld()->getEntitiesOfType( shams, get_type_by_vtable(0x896C10)->ptleType );

	if (!shams.empty()) {
		Matrix4f mat = shams[0]->m_transformMatrix;
		mat.data[3][2] += 8.0F;
		EIHarry_Tp( Gizmod::getHarry(), &mat, false, 1.0F );
	}
	else {
		Gizmod::getInstance()->getLogger()->log( "No shaman in this level.\n" );
	}
}


// --------------------------------------------------------------------------------
// Display Harry's position.
// --------------------------------------------------------------------------------
static void _display_coords()
{
	EIHarry* harry = Gizmod::getHarry();
	if ( !harry ) return;

	Gizmod::getInstance()->getLogger()->log_printf( "Harry's position : [%f, %f, %f]\n",
		harry->m_position.x,
		harry->m_position.y,
		harry->m_position.z );
}


static void _give_five_idols()
{
	Gizmod::getHarry()->m_idolsCollected += 5;
}

//
// List of available commands / cheats.
//

const cheat_t cheats[] =
{
	/* Name,                   On Enable / Disable */
	{ "Auto Skip Cutscene",   _auto_skip_cutscenes },
	{ "Effector Debug Draw",  _effector_debug_draw },
	{ "Invulnerable",         _invulnerable },
	{ "Infinite Jump",        _infinite_jump },
	{ "TNT Rain",             _tnt_rain },
	{ "All TNT",              _all_tnt },
	{ "Free Camera",          _free_camera },
	{ "Deactivate Effectors", _deactivate_effectors },
};

const command_t commands[] =
{
	/* Name,                  On Trigger */
	{ "Give Stuff",           _give_stuff },
	{ "Heal",                 _heal },
	{ "Remove Stuff",         _remove_stuff },
	{ "Die",                  _die },
	{ "Spawn Evil Harry",     _spawn_evilharry },
	{ "Spawn Micay",          _micay_spawn },
	{ "Spawn Native",         _netiv_spawn },
	{ "Spawn Idol",           _idol_spawn_regular },
	{ "List models",          _list_model_assets },
	{ "List entities w/ IDs", _list_entities },
	{ "TP to Explorer",       _tp_to_explorer },
	{ "TP to Shaman",         _tp_to_shaman },
	{ "Display Harry Pos",    _display_coords },
	{ "Give 5 idols",         _give_five_idols },
};

const int NUM_CHEATS = sizeof(cheats) / sizeof(cheat_t);
const int NUM_COMMANDS = sizeof(commands) / sizeof(command_t);

static bool cheat_states[NUM_CHEATS] = { false, };



//
// Go to...
//

#include <vector>

std::vector<level_info_t> gotoLevels;
std::vector<level_info_t> regularLevels, nativeGamesLevels;

static void init_gotolevels_list()
{
	// Sort levels by category.
	for ( int i = 0; i < levels_game.count; i++ ) {
		uint32_t crc = levels_game.levels[i].crc;
		switch ( crc )
		{
		case levelCRC::KABOOM:
		case levelCRC::PICKAXE_RACE:
		case levelCRC::RAFT_BOWLING:
		case levelCRC::TUCO_SHOOT:
		case levelCRC::WHACK_A_TUCO:
			nativeGamesLevels.push_back( levels_game.levels[i] );
			break;

		default:
			regularLevels.push_back( levels_game.levels[i] );
			break;
		}
	}
}



//
// Save slots.
// 
// These only save Harry's position and orientation, and restore them by teleporting him.
// The camera doesn't follow yet. The player must move around, jump, or move the camera to
// center it back.
//

savepoint_t save_slots[10];

GET_METHOD( 0x60FEA0, void, EICharacter_SetPos, EICharacter*, Vector3f*, bool );
GET_METHOD( 0x4B55F0, void, EIHarry_Teleport, EIHarry*, Matrix4f*, bool, float );

bool load_saveslot( int i )
{
	if ( save_slots[i].levelID == -1 ) {
		log_printf( "Failed to load state %d (slot might be unused).\n", i );
		return false;
	}

	//ScheduleWorldLoad( (void*) 0x917028, levels_game.levels[id - 400].crc, false );
	EIHarry* harry = *((EIHarry**) 0x917034);

	EIHarry_Teleport( harry, &save_slots[i].playerTransform, false, 1.0F );

	log_printf( "Loaded save state %d.\n", i );
	return true;
}

void save_saveslot( int i )
{
	EIHarry* harry = *((EIHarry**) 0x917034);

	save_slots[i].playerTransform = harry->m_rigidBodyMatrix;

	log_printf( "Saved state %d.\n", i );
}



//
// Command palette window.
//

#include <Windows.h>

static struct palette_wnd_data_t
{
	HWND previous_area_label;
	HWND idol_count_label;

} paletteWndData;
static HWND hwndPalette;
static const char className[] = "PaletteWindow";

// Geometry.
#define BUTTON_WIDTH   150
#define MARGIN_WIDTH   10
#define WINDOW_WIDTH   (BUTTON_WIDTH*2 + MARGIN_WIDTH*3)
#define WINDOW_HEIGHT  500

// Identifiers.
#define ID_CHEATS   100
#define ID_COMMANDS 200

#define ID_LEVELS   400

#define ID_ITEMS    600
#define ID_ITEMSWAP (ID_ITEMS+9)
#define ID_SKILLS   (ID_ITEMSWAP+1)

#define ID_INGAMECHEAT 500

#define ID_SAVESLOT_LOAD 700
#define ID_SAVESLOT_SAVE (700+10)


static void PaletteCreate( HWND hwnd )
{
	int y = 10;
	int i = 0;
	for ( const cheat_t* cheat = cheats; cheat != cheats + NUM_CHEATS; cheat++, i++ ) {
		CreateWindow( "BUTTON", cheat->name,
			WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE,
			10 + (i & 1) * (BUTTON_WIDTH+10), y,
			BUTTON_WIDTH, 22, hwnd, (HMENU) (ID_CHEATS + i), 0, 0 );
		if ( i & 1 ) {
			y += 24;
		}
	}
	y += 14;
	i = 0;
	for ( const command_t* cmd = commands; cmd != commands + NUM_COMMANDS; cmd++, i++ ) {
		CreateWindow( "BUTTON", cmd->name,
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			10 + (i & 1) * (BUTTON_WIDTH+10), y,
			BUTTON_WIDTH, 22, hwnd, (HMENU) (ID_COMMANDS + i), 0, 0 );
		if ( i & 1 ) {
			y += 24;
		}
	}
	y += 14;
	paletteWndData.previous_area_label = CreateWindow("STATIC", "Previous Area: ",
		WS_CHILD | WS_VISIBLE,
		10, y, 300, 20,
		hwnd, nullptr, nullptr, nullptr);
	y += 20;
	paletteWndData.idol_count_label = CreateWindow("STATIC", "Idols: ",
		WS_CHILD | WS_VISIBLE,
		10, y, 300, 20,
		hwnd, nullptr, nullptr, nullptr);
}

static void PaletteRefreshInfo( HWND hwnd )
{
	char buffer[64];

	uint32_t* prevAreaPtr = find_previous_area_memory();
	const char* prefix = "Previous Area: ";
	if (!prevAreaPtr) {
		sprintf_s(buffer, "%sNot found", prefix);
	}
	else {
		sprintf_s(buffer, "%s0x%.8X (%s)", prefix, *prevAreaPtr, level_get_name(level_get_by_crc(*prevAreaPtr)));
	}

	SetWindowText(paletteWndData.previous_area_label, buffer);

	EIHarry* harry = *((EIHarry**) 0x917034);
	if ( harry ) {
		sprintf_s(buffer, "Idols: %d", harry->m_idolsCollected);

		SetWindowText(paletteWndData.idol_count_label, buffer);
	}
}

static void PaletteCommand( HWND hwnd, WPARAM id )
{
	if ( id >= ID_CHEATS && id < ID_CHEATS + NUM_CHEATS ) {
		int i = id - ID_CHEATS;
		const cheat_t* cmd = &cheats[i];
		cheat_states[i] = !cheat_states[i];
		cmd->func( cheat_states[i] );
	}
	else if ( id >= ID_COMMANDS && id < ID_COMMANDS + NUM_COMMANDS ) {
		const command_t* cmd = &commands[id - ID_COMMANDS];
		cmd->func();
	}
	else if ( id >= ID_LEVELS && id < ID_LEVELS + gotoLevels.size() ) {
		ScheduleWorldLoad( (void*) 0x917028, gotoLevels[id - ID_LEVELS].crc, false );
	}
	else if ( id >= ID_ITEMS && id < ID_ITEMS+9 ) {
		ItemStruct* items = (ItemStruct*) 0x8EEB90;
		items[id - ID_ITEMS].m_unlocked = !items[id - ID_ITEMS].m_unlocked;
	}
	else if ( id == ID_ITEMSWAP ) {
		bool* itemSwapPtr = (bool*) 0x9106A2;
		(*itemSwapPtr) = !(*itemSwapPtr);
	}
	else if ( id >= ID_SKILLS && id < ID_SKILLS+6 ) {
		EIHarry* harry = *((EIHarry**) 0x917034);
		if ( harry ) {
			switch ( id - ID_SKILLS ) {
			case 0: harry->m_risingStrike = !harry->m_risingStrike; break;
			case 1: harry->m_smashStrike  = !harry->m_smashStrike;  break;
			case 2: harry->m_heroicDash   = !harry->m_heroicDash;   break;
			case 3: harry->m_heroicDive   = !harry->m_heroicDive;   break;
			case 4: harry->m_superSling   = !harry->m_superSling;   break;
			case 5: harry->m_breakdance   = !harry->m_breakdance;   break;
			}
		}
	}
	else if ( id >= ID_INGAMECHEAT && id < ID_INGAMECHEAT+3 ) {
		switch ( id - ID_INGAMECHEAT ) {
		case 0: *((bool*) 0x90DA18) ^= 1; break;
		case 1: *((bool*) 0x90DA1F) ^= 1; break;
		case 2: *((bool*) 0x90DA19) ^= 1; break;
		}
	}
	else if ( id >= ID_SAVESLOT_LOAD && id < ID_SAVESLOT_LOAD+10 ) {
		load_saveslot( id - ID_SAVESLOT_LOAD );
	}
	else if ( id >= ID_SAVESLOT_SAVE && id < ID_SAVESLOT_SAVE+10 ) {
		save_saveslot( id - ID_SAVESLOT_SAVE );
	}
}

static void PaletteUpdateMenuChecks( HWND hwnd, WPARAM wparam, LPARAM lparam )
{
	if ( HIWORD( wparam ) & MF_POPUP ) {
		EIHarry* harry = *((EIHarry**) 0x917034);
		ItemStruct* items = (ItemStruct*) 0x8EEB90;
		HMENU subMenu = GetSubMenu( (HMENU) lparam, LOWORD(wparam) );

		switch ( LOWORD( wparam ) ) {
		case 1:
			for ( int i = 0; i < 9; i++ ) {
				CheckMenuItem( subMenu, ID_ITEMS + i, (items[i].m_unlocked) ? MF_CHECKED : MF_UNCHECKED );
			}
			CheckMenuItem( subMenu, ID_ITEMSWAP, *((bool*) 0x9106A2) ? MF_UNCHECKED : MF_CHECKED );
			break;
		case 2:
			for ( int i = 0; i < 6; i++ ) {
				EnableMenuItem( subMenu, ID_SKILLS + i, harry != 0 ? MF_ENABLED : MF_GRAYED );
			}
			CheckMenuItem( subMenu, ID_SKILLS,   (harry != 0 && harry->m_risingStrike) ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( subMenu, ID_SKILLS+1, (harry != 0 && harry->m_smashStrike)  ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( subMenu, ID_SKILLS+2, (harry != 0 && harry->m_heroicDash)   ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( subMenu, ID_SKILLS+3, (harry != 0 && harry->m_heroicDive)   ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( subMenu, ID_SKILLS+4, (harry != 0 && harry->m_superSling)   ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( subMenu, ID_SKILLS+5, (harry != 0 && harry->m_breakdance)   ? MF_CHECKED : MF_UNCHECKED );
			break;
		case 3:
			CheckMenuItem( subMenu, ID_INGAMECHEAT,   *((bool*) 0x90DA18) ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( subMenu, ID_INGAMECHEAT+1, *((bool*) 0x90DA1F) ? MF_CHECKED : MF_UNCHECKED );
			CheckMenuItem( subMenu, ID_INGAMECHEAT+2, *((bool*) 0x90DA19) ? MF_CHECKED : MF_UNCHECKED );
			break;
		}
	}
}

static LRESULT CALLBACK PaletteWndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	static const int REFRESH_TIMER_ID = 1;

	switch ( msg )
	{
	case WM_CREATE:
		PaletteCreate( hwnd );

		// Set a UI refresh timer at 30 FPS
		SetTimer(hwnd, REFRESH_TIMER_ID, 1000 / 30, NULL);
		break;

	case WM_TIMER: 
		if (wparam == REFRESH_TIMER_ID) {
			PaletteRefreshInfo( hwnd );
		}
		break;

	case WM_CLOSE:
		DestroyWindow( hwnd );
		return 0;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;

	case WM_COMMAND:
		PaletteCommand( hwnd, wparam );
		break;

	case WM_MENUSELECT:
		PaletteUpdateMenuChecks( hwnd, wparam, lparam );
		break;
	}

	return DefWindowProc( hwnd, msg, wparam, lparam );
}

static HMENU create_menu()
{
	HMENU menu = CreateMenu();

	HMENU goTo = CreateMenu(); {
		init_gotolevels_list();
		UINT_PTR id = ID_LEVELS;

		HMENU beta = CreateMenu(); {
			for ( int i = 0; i < levels_beta.count; i++ ) {
				AppendMenu( beta, MF_STRING, id + i, levels_beta.levels[i].displayName );
				gotoLevels.push_back( levels_beta.levels[i] );
			}
			id += levels_beta.count;

			AppendMenu( goTo, MF_POPUP, (UINT_PTR) beta, "Beta" );
		}

		HMENU nativeGames = CreateMenu(); {
			for ( int i = 0; i < nativeGamesLevels.size(); i++ ) {
				AppendMenu( nativeGames, MF_STRING, id + i, nativeGamesLevels[i].displayName );
				gotoLevels.push_back( nativeGamesLevels[i] );
			}
			id += nativeGamesLevels.size();

			AppendMenu( goTo, MF_POPUP, (UINT_PTR) nativeGames, "Native Games" );
		}

		{
			const int rows = 19;
			auto it = regularLevels.begin();
			for ( int i = 0; i < regularLevels.size(); i++, it++ ) {
				UINT flags = MF_STRING;
				if      ( i == 0 )        flags |= MF_MENUBARBREAK;
				else if ( i % rows == 0 ) flags |= MF_MENUBREAK;

				AppendMenu( goTo, flags, id + i, regularLevels[i].displayName );
				gotoLevels.push_back( regularLevels[i] );
			}
			id += regularLevels.size();

			AppendMenu( menu, MF_POPUP, (UINT_PTR) goTo, "Go to" );
		}
	}

	HMENU items = CreateMenu(); {
		AppendMenu( items, MF_STRING, 607, "Canteen" );
		AppendMenu( items, MF_STRING, 600, "Sling" );
		AppendMenu( items, MF_STRING, 601, "Torch" );
		AppendMenu( items, MF_STRING, 604, "Shield" );
		AppendMenu( items, MF_STRING, 606, "Gas Mask" );
		AppendMenu( items, MF_STRING, 605, "Raft" );
		AppendMenu( items, MF_STRING, 602, "Pickaxes" );
		AppendMenu( items, MF_STRING, 603, "TNT" );
		AppendMenu( items, MF_STRING, 608, "Stink Bomb" );   // TODO : doesn't work.

		AppendMenu( items, MF_SEPARATOR, 0, 0 );

		AppendMenu( items, MF_STRING, ID_ITEMSWAP, "Item Swap" );

		AppendMenu( menu, MF_POPUP, (UINT_PTR) items, "Items" );
	}

	HMENU skills = CreateMenu(); {
		AppendMenu( skills, MF_STRING, ID_SKILLS,   "Rising Strike" );
		AppendMenu( skills, MF_STRING, ID_SKILLS+1, "Smash Strike" );
		AppendMenu( skills, MF_STRING, ID_SKILLS+2, "Heroic Dash" );
		AppendMenu( skills, MF_STRING, ID_SKILLS+3, "Heroic Dive" );
		AppendMenu( skills, MF_STRING, ID_SKILLS+4, "Super Sling" );
		AppendMenu( skills, MF_STRING, ID_SKILLS+5, "Breakdance" );

		AppendMenu( menu, MF_POPUP, (UINT_PTR) skills, "Skills" );
	}

	HMENU cheats = CreateMenu(); {
		AppendMenu( cheats, MF_STRING, ID_INGAMECHEAT,   "Nicole Outfit" );
		AppendMenu( cheats, MF_STRING, ID_INGAMECHEAT+1, "Retro Outfit" );
		AppendMenu( cheats, MF_STRING, ID_INGAMECHEAT+2, "Native Outfit" );

		AppendMenu( menu, MF_POPUP, (UINT_PTR) cheats, "Cheats" );
	}

	HMENU saveSlots = CreateMenu(); {
		HMENU load = CreateMenu(); {
			for ( int i = 0; i < 10; i++ ) {
				char name[256] = {0,};
				sprintf( name, "Slot %d", i );
				AppendMenu( load, MF_STRING, ID_SAVESLOT_LOAD+i, name );
			}

			AppendMenu( saveSlots, MF_POPUP, (UINT_PTR) load, "Load..." );
		}
		HMENU save = CreateMenu(); {
			for ( int i = 0; i < 10; i++ ) {
				char name[256] = {0,};
				sprintf( name, "Slot %d", i );
				AppendMenu( save, MF_STRING, ID_SAVESLOT_SAVE+i, name );
			}

			AppendMenu( saveSlots, MF_POPUP, (UINT_PTR) save, "Save..." );
		}

		AppendMenu( menu, MF_POPUP, (UINT_PTR) saveSlots, "Save Slots" );
	}

	return menu;
}



//
// Public API.
//

void command_create_window()
{
	WNDCLASSEX wcex; ZeroMemory( &wcex, sizeof(wcex) );
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hInstance = GetModuleHandle(0);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground = (HBRUSH) COLOR_WINDOW;
	wcex.lpszClassName = className;
	wcex.lpfnWndProc = PaletteWndProc;

	RegisterClassEx( &wcex );

	RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	DWORD style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

	AdjustWindowRect( &rect, style, false );

	HMENU menu = create_menu();
	hwndPalette = CreateWindow( className, "Cheat Palette", style,
		10, 10,
		rect.right - rect.left, rect.bottom - rect.top,
		0, menu, GetModuleHandle(0), 0 );

	// Create & set font recursively.
	HFONT font = CreateFont( 16, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Segoe UI" );
	EnumChildWindows( hwndPalette, [](HWND hwnd, LPARAM p) -> BOOL {
		SendMessage( hwnd, WM_SETFONT, p, 0 );
		return true;
	}, (LPARAM) font );

	ShowWindow( hwndPalette, SW_SHOW );
}

void command_process_window()
{
	MSG msg;
	while ( GetMessage(&msg, 0, 0, 0) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}
