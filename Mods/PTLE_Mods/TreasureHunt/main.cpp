
#include "gizmod/Gizmod.h"
#include "gizmod/GizmodPlugin.h"

#include "gizmod/event/LoadLevelEvent.h"
#include "gizmod/event/LevelLoadedEvent.h"
#include "gizmod/event/CollectIdolEvent.h"

#include "ptle/levels/level_info.h"
#include "ptle/EInstance.h"
#include "ptle/EITreasureIdol.h"
#include "ptle/EIHarry.h"
#include "ptle/EUIHud.h"
#include "ptle/math/Vector4f.h"

#include "utils/func.h"
#include "injector/injector.hpp"

#include <map>
#include <set>


GET_FUNC( 0x6F8DA0, uint32_t, HashMemBuffer, void*, uint32_t );

GET_METHOD( 0x613990, void, RemoveDynamicInstance, EInstance* );

GET_METHOD( 0x49C4D0, void, EGamesUI3dScore_Init, EInstance*, int, int, Vector3f, uint32_t, bool );
GET_METHOD( 0x5D31A0, void, EUIHudMain_Tick, EUIHud* );
GET_METHOD( 0x5D3520, void, EUIHudMain_Render, EUIHud*, void*, void* );
GET_METHOD( 0x49D800, void, EGamesUI3dScore_SetScore, EInstance*, int );
GET_METHOD( 0x49D830, void, EGamesUI3dScore_SetStartScore, EInstance*, int );
GET_METHOD( 0x49D6E0, void, EGamesUI3dScore_DecrementScore, EInstance* );

GET_METHOD( 0x626670, void, AddToWorld, ERLevel*, EInstance* );
GET_METHOD( 0x6265A0, void, AddToWorldFancy, ERLevel*, EInstance*, EInstance*, bool );
GET_METHOD( 0x6268D0, void, ERLevel_SetupEntity, ERLevel*, EInstance*, EInstance* );
GET_METHOD( 0x5977B0, void, EITreasureIdol_Init, EITreasureIdol* );
GET_METHOD( 0x597400, void, EITreasureIdol_InitValues, EITreasureIdol*, const Vector3f*, const Vector4f*, uint32_t, uint32_t, uint32_t );

GET_METHOD( 0x5229B0, void, SetMusic, void*, uint32_t, bool );


GET_METHOD( 0x49DBC0, void, ScoreHide, EUIObjectNode* );
GET_METHOD( 0x49DBE0, void, ScoreShow, EUIObjectNode* );
GET_METHOD( 0x5C68B0, int, GetScoreState, EUIObjectNode* );


static EUIObjectNode* ui;
static int rem_idols_count = 0;

class M : public EUIHud
{
public:

	void TickUI()
	{
		EUIHudMain_Tick(this);

		switch ( m_state )
		{
		case 1:
			if ( GetScoreState(ui) != 2 ) {
				ScoreShow(ui);
			}
			break;

		case 3:
			if ( GetScoreState(ui) != 0 ) {
				ScoreHide(ui);
			}
			break;
		}

		if ( m_state != 0 ) {
			ui->Tick();
			EGamesUI3dScore_SetStartScore( ui, 0 );
			EGamesUI3dScore_SetScore( ui, rem_idols_count );
		}
	}

	void RenderUI(void* p0, void* p1)
	{
		EUIHudMain_Render(this, p0, p1);

		if ( m_state != 0 && !(*((bool*) 0x912CC8)) ) {
			ui->Render(p0, p1);
		}
	}
};


enum StartingAbility
{
	CANTEEN,
	SLING,
	TORCH,
	SHIELD,
	GASMASK,
	RAFT,
	PICKAXES,
	TNT,
	STINKBOMB,

	RISING_STRIKE,
	SMASH_STRIKE,
	HEROIC_DASH,
	HEROIC_DIVE,
	BREAKDANCE,
	SUPER_SLING,
};

class HuntScenario
{
public:

	levelCRC::Enum startLevel;
	std::vector<StartingAbility> startingAbilities;
	std::map<levelCRC::Enum, std::vector<Vector3f>> idolPlacement;

	int getTotalIdolCount() const
	{
		int count = 0;
		for ( auto pair : idolPlacement ) {
			count += pair.second.size();
		}
		return count;
	}
};



static HuntScenario courtyard_mts =
{
	levelCRC::FLOODED_COURTYARD,
	{ },
	{
		{   levelCRC::FLOODED_COURTYARD,
			{
				{  102.010788, -115.286522, 6.326990 },
				{  82.194466,  -104.376343, 20.517553 },
				{  65.266258,  -86.101440,  108.902649 },
				{ -22.878366,  -87.604660,  102.084785 },
				{ -124.594803, -137.542313, 106.816895 },
				{ -210.114273, -88.246109,  119.410332 },
				{ -197.859482,  65.991707,  119.410332 },
				{ -203.312027,  192.332855, 93.371483 },
				{ -320.662323,  380.768036, 119.410301 },
				{ -325.989197,  573.517212, 119.410332 },
			}
		}
	}
};

static HuntScenario turtle_monument_short =
{
	levelCRC::TURTLE_MONUMENT,
	{ HEROIC_DASH },
	{
		{   levelCRC::TURTLE_MONUMENT,
			{
				{ -166.383728, -75.017776, -4.578650 },
				{ -110.391052, -87.390785, -4.393937 },
				{ -78.261070, -92.383415, 2.392612 },
				{ -46.433327, -102.385101, 3.144650 },
				{ -22.100163, -121.653061, 3.432478 },
				{ -1.367309, -143.379776, 3.032932 },
				{  0.853697, -270.203583, -13.136160 },
				{ -30.266031, -384.710724, -15.054810 },
				{ -39.981194, -421.994751, 31.848087 },
				{ -73.671326, -375.951508, 37.193520 },
				{ -106.857513, -342.086670, 37.193516 },
				{ -153.709442, -268.201935, 57.877388 },
				{ -111.247948, -222.209274, 59.537418 },
			}
		}
	}
};

static HuntScenario turtle_monument_long =
{
	levelCRC::TURTLE_MONUMENT,
	{ HEROIC_DASH, RISING_STRIKE, TNT, PICKAXES },
	{
		{   levelCRC::TURTLE_MONUMENT,
			{
				{ -166.383728, -75.017776, -4.578650 },
				{ -110.391052, -87.390785, -4.393937 },
				{ -78.261070, -92.383415, 2.392612 },
				{ -46.433327, -102.385101, 3.144650 },
				{ -22.100163, -121.653061, 3.432478 },
				{ -1.367309, -143.379776, 3.032932 },
				{  0.853697, -270.203583, -13.136160 },
				{ -30.266031, -384.710724, -15.054810 },
				{ -39.981194, -421.994751, 31.848087 },
				{ -73.671326, -375.951508, 37.193520 },
				{ -106.857513, -342.086670, 37.193516 },
				{ -153.709442, -268.201935, 57.877388 },
				{ -111.247948, -222.209274, 59.537418 },
				{ -247.086380, -78.263741, -15.705510 },
			}
		},
		{   levelCRC::TELEPORT,
			{
				{ -574.207458, 14.539450, 0.172576 },
				{ -574.326477, -60.027332, 2.000042 },
			}
		},
		{   levelCRC::WHITE_VALLEY,
			{
				{ 130.632538, 217.561752, -16.724506 },
				{ 126.695999, 136.606873, -12.362529 },
				{ 177.468414, 25.441376, -0.495964 },
				{ 139.579910, -41.344410, -10.725578 },
				{ 74.128288, -11.977942, 20.183189 },
				{ 304.905304, 143.853485, 35.774742 },
				{ 143.383759, 167.040787, 34.559772 },
			}
		}
	}
};

static HuntScenario* scenario = &turtle_monument_long;

static std::set<uint32_t> collectedIdols;



class TreasureHuntPlugin : public GizmodPlugin
	, public ILoadLevelListener
	, public ILevelLoadedListener
	, public ICollectIdolListener
{
public:

	virtual const char* getDisplayName() const override { return "Treasure Hunt"; }
	virtual const char* getSystemName() const override { return "TreasureHunt"; }

	static uint32_t makeIdolIDForCurrentLevel( int idolIndex )
	{
		uint32_t buf[] = { levelCRC::FLOODED_COURTYARD, idolIndex };
		return HashMemBuffer( &buf, sizeof(buf) );
	}

	void spawnIdol( int index, ERLevel* world, const Vector3f& pos, const Vector4f& rot )
	{
		uint32_t uniqueID = makeIdolIDForCurrentLevel( index );
		if ( collectedIdols.find(uniqueID) != collectedIdols.end() ) {
			return;
		}

		const type_info_t* type = get_type_by_vtable( 0x89A2F8 );
		EITreasureIdol* idol = instantiate_object<EITreasureIdol>( type );

		idol->m_uniqueID = uniqueID;

		EITreasureIdol_InitValues( idol, &pos, &rot, 0x816D97BF, 0x14F14DDC, 0x6061A9B3 );

		AddToWorld( world, idol );
		AddToWorldFancy( world, idol->m_particleEmitter, idol, false );
		ERLevel_SetupEntity( world, idol, 0 );

		EITreasureIdol_Init( idol );
	}

	void grantStartingAbilities()
	{
		EIHarry* harry = Gizmod::getHarry();
		InventoryItem::Item item;
		int numItems = 0;

		for ( StartingAbility a : scenario->startingAbilities ) {
			switch ( a )
			{
			case RISING_STRIKE: harry->m_risingStrike = 1; break;
			case SMASH_STRIKE:  harry->m_smashStrike  = 1; break;
			case HEROIC_DASH:   harry->m_heroicDash   = 1; break;
			case HEROIC_DIVE:   harry->m_heroicDive   = 1; break;
			case BREAKDANCE:    harry->m_breakdance   = 1; break;
			case SUPER_SLING:   harry->m_superSling   = 1; break;

			case CANTEEN: case SLING: case TORCH: case SHIELD:
			case GASMASK: case RAFT: case PICKAXES: case TNT: case STINKBOMB:
				switch ( a )
				{
				case CANTEEN:  item = InventoryItem::CANTEEN; break;
				case SLING:    item = InventoryItem::SLING; break;
				case TORCH:    item = InventoryItem::TORCH; break;
				case SHIELD:   item = InventoryItem::SHIELD; break;
				case GASMASK:  item = InventoryItem::GAS_MASK; break;
				case RAFT:     item = InventoryItem::RAFT; break;
				case PICKAXES: item = InventoryItem::PICKAXES; break;
				case TNT:      item = InventoryItem::TNT; break;
				}
				// Assign to hotbar.
				if ( numItems < 4 ) {
					InventoryItem::setItemInHotbar( numItems, item );
				}
				Gizmod::getInstance()->getInventoryItem( item )->setUnlocked( true );
				numItems++;
				break;
			}
		}
	}


	virtual void onLoadLevel( LoadLevelEvent& event )
	{
		
	}

	virtual void onLevelLoaded( LevelLoadedEvent& event )
	{
		GizmodWorld* world = Gizmod::getInstance()->getWorld();

		// Remove all vanilla idols & explorers.
		std::vector<EInstance*> idols;
		std::vector<EInstance*> explorers;
		world->getEntitiesOfType( idols, get_type_by_vtable(0x89A2F8)->ptleType );
		world->getEntitiesOfType( explorers, get_type_by_vtable(0x876400)->ptleType );

		for ( EInstance* inst : idols ) {
			EITreasureIdol* idol = (EITreasureIdol*) inst;
			RemoveDynamicInstance( idol->m_particleEmitter );
			idol->m_collected = true;
			idol->m_frozen = true;
			idol->m_flags &= ~0xf;
		}

		for ( EInstance* inst : explorers ) {
			inst->m_flags |= 0x90;
			inst->m_flags &= ~0xf;
		}

		// Spawn custom.
		auto it = scenario->idolPlacement.find( (levelCRC::Enum) Gizmod::getCurrentLevelCRC() );
		if ( it != scenario->idolPlacement.end() ) {
			const std::vector<Vector3f>& idolPositions = it->second;
			const int count = idolPositions.size();

			const Vector4f quatID = { 0, 0, 0, 1 };
			for ( int i = 0; i < count; i++ ) {
				spawnIdol( i, Gizmod::getLevel(), idolPositions[i], quatID );
			}
		}

		// Spawn score.
		ui = instantiate_object<EUIObjectNode>( get_type_by_vtable(0x87A3D8) );
		EGamesUI3dScore_Init( ui, rem_idols_count, 90, {-250.0F, 0.0F, -160.0F}, 0xbf1583a7, true );

		// Spirit fight / Native games music.
		event.setMusicOverride( 0xB05ADA8F );

		static bool firstplay = true;
		if (firstplay) {
			firstplay = false;
			grantStartingAbilities();
		}
	}

	virtual void onCollectIdol( CollectIdolEvent& event )
	{
		rem_idols_count--;
		EGamesUI3dScore_SetScore( ui, rem_idols_count );

		collectedIdols.insert( event.getEntity()->m_uniqueID );
	}


	virtual void onEnable()
	{
		register_types();

		rem_idols_count = scenario->getTotalIdolCount();
		Gizmod::getInstance()->getLogger()->log_printf("%d\n", rem_idols_count);

		EventListener* eventListener = Gizmod::getInstance()->getEventListener();
		eventListener->registerEvent<LoadLevelEvent>( this );
		eventListener->registerEvent<LevelLoadedEvent>( this );
		eventListener->registerEvent<CollectIdolEvent>( this );

		// Hijack main UI to include our idol counter.
		injector::WriteMemory( 0x8A070C, &M::TickUI );
		injector::WriteMemory( 0x8A0714, &M::RenderUI );

		// Starting area.
		injector::WriteMemory( 0x5EB9E6, scenario->startLevel );
	}
};

DECLARE_PLUGIN( TreasureHuntPlugin );
