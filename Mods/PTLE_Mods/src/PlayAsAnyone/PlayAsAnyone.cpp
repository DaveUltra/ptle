#include "gizmod/Gizmod.h"
#include "gizmod/GizmodPlugin.h"

#include "gizmod/event/LevelLoadedEvent.h"

#include "gizmod/PluginConfig.h"

#include "ptle/EIHarry.h"
#include "ptle/EIBushNinja.h"
#include "ptle/EINPCBeast.h"

#include "ptle/ERCharacter.h"
#include "ptle/types/types.h"

#include "utils/func.h"
#include "injector/injector.hpp"

#include "Characters/CharacterInfo.h"
#include "Characters/Bittenbinder.h"
#include "Characters/BushNinja.h"
#include "Characters/Chief.h"
#include "Characters/Leech.h"
#include "Characters/Micay.h"
#include "Characters/Shaman.h"
#include "Characters/Spinja.h"
#include "MoreMath.h"

#include "PlayAsAnyone.h"

#include <map>


GET_METHOD( 0x621120, EResource*, LoadAsset, void*, uint32_t, void*, void* );


CharacterInfo* playerDisguise = 0;

CharacterInfo* bittenbinder = new CharacterBittenbinder();
CharacterInfo* bushninja = new CharacterBushNinja();
CharacterInfo* chief = new CharacterChief();
CharacterInfo* leech = new CharacterLeech();
CharacterInfo* micay = new CharacterMicay();
CharacterInfo* shaman = new CharacterShaman();
CharacterInfo* spinja = new CharacterSpinja();

ERCharacter* harryCharacter = 0;
ERCharacter* myCharacter = 0;

static std::vector<EIBeast*> disguisedBeasts;

static std::map<uint32_t, CharacterInfo*> beastDisguises;


static CharacterInfo* getBeastDefaultCharacter( uint32_t vtable, int variant )
{
	//static CharacterInfo* natives[] = { native, renegade };
	static CharacterInfo* npcs[] = { bittenbinder, chief, leech, micay };

	switch (vtable)
	{
	case 0x86D1D8: return bushninja;
	case 0x898C20: return spinja;

	case 0x88B868:
		return npcs[variant];

	default: return 0;
	}
}

static void playAsCharacter( const std::string& name )
{
	if ( name == "harry" ) return;

	else if ( name == "bittenbinder" ) playerDisguise = bittenbinder;
	else if ( name == "bushninja" )    playerDisguise = bushninja;
	else if ( name == "chief" )        playerDisguise = chief;
	else if ( name == "leech" )        playerDisguise = leech;
	else if ( name == "micay" )        playerDisguise = micay;
	else if ( name == "shaman" )       playerDisguise = shaman;
	else if ( name == "spinja" )       playerDisguise = spinja;
}


GET_METHOD( 0x61B860, void, EAnimController_ComputeModelPose, EAnimController*, Matrix4f* );
static void __stdcall _EAnimController_ComputeModelPose_Custom( EAnimController* self, Matrix4f* root )
{
	EICharacter* owner = 0;
	if ( self == &Gizmod::getHarry()->m_animController ) {
		owner = Gizmod::getHarry();
	}
	else {
		for ( EIBeast* b : disguisedBeasts ) {
			if ( self == &b->m_animController ) {
				owner = b;
				break;
			}
		}
	}

	// Unrecognized anim controller.
	if ( !owner ) {
		EAnimController_ComputeModelPose(self, root);
		return;
	}

	// Harry to character mapping.
	if ( owner == Gizmod::getHarry() ) {
		Transform* origPose = (Transform*) malloc( NUM_HARRY_BONES * sizeof(Transform) );
		Matrix4f*  origModelPose = (Matrix4f*) malloc( NUM_HARRY_BONES * sizeof(Matrix4f) );
		Transform* buf = (Transform*) 0x917940;
		const int* map = playerDisguise->m_bonesMap;

		EAnimController_ComputeModelPose( self, root );

		memcpy( origPose, buf, NUM_HARRY_BONES * sizeof(Transform) );
		memcpy( origModelPose, self->m_pose, NUM_HARRY_BONES * sizeof(Matrix4f) );

		for ( int i = 0; i < playerDisguise->m_numBones; i++ ) {
			int mappedID = map[i];
			if (mappedID == -1) {
				IdentityTransform( &buf[i] );
			}
			else {
				memcpy( &buf[i], &origPose[mappedID], sizeof(Transform) );
			}
		}

		playerDisguise->postProcessPose( buf, origPose );

		ERCharacter* originalCharacter = self->m_characterResource;
		self->m_characterResource = myCharacter;
		EAnimController_ComputeModelPose( self, root );
		self->m_characterResource = originalCharacter;

		self->m_pose[71] = origModelPose[71];

		free( origPose );
		free( origModelPose );
	}

	// Character to Harry mapping.
	else {
		uint32_t vtable = *((uint32_t*) owner);
		CharacterInfo* chA = getBeastDefaultCharacter( vtable, 0 );
		CharacterInfo* chB = beastDisguises[vtable];
		if ( !chA ) {
			EAnimController_ComputeModelPose( self, root );
			return;
		}

		Transform* origPose = (Transform*) malloc( chA->m_numBones * sizeof(Transform) );
		Transform* buf = (Transform*) 0x917940;
		const int* map = chA->m_inverseBonesMap;

		memcpy( origPose, buf, chA->m_numBones * sizeof(Transform) );

		for ( int i = 0; i < NUM_HARRY_BONES; i++ ) {
			int mappedID = map[i];
			if (mappedID == -1) {
				IdentityTransform( &buf[i] );
			}
			else {
				memcpy( &buf[i], &origPose[mappedID], sizeof(Transform) );
			}
		}

		if ( chB ) {

		}

		//chb->postProcessPose( buf, origPose );

		free( origPose );

		ERCharacter* originalCharacter = self->m_characterResource;
		self->m_characterResource = harryCharacter;
		EAnimController_ComputeModelPose( self, root );
		self->m_characterResource = originalCharacter;
	}
}
MAKE_THISCALL_WRAPPER( EAnimController_ComputeModelPose_Custom, _EAnimController_ComputeModelPose_Custom );


GET_METHOD( 0x61A970, void, EAnimController_ComputeFinalPose, EAnimController*, Matrix4f*, uint32_t );
static void __stdcall _EAnimController_ComputeFinalPose_Custom( EAnimController* self, Matrix4f* root, uint32_t flags )
{
	EAnimController_ComputeFinalPose( self, root, flags );
}
MAKE_THISCALL_WRAPPER( EAnimController_ComputeFinalPose_Custom, _EAnimController_ComputeFinalPose_Custom );


GET_METHOD( 0x60F770, void, EICharacter_SetModel, EICharacter*, uint32_t );
GET_METHOD( 0x4B42E0, void, EIHarry_Render, EIHarry*, void*, uint32_t );
static void __stdcall _EIHarry_Render_Custom( EIHarry* self, void* renderPtr, uint32_t flags )
{
	ERCharacter* originalCharacter = self->m_animController.m_characterResource;
	self->m_animController.m_characterResource = myCharacter;

	EIHarry_Render( self, renderPtr, flags );

	self->m_animController.m_characterResource = originalCharacter;
}
MAKE_THISCALL_WRAPPER( EIHarry_Render_Custom, _EIHarry_Render_Custom );


GET_METHOD( 0x41C6A0, void, EIBeast_Render, EIBeast*, void*, uint32_t );
static void __stdcall _EIBeast_Render_Custom( EIBeast* self, void* renderPtr, uint32_t flags )
{
	if ( std::find(disguisedBeasts.begin(), disguisedBeasts.end(), self) != disguisedBeasts.end() ) {
		uint32_t vtable = *((uint32_t*) self);
		CharacterInfo* disguise = beastDisguises[vtable];

		EICharacter_SetModel( self, disguise ? disguise->m_modelCRC : 0xC9252A08 );
	}

	EIBeast_Render( self, renderPtr, flags );
}
MAKE_THISCALL_WRAPPER( EIBeast_Render_Custom, _EIBeast_Render_Custom );


GET_METHOD( 0x60F380, void, EICharacter_Init, EICharacter* );
static void __stdcall _EICharacter_Init_Custom( EICharacter* self )
{
	// Load characters.
	if ( !harryCharacter ) {
		void* archiveCharacters = (void*) 0x91EB78;

		myCharacter    = (ERCharacter*) LoadAsset( archiveCharacters, playerDisguise->m_characterCRC, 0, 0 );
		harryCharacter = (ERCharacter*) LoadAsset( archiveCharacters, 0x780BF795, 0, 0 );
	}

	EICharacter_Init( self );

	// Set model override.
	if ( self == Gizmod::getHarry() ) {
		EICharacter_SetModel( self, playerDisguise->m_modelCRC );
	}
	else {
		if ( std::find(disguisedBeasts.begin(), disguisedBeasts.end(), self) != disguisedBeasts.end() ) {
			uint32_t vtable = *((uint32_t*) self);
			CharacterInfo* disguise = beastDisguises[vtable];

			EICharacter_SetModel( self, disguise ? disguise->m_modelCRC : 0xC9252A08 );
		}
	}
}
MAKE_THISCALL_WRAPPER( EICharacter_Init_Custom, _EICharacter_Init_Custom );


static const uint32_t computeFinalPos_calls[] = {
	/* beast */           0x413AC0, 0x41DC63, 0x41ED36,
	/* bush ninja */      0x428D7C, 0x42927A,
	/* crocs */           0x437284, 0x437FFC, 0x4389F1, 0x43AB03, 0x43ABEA,
	/* evil harry mm */   0x478954,
	/* evil harry p */    0x47AF9A, 0x47B0EA, 0x47B732, 0x47B832, 0x47B932,
	/* evil harry s */    0x480A0A, 0x482378,
	/* eyeballs */        0x485603,
	/* games native picks */ 0x492FD2,
	/* mole */            0x521EE8,
	/* native */          0x524AA1,
	/* ninja bush (unsure) */ 0x527570,
	/* plant nox */       0x545D68, 0x5460CC,
	/* player beast */    0x549A1C,
	/* player monkey */   0x54CE40,
	/* player scorpion */ 0x550858,
	/* pusca */           0x561454, 0x5618AA, 0x561940, 0x561A54, 0x561D04,
	/* pusca minion */    0x562F9F,
	/* scorpion */        0x579ADC,
	/* shaman */          0x581577,
	/* spinja */          0x58ADD4, 0x58AFCA, 0x58BBB1,
	/* st clair */        0x58D59B,
	/* supai */           0x58F688, 0x58F6E0, 0x58FE0D, 0x5920CB, 0x592257, 0x59295B, 0x592A27,
	/* swing monkey */    0x594396, 0x594590, 0x594650,
	/* tuco tuco */       0x598AC3,
	/* yoyo monkey */     0x5EE0C6, 0x5EE2C0, 0x5EE380,
	/* character (general) */ 0x60FAA3, 0x6129E0,
};


class PlayAsAnyonePlugin : public GizmodPlugin, public ILevelLoadedListener
{
public:

	virtual const char* getSystemName() const override { return "PlayAsAnyone"; }
	virtual const char* getDisplayName() const override { return "Play As Anyone"; }

	virtual void onLevelLoaded( LevelLoadedEvent& event ) override
	{
		disguisedBeasts.clear();

		std::vector<EIBeast*> allBeasts;
		Gizmod::getInstance()->getWorld()->getEntitiesOfType<EIBeast>( allBeasts, get_type_by_vtable(0x86C3D0)->ptleType );
		for ( EIBeast* b : allBeasts ) {
			uint32_t vtable = *((uint32_t*) b);

			if ( beastDisguises.find(vtable) != beastDisguises.end() ) {
				disguisedBeasts.push_back(b);
			}
		}
	}

	virtual void onEnable() override
	{
		register_types();

		PluginConfig config;
		std::string charName = "harry";

		Gizmod::getInstance()->getEventListener()->registerEvent<LevelLoadedEvent>( this );

		beastDisguises.emplace( 0x86D1D8, (CharacterInfo*) 0 );
		beastDisguises.emplace( 0x898C20, (CharacterInfo*) 0 );

		if ( config.parseDefaultConfigFile() ) {
			charName = config.getOptionString( "playAs", charName );
			playAsCharacter( charName );
		}

		if ( playerDisguise ) {
			// Skeleton mapping.
			injector::WriteMemory( 0x8A5888, EAnimController_ComputeModelPose_Custom );

			// Harry's appearance.
			//injector::WriteMemory( 0x4ADBDF, playerDisguise->m_modelCRC );
			//injector::WriteMemory( 0x4ADBEF, playerDisguise->m_characterCRC );

			// Evil Harry.
			/*
			injector::WriteMemory( 0x478647, playerDisguise->m_modelCRC );      // Monkey Temple
			injector::WriteMemory( 0x478654, playerDisguise->m_characterCRC );  //
			injector::WriteMemory( 0x4807A6, playerDisguise->m_modelCRC );      // Scorpion Temple
			injector::WriteMemory( 0x4807B3, playerDisguise->m_characterCRC );  //
			injector::WriteMemory( 0x47A9D2, playerDisguise->m_modelCRC );      // Penguin Temple
			injector::WriteMemory( 0x47A9E3, playerDisguise->m_characterCRC );  //
			*/

			injector::MakeCALL( 0x428D7C, EAnimController_ComputeFinalPose_Custom );
			injector::MakeCALL( 0x42927A, EAnimController_ComputeFinalPose_Custom );

			// Patching relevant EAnimController calls.
			injector::MakeCALL( 0x4B42B1, EAnimController_ComputeFinalPose_Custom );
			injector::MakeCALL( 0x60FAA3, EAnimController_ComputeFinalPose_Custom );
			injector::MakeCALL( 0x6129E0, EAnimController_ComputeFinalPose_Custom );
			injector::MakeCALL( 0x61E166, EAnimController_ComputeFinalPose_Custom );

			// Character init (harry).
			injector::MakeCALL( 0x4ADC7C, EICharacter_Init_Custom );

			// Character init (beast).
			injector::MakeCALL( 0x41372A, EICharacter_Init_Custom );

			// Superclass Harry render.
			injector::WriteMemory( 0x87D334, EIHarry_Render_Custom );

			// Superclass beast render.
			for ( const auto& pair : beastDisguises ) {
				uint32_t vtable = pair.first;
				injector::WriteMemory( vtable + 0x34, EIBeast_Render_Custom );
			}

			// Nuke a few EICharacter::SetModel() calls.
			injector::MakeRangedNOP( 0x41C7D9, 0x41C7E2 );
			injector::MakeRangedNOP( 0x41C7B1, 0x41C7BA );

			//injector::MakeRangedNOP( 0x60FA8F, 0x60FAA8 );

			Gizmod::getInstance()->getLogger()->log_printf( "Selected character : %s\n", charName.c_str() );
		}
		else {
			Gizmod::getInstance()->getLogger()->log( "No character selected.\n" );
		}
	}
};


DECLARE_PLUGIN( PlayAsAnyonePlugin );
