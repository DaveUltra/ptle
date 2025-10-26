#include "gizmod/Gizmod.h"
#include "gizmod/GizmodPlugin.h"

#include "gizmod/PluginConfig.h"

#include "ptle/EIHarry.h"
#include "ptle/ERCharacter.h"

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


GET_METHOD( 0x621120, EResource*, LoadAsset, void*, uint32_t, void*, void* );


CharacterInfo* ch = 0;
ERModel* harryModel = 0;
ERCharacter* harryCharacter = 0;

static void playAsCharacter( const std::string& name )
{
	if ( name == "harry" ) return;

	else if ( name == "bittenbinder" ) ch = new CharacterBittenbinder();
	else if ( name == "bushninja" )    ch = new CharacterBushNinja();
	else if ( name == "chief" )        ch = new CharacterChief();
	else if ( name == "leech" )        ch = new CharacterLeech();
	else if ( name == "micay" )        ch = new CharacterMicay();
	else if ( name == "shaman" )       ch = new CharacterShaman();
	else if ( name == "spinja" )       ch = new CharacterSpinja();
}


GET_METHOD( 0x61B860, void, EAnimController_ComputeModelPose, EAnimController*, Matrix4f* );
static void __stdcall _EAnimController_ComputeModelPose_Custom( EAnimController* self, Matrix4f* root )
{
	// Retro Harry outfit disabled or not Harry.
	if ( !(*((bool*) 0x90DA1F)) || self != &Gizmod::getHarry()->m_animController ) {
		EAnimController_ComputeModelPose( self, root );
		return;
	}

	Transform* origPose = (Transform*) malloc( ch->m_numBones * sizeof(Transform) );
	Transform* buf = (Transform*) 0x917940;
	const int* map = ch->m_bonesMap;

	memcpy( origPose, buf, ch->m_numBones * sizeof(Transform) );

	for ( int i = 0; i < ch->m_numBones; i++ ) {
		int mappedID = map[i];
		if (mappedID == -1 || mappedID >= ch->m_numBones) {
			IdentityTransform( &buf[i] );
		}
		else {
			memcpy( &buf[i], &origPose[mappedID], sizeof(Transform) );
		}
	}

	ch->postProcessPose( buf, origPose );

	free( origPose );

	EAnimController_ComputeModelPose( self, root );
}
MAKE_THISCALL_WRAPPER( EAnimController_ComputeModelPose_Custom, _EAnimController_ComputeModelPose_Custom );


GET_METHOD( 0x61A970, void, EAnimController_ComputeFinalPose, EAnimController*, Matrix4f*, uint32_t );
static void __stdcall _EAnimController_ComputeFinalPose_Custom( EAnimController* self, Matrix4f* root, uint32_t flags )
{
	// If the function is processing Harry, we spoof the number of bones to trick the game into
	// rendering the full animations.
	const bool isHarry = self == &Gizmod::getHarry()->m_animController;
	ERCharacter* ourCharacter = 0;

	/*if ( isHarry ) {
		ourCharacter = self->m_characterResource;
		self->m_characterResource = harryCharacter;
	}*/

	EAnimController_ComputeFinalPose( self, root, flags );

	/*if ( isHarry ) {
		self->m_characterResource = ourCharacter;
	}*/
}
MAKE_THISCALL_WRAPPER( EAnimController_ComputeFinalPose_Custom, _EAnimController_ComputeFinalPose_Custom );


GET_METHOD( 0x61E150, void, EAnimController_RenderSkinnedModel, EAnimController*, void*, ERModel*, Matrix4f*, uint32_t );
static void __stdcall _EAnimController_RenderSkinnedModel_Custom( EAnimController* self, void* renderPtr, ERModel* model, Matrix4f* root, uint32_t flags )
{
	// If the function is processing Harry, we spoof the number of bones to trick the game into
	// rendering the full animations.
	const bool isHarry = self == &Gizmod::getHarry()->m_animController;
	ERCharacter* ourCharacter = 0;

	if ( isHarry ) {
		ourCharacter = self->m_characterResource;
		self->m_characterResource = harryCharacter;
		model = harryModel;
		flags = flags & ~4;
	}

	EAnimController_RenderSkinnedModel( self, renderPtr, model, root, flags );

	if ( isHarry ) {
		self->m_characterResource = ourCharacter;
	}
}
MAKE_THISCALL_WRAPPER( EAnimController_RenderSkinnedModel_Custom, _EAnimController_RenderSkinnedModel_Custom );


GET_METHOD( 0x60F380, void, EICharacter_Init, EICharacter* );
static void __stdcall _EICharacter_Init_Custom( EICharacter* self )
{
	if ( !harryCharacter ) {
		void* archiveModels     = (void*) 0x91A9B8;
		void* archiveCharacters = (void*) 0x91EB78;
		harryModel     = (ERModel*)     LoadAsset( archiveModels,     ch->m_modelCRC,     0, 0 );
		harryCharacter = (ERCharacter*) LoadAsset( archiveCharacters, ch->m_characterCRC, 0, 0 );
	}

	EICharacter_Init( self );
}
MAKE_THISCALL_WRAPPER( EICharacter_Init_Custom, _EICharacter_Init_Custom );



class PlayAsAnyonePlugin : public GizmodPlugin
{
public:

	virtual const char* getSystemName() const override { return "PlayAsAnyone"; }
	virtual const char* getDisplayName() const override { return "Play As Anyone"; }

	virtual void onEnable() override
	{
		PluginConfig config;
		std::string charName = "harry";

		if ( config.parseDefaultConfigFile() ) {
			charName = config.getOptionString( "playAs", charName );
			playAsCharacter( charName );
		}

		if ( ch ) {
			// Skeleton mapping.
			injector::WriteMemory( 0x8A5888, EAnimController_ComputeModelPose_Custom );

			// Harry's appearance.
			//injector::WriteMemory( 0x4ADBDF, ch->m_modelCRC );
			//injector::WriteMemory( 0x4ADBEF, ch->m_characterCRC );

			// Evil Harry.
			injector::WriteMemory( 0x478647, ch->m_modelCRC );      // Monkey Temple
			injector::WriteMemory( 0x478654, ch->m_characterCRC );  //
			injector::WriteMemory( 0x4807A6, ch->m_modelCRC );      // Scorpion Temple
			injector::WriteMemory( 0x4807B3, ch->m_characterCRC );  //
			injector::WriteMemory( 0x47A9D2, ch->m_modelCRC );      // Penguin Temple
			injector::WriteMemory( 0x47A9E3, ch->m_characterCRC );  //

			// Patching relevant EAnimController calls.
			injector::MakeCALL( 0x4B42B1, EAnimController_ComputeFinalPose_Custom );
			injector::MakeCALL( 0x60FAA3, EAnimController_ComputeFinalPose_Custom );
			injector::MakeCALL( 0x61E166, EAnimController_ComputeFinalPose_Custom );

			injector::MakeCALL( 0x4ADC7C, EICharacter_Init_Custom );

			injector::MakeCALL( 0x60FC02, EAnimController_RenderSkinnedModel_Custom );

			Gizmod::getInstance()->getLogger()->log_printf( "Selected character : %s\n", charName.c_str() );
		}
		else {
			Gizmod::getInstance()->getLogger()->log( "No character selected.\n" );
		}
	}
};


DECLARE_PLUGIN( PlayAsAnyonePlugin );
