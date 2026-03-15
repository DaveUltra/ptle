/*
 * Randomization of anything that's collectible, or that can be momentarily taken away :
 * - Items
 * - Idols
 * - Heroic abilities (dash, dive, smash strike...)
 * - Base moves (rolling, striking...)
 */

#include "item_rando.h"

#include "rando.h"

#include "gizmod/Gizmod.h"
#include "gizmod/event/HeroicUnlockEvent.h"
#include "gizmod/event/ShamanPurchaseEvent.h"

#include "ptle/EIHarry.h"
#include "ptle/EScriptContext.h"
#include "ptle/EITreasureIdol.h"
#include "ptle/EHarryActions.h"
#include "ptle/EUIDDStoreItem.h"
#include "ptle/levels/level_info.h"

#include "injector/injector.hpp"
#include "utils/func.h"
#include "utils/log.h"

#include <map>
#include <vector>
#include <algorithm>



//
// Definitions.
//


GET_METHOD( 0x506170, void, UnlockItem, void*, uint32_t );
GET_FUNC( 0x5E34A0, void, AddCollectedIdols, uint32_t, int );
GET_METHOD( 0x597400, void, EITreasureIdol_InitValues, EITreasureIdol*, Vector3f*, Vector4f*, uint32_t, uint32_t, uint32_t );

GET_FUNC( 0x4E9EC0, void, Script_HarryIsInInventory, EScriptContext* );
GET_FUNC( 0x65BDF0, int*, PopScriptVariable_Int, EScriptContext* );
GET_FUNC( 0x65C880, int*, GetOutVariable, EScriptContext* );

static uint32_t get_item_default_location( uint32_t itemHash );
static uint32_t get_skill_default_location( EHeroicSkill::Enum skill );



enum UnlockableType
{
	INVENTORY_ITEM = 0,  // Harry's items (canteen, sling, etc...)
	IDOL_SINGLE,         // Idols in totems.
	IDOL_EXPLORER,       // Idols given by explorers.
	IDOL_RAFT,           // Special idol in Mountain Sled Run.
	ARTIFACT,            // Temple artifacts.
	HEROIC_SKILL,        // Harry abilities, excluding base moves.
	HARRY_ACTION,        // Base moves (rolling, punching...).
	NOTES,			  	 // Notes in the shaman shop.
};

struct Unlockable
{
	UnlockableType m_type;
	union {
		uint32_t m_itemHash;
		EHeroicSkill::Enum m_heroicSkill;
		EHarryActions::Enum m_harryAction;
		InventoryItem::Notes m_note;
		const Idol* m_idol;
	};
	const char* m_displayName;
	const wchar_t* m_displayNameW;

	Unlockable( UnlockableType type, uint32_t id, const char* name, const wchar_t* nameW )
		: m_type(type), m_itemHash(id), m_displayName(name), m_displayNameW(nameW)
	{
	}

	void grant()
	{
		EIHarry* harry;

		switch ( m_type )
		{
		case INVENTORY_ITEM:
			harry = Gizmod::getHarry();
			if (  InventoryItem::getItemByHash(m_itemHash) == InventoryItem::STINK_BOMB ) {
				*((bool*) 0x90DA22) = true;
			}
			else {
				UnlockItem( harry->m_itemHotbar, m_itemHash );
			}
			Gizmod::getInstance()->getLogger()->log_printf( "Collected %s!\n", m_displayName );
			break;
		case IDOL_SINGLE:
			AddCollectedIdols( m_idol->m_levelCRC, 1 );
			Gizmod::getInstance()->getLogger()->log( "Collected an idol!\n" );
			break;
		case IDOL_EXPLORER:
			AddCollectedIdols( m_idol->m_levelCRC, 5 );
			Gizmod::getInstance()->getLogger()->log( "Collected an explorer idol!\n" );
			break;
		case HEROIC_SKILL:
			EHeroicSkill::unlockSkill( m_heroicSkill );
			Gizmod::getInstance()->getLogger()->log_printf( "Collected %s!\n", m_displayName );
			break;
		case HARRY_ACTION:
			EHarryActions::enableAction( m_harryAction );
			g_itemRando_harryActions = EHarryActions::getActionsBitfield();
			Gizmod::getInstance()->getLogger()->log_printf( "Collected %s!\n", m_displayName );
			break;
		case NOTES:
			InventoryItem::setNotesUnlocked( m_note, true );
			Gizmod::getInstance()->getLogger()->log_printf( "Collected %s!\n", m_displayName );
			break;
		}
	}

	// Currently unused.
	void revoke()
	{
		switch ( m_type )
		{
		case INVENTORY_ITEM:
			break;
		}
	}

	// Does the player have that unlockable?
	bool isUnlocked() const
	{
		switch ( m_type )
		{
		case INVENTORY_ITEM: return Gizmod::getInstance()->getInventoryItem( InventoryItem::getItemByHash(m_itemHash) )->isUnlocked();
		//case IDOL_SINGLE:    return Gizmod::getInstance()->getSaveManager()->getNativeManager()->GetVar();
		//case IDOL_EXPLORER:  return Gizmod::getInstance()->getSaveManager()->getNativeManager()->GetVar();
		case HEROIC_SKILL:   return EHeroicSkill::isSkillUnlocked( m_heroicSkill );
		case HARRY_ACTION:   return EHarryActions::isActionEnabled( m_harryAction );
		case NOTES:          return InventoryItem::isNotesUnlocked( m_note );
		}
		return false;
	}

	// CRC of the original level we're supposed to be getting this item from.
	uint32_t getDefaultLocationCRC() const
	{
		switch ( m_type )
		{
		case INVENTORY_ITEM: return get_item_default_location( m_itemHash );

		case IDOL_SINGLE:
		case IDOL_EXPLORER:  return m_idol->m_levelCRC;

		case HEROIC_SKILL:   return get_skill_default_location( m_heroicSkill );
		}
		return 0;
	}

	// Model to represent this unlockable element (if replacing an idol).
	uint32_t getModelCRC() const
	{
		switch ( m_type )
		{
		case INVENTORY_ITEM: return Gizmod::getInstance()->getInventoryItem( InventoryItem::getItemByHash(m_itemHash) )->getModelCRC();
		case IDOL_SINGLE:    return ItemModelsCRC::IDOL_SILVER;
		case IDOL_EXPLORER:  return ItemModelsCRC::IDOL_EXPLORER;
		case HEROIC_SKILL:   return ItemModelsCRC::ARTIFACT_CHAMELEON;//return 0x4AF495E1;   // TODO : Heroic handbook? Or something else?
		case HARRY_ACTION:   return ItemModelsCRC::ARTIFACT_CHAMELEON;//return 0x4AF495E1;   //
		case NOTES:  	     return ItemModelsCRC::ARTIFACT_CHAMELEON;
		}
		return 0;
	}

	// Icon to represent this unlockable element (if present in shaman shop).
	uint32_t getShaderIconCRC() const
	{
		switch ( m_type )
		{
		case INVENTORY_ITEM:    return itemShaderIcon( InventoryItem::getItemByHash(m_itemHash) );
		//case IDOL_SINGLE:     Purchasing idols
		//case IDOL_EPLORER:    makes no sense.
		case HEROIC_SKILL:
		case HARRY_ACTION:
		case NOTES:              return 0xDEB3BFDA;    // Default icon, which is a drawn Harry face.
		}
		return 0;
	}


	static uint32_t itemShaderIcon( InventoryItem::Item item )
	{
		switch ( item )
		{
		case InventoryItem::SLING:    return 0x14461AFB;
		case InventoryItem::TORCH:    return 0x81B14648;
		case InventoryItem::PICKAXES: return 0x27E1BAF9;
		case InventoryItem::TNT:      return 0xC5E75E11;
		case InventoryItem::SHIELD:   return 0x93E99760;
		case InventoryItem::RAFT:     return 0x82B2D5FE;
		case InventoryItem::CANTEEN:  return 0x02D920F7;
		case InventoryItem::GAS_MASK: return 0x280CA646;
		//case InventoryItem::SLING:    return 0x6F844D20    This is the pogostilt.
		case InventoryItem::STINK_BOMB: return 0xB260B22E;
		}
		return 0;

		// Default shaman icons :
		// D8E700DC - Health / Canteen
		// DEB3BFDA - Heroic Ability
		// C10458B7 - Notes
		// 4B601A6B - Mystery Item
	}
};


#define NAME( s ) s, L##s
static Unlockable g_unlockableItems[] =
{
	{ INVENTORY_ITEM, 0x915AA574, NAME("Sling") },
	{ INVENTORY_ITEM, 0x04ADF9C7, NAME("Torch") },
	{ INVENTORY_ITEM, 0xC024157C, NAME("Pickaxes") },
	{ INVENTORY_ITEM, 0x9608A818, NAME("TNT") },
	{ INVENTORY_ITEM, 0xEE6B156E, NAME("Shield") },
	{ INVENTORY_ITEM, 0xFB3D82AC, NAME("Raft") },
	{ INVENTORY_ITEM, 0xCFC909C3, NAME("Gas Mask") },
	{ INVENTORY_ITEM, 0xE51C8F72, NAME("Canteen") },
	//{ INVENTORY_ITEM, 0x55A51DAB, "Stink Bomb" },
};

// These serve as storage for Unlockables essentially.
static std::vector<Unlockable> g_unlockableIdols;
static std::vector<Unlockable> g_unlockableExplorers;
static std::vector<Unlockable> g_unlockableHeroicSkills;
static std::vector<Unlockable> g_unlockableHarryActions;
static std::vector<Unlockable> g_unlockableNotes;

static std::map<uint32_t, uint32_t> g_itemLocations;

uint32_t g_itemRando_harryActions = 0;


// Global item rando mapping.
struct less : public std::less<Unlockable*> {
	bool operator()( Unlockable* const a, Unlockable* const b ) const {
		if ( a->m_type == b->m_type ) {
			return a->m_itemHash < b->m_itemHash;
		}
		else {
			return a->m_type < b->m_type;
		}
	}
};
static std::map<Unlockable*, Unlockable*, less> g_unlockablesMap;

static Unlockable* mapUnlockable( UnlockableType type, uint32_t item )
{
	Unlockable u( type, item, 0, 0 );
	auto it = g_unlockablesMap.find( &u );
	return it == g_unlockablesMap.end() ? 0 : it->second;
}

static const Idol* get_idol( uint32_t levelCRC, uint32_t uniqueID )
{
	if ( uniqueID == 0x412B274 ) {
		auto it = explorer_info.find( levelCRC );
		if ( it != explorer_info.end() ) {
			return &it->second;
		}
	}
	else {
		auto it = idols_info.find( levelCRC );
		if ( it != idols_info.end() ) {
			for ( const Idol& i : it->second ) {
				if ( i.m_uniqueID == uniqueID ) {
					return &i;
				}
			}
		}
	}
	return 0;
}

static uint32_t get_item_default_location( uint32_t itemHash )
{
	switch ( itemHash )
	{
	case 0x915AA574: return levelCRC::BITTENBINDER_CAMP;
	case 0x04ADF9C7: return levelCRC::MOUTH_OF_INTI;
	case 0xC024157C: return levelCRC::MOUNTAIN_OVERLOOK;
	case 0x9608A818: return levelCRC::APU_ILLAPU_SHRINE;
	case 0xEE6B156E: return levelCRC::NATIVE_VILLAGE;
	case 0xFB3D82AC: return levelCRC::CAVERN_LAKE;
	case 0xCFC909C3: return levelCRC::RENEGADE_HEADQUARTERS;
	case 0xE51C8F72: return levelCRC::PLANE_COCKPIT;
	default: return 0;
	}
}

static uint32_t get_skill_default_location( EHeroicSkill::Enum skill )
{
	switch ( skill )
	{
	case EHeroicSkill::RISING_STRIKE: return levelCRC::BITTENBINDER_CAMP;
	case EHeroicSkill::HEROIC_DASH:   return levelCRC::FLOODED_COURTYARD;
	case EHeroicSkill::HEROIC_DIVE:   return levelCRC::TURTLE_MONUMENT;
	default: return 0;   // The rest is shaman purchasable.
	}
}



//
// Code injection section.
//


static void collectItem( CollectItemEvent& event )
{
	uint32_t currentAreaCRC = Gizmod::getCurrentLevelCRC();

	// St.Claire's Camp : Don't randomize items!
	if ( currentAreaCRC == levelCRC::ST_CLAIRE_EXCAVATION_CAMP_NIGHT || currentAreaCRC == levelCRC::ST_CLAIRE_EXCAVATION_CAMP_DAY ) {
		return;
	}

	// Getting Sling from Gates of El Dorado cutscene.
	if ( currentAreaCRC == levelCRC::GATES_OF_EL_DORADO ) {
		event.setCancelled( true );
		return;
	}

	// Native Village cutscene gives shield twice.
	if ( currentAreaCRC == levelCRC::NATIVE_VILLAGE ) {
		static bool unlockedShield = false;
		if ( unlockedShield ) {
			event.setCancelled( true );
			return;
		}
		unlockedShield = true;
	}

	// Find override.
	Unlockable* u = mapUnlockable( INVENTORY_ITEM, event.getItemHash() );
	if ( u ) {
		event.setCancelled( true );
		u->grant();
	}
}

static bool collectIdol( EITreasureIdol* self, uint32_t levelCRC, int amount )
{
	levelCRC = Gizmod::getCurrentLevelCRC();

	// Find override.
	const Idol* idol = get_idol( levelCRC, self->m_uniqueID );
	if ( idol == 0 ) {
		log_printf( "No idol found! Was idols_infos.json loaded correctly?\n" );
		return false;
	}

	Unlockable* u = mapUnlockable( idol->isExplorerIdol() ? IDOL_EXPLORER : IDOL_SINGLE, (uint32_t) idol );
	if ( u ) {
		u->grant();
		return true;
	}
	return false;
}

static void _EITreasureIdol_InitValues_custom( EITreasureIdol* self, Vector3f* pos, Vector4f* rot, uint32_t modelCRC, uint32_t particleCRC, uint32_t soundCRC )
{
	uint32_t areaCRC = Gizmod::getCurrentLevelCRC();

	// Replace gas mask in Renegade HQ.
	InventoryItem* gasmask = Gizmod::getInstance()->getInventoryItem( InventoryItem::GAS_MASK );
	if ( areaCRC == levelCRC::RENEGADE_HEADQUARTERS && modelCRC == gasmask->getModelCRC() ) {
		Unlockable* u = mapUnlockable( INVENTORY_ITEM, gasmask->getHash() );
		if ( u ) {
			modelCRC = u->getModelCRC();
		}
	}

	// Replace idol models.
	const Idol* idol = get_idol( areaCRC, self->m_uniqueID );
	if ( idol ) {
		Unlockable* u = mapUnlockable( idol->isExplorerIdol() ? IDOL_EXPLORER : IDOL_SINGLE, (uint32_t) idol );
		if ( u ) {
			modelCRC = u->getModelCRC();

			// Raise model a bit for items, otherwise they look weird (and Valley of Spirits 2nd idol is stupid hard to grab).
			if ( u->m_type == INVENTORY_ITEM ) {
				pos->z += 1.0F;
			}
		}
	}

	EITreasureIdol_InitValues( self, pos, rot, modelCRC, particleCRC, soundCRC );
}
MAKE_THISCALL_WRAPPER( EITreasureIdol_InitValues_custom, _EITreasureIdol_InitValues_custom );

static void Script_HarryIsInInventory_custom( EScriptContext* context )
{
	uint32_t currentAreaCRC = Gizmod::getCurrentLevelCRC();

	// Plane cockpit cutscene checks if we have canteen.
	if ( currentAreaCRC == levelCRC::PLANE_COCKPIT ) {
		PopScriptVariable_Int( context );
		int* out = GetOutVariable( context );

		*out = 0;
		Gizmod::getInstance()->getSaveManager()->getNativeManager()->GetVar( levelCRC::PLANE_COCKPIT, 0x07BC40E8, "numTimesPlayed", out, true );
	}
	// Native Village's shield cutscene will be gone if we enter with shield already.
	else if ( currentAreaCRC == levelCRC::NATIVE_VILLAGE ) {
		PopScriptVariable_Int( context );
		int* out = GetOutVariable( context );

		*out = 0;
		Gizmod::getInstance()->getSaveManager()->getNativeManager()->GetVar( levelCRC::NATIVE_VILLAGE, 0x07BC40E8, "numTimesPlayed", out, false );
	}
	else {
		Script_HarryIsInInventory( context );
	}
}

// Patching icon and text for randomized shaman shop.
GET_METHOD( 0x640760, void, EUIObjectNode_SetFlags, EUIObjectNode*, uint32_t, bool );
GET_METHOD( 0x5BD650, void, EUIDDStoreItem_InitItem, EUIDDStoreItem*, int, int );
static void __stdcall _EUIDDStoreItem_InitItem_Custom( EUIDDStoreItem* self, int p2, int p3 )
{
	char* storeData = (char*) self->m_storeResourcePtr;
	int slot = *(storeData + 0x2C);

	// Find mapping.
	UnlockableType origType;
	EHeroicSkill::Enum origSkill;
	InventoryItem::Notes origNote;

	switch ( slot )
	{
	case 2: origType = NOTES; origNote = InventoryItem::JUNGLE_NOTES; break;
	case 3: origType = NOTES; origNote = InventoryItem::NATIVE_NOTES; break;
	case 4: origType = NOTES; origNote = InventoryItem::CAVERN_NOTES; break;
	case 5: origType = NOTES; origNote = InventoryItem::MOUNTAIN_NOTES; break;

	case 6: origType = HEROIC_SKILL; origSkill = EHeroicSkill::SMASH_STRIKE; break;
	case 7: origType = HEROIC_SKILL; origSkill = EHeroicSkill::SUPER_SLING; break;
	case 8: origType = HEROIC_SKILL; origSkill = EHeroicSkill::BREAKDANCE; break;
	default:
		EUIDDStoreItem_InitItem( self, p2, p3 );
		return;
	}

	Unlockable* mapped = nullptr;
	if ( origType == HEROIC_SKILL ) {
		mapped = mapUnlockable(origType, origSkill);
	} 
	else if ( origType == NOTES ) {
		mapped = mapUnlockable(origType, origNote);
	}

	// Icon.
	*((uint32_t*) (storeData + 0x20)) = mapped->getShaderIconCRC();

	// Let it initialize.
	EUIDDStoreItem_InitItem( self, p2, p3 );

	// Text.
	*self->m_text = (wchar_t*) mapped->m_displayNameW;

	// Make our own checks for disabling items we already purchased.
	EUIObjectNode_SetFlags( self, 0x14, true );    // Force all items on.
	if ( mapped->isUnlocked() ) {
		EUIObjectNode_SetFlags( self, 0x14, false );
	}
}
MAKE_THISCALL_WRAPPER( EUIDDStoreItem_InitItem_Custom, _EUIDDStoreItem_InitItem_Custom );


// NOTE : Shaman purchase event may be useless here, as we want to capture ALL heroic abilities, some of which are unlocked with cutscenes!!
class ItemRandoListener
	: public ICollectItemListener
	, public ICollectIdolListener
	, public IHeroicUnlockListener
	, public IShamanPurchaseListener
{
public:

	void init( EventListener* el )
	{
		el->registerEvent<CollectItemEvent>( &g_itemRandoListener );
		el->registerEvent<CollectIdolEvent>( &g_itemRandoListener );
		el->registerEvent<HeroicUnlockEvent>( &g_itemRandoListener );
		el->registerEvent<ShamanPurchaseEvent>( &g_itemRandoListener );
	}

	virtual void onCollectItem( CollectItemEvent& event ) override
	{
		collectItem( event );
	}

	virtual void onCollectIdol( CollectIdolEvent& event ) override
	{
		bool overriden = collectIdol( event.getEntity(), Gizmod::getCurrentLevelCRC(), event.getAmount() );
		if ( overriden ) {
			event.setCancelled( true );
		}
	}

	virtual void onHeroicUnlock( HeroicUnlockEvent& event ) override
	{
		Unlockable* u = 0;

		// Insta dash causes stink bomb to be randomized, we must map the check manually.
		if ( rando_config.instaDash && event.getSkill() == EHeroicSkill::HEROIC_DASH ) {
			u = mapUnlockable( INVENTORY_ITEM, 0x55A51DAB );
		}
		else {
			u = mapUnlockable( HEROIC_SKILL, event.getSkill() );
		}

		if ( u ) {
			event.setCancelled( true );
			u->grant();
		}
	}

	virtual void onShamanPurchase( ShamanPurchaseEvent& event ) override
	{
		ShamanShop::PriceSlot item = event.getItem();
		Unlockable* u = 0;

		if (rando_config.itemRandoNotesSlots)
		{
			switch ( item )
			{
			case ShamanShop::NATIVE_NOTES:    u = mapUnlockable( NOTES, InventoryItem::NATIVE_NOTES ); break;
			case ShamanShop::CAVERN_NOTES:    u = mapUnlockable( NOTES, InventoryItem::CAVERN_NOTES ); break;
			case ShamanShop::MOUNTAIN_NOTES:  u = mapUnlockable( NOTES, InventoryItem::MOUNTAIN_NOTES ); break;
			case ShamanShop::JUNGLE_NOTES:    u = mapUnlockable( NOTES, InventoryItem::JUNGLE_NOTES ); break;
			}
			
			if ( u ) {
				// TODO: still gives the notes as event isn't cancelled
				u->grant();
				Gizmod::getInstance()->getLogger()->log_printf( "Purchased Shaman Notes, Replaced with %s\n", u->m_displayName);
			}
			}
		
	}
}
g_itemRandoListener;


//
// Initialization.
//

static inline bool isItemRandoActive()
{
	return
		rando_config.itemRandoInventory ||
		rando_config.itemRandoIdols ||
		rando_config.itemRandoExplorers ||
		rando_config.itemRandoHeroicSkills ||
		rando_config.itemRandoHarryActions;
}

static void createUnlockables()
{
	/* Inventory items. */
	if ( rando_config.itemRandoInventory ) {
		/*for ( int i = 0; i < _countof(g_unlockableItems); i++ ) {
			original.push_back( g_unlockableItems + i );
		}*/
	}

	/* Regular (silver) idols. */
	if ( rando_config.itemRandoIdols ) {
		g_unlockableIdols.reserve( 100 );
		for ( auto& idolsList : idols_info ) {
			// St.Claire's camp : Don't randomize anything!
			if ( idolsList.first == levelCRC::ST_CLAIRE_EXCAVATION_CAMP_DAY || idolsList.first == levelCRC::ST_CLAIRE_EXCAVATION_CAMP_NIGHT ) continue;

			for ( const Idol& idol : idolsList.second ) {
				g_unlockableIdols.emplace_back( IDOL_SINGLE, (uint32_t) &idol, NAME("Idol") );
			}
		}
	}

	/* Explorer (golden) idols. */
	if ( rando_config.itemRandoExplorers ) {
		g_unlockableExplorers.reserve( 15 );
		for ( const auto& explorer : explorer_info ) {
			// St.Claire's camp : Don't randomize anything!
			if ( explorer.first == levelCRC::ST_CLAIRE_EXCAVATION_CAMP_DAY || explorer.first == levelCRC::ST_CLAIRE_EXCAVATION_CAMP_NIGHT ) continue;

			g_unlockableExplorers.emplace_back( IDOL_EXPLORER, (uint32_t) &explorer.second, NAME("Explorer") );
		}
	}

	/* Heroic skills (cutscenes or shaman). */
	if ( rando_config.itemRandoHeroicSkills ) {
		g_unlockableHeroicSkills.emplace_back( HEROIC_SKILL,       EHeroicSkill::RISING_STRIKE, NAME("Rising Strike") );
		g_unlockableHeroicSkills.emplace_back( HEROIC_SKILL,       EHeroicSkill::SMASH_STRIKE,  NAME("Smash Strike") );
		if ( rando_config.instaDash ) {
			g_unlockableHeroicSkills.emplace_back( INVENTORY_ITEM, 0x55A51DAB,                  NAME("Stink Bomb") );
		}
		else {
			g_unlockableHeroicSkills.emplace_back( HEROIC_SKILL,   EHeroicSkill::HEROIC_DASH,   NAME("Heroic Dash") );
		}
		g_unlockableHeroicSkills.emplace_back( HEROIC_SKILL,       EHeroicSkill::HEROIC_DIVE,   NAME("Heroic Dive") );
		g_unlockableHeroicSkills.emplace_back( HEROIC_SKILL,       EHeroicSkill::BREAKDANCE,    NAME("Breakdance") );
		g_unlockableHeroicSkills.emplace_back( HEROIC_SKILL,       EHeroicSkill::SUPER_SLING,   NAME("Super Sling") );
	}

	// Notes
	g_unlockableNotes.emplace_back( NOTES,       InventoryItem::JUNGLE_NOTES, NAME("Jungle Notes") );
	g_unlockableNotes.emplace_back( NOTES,       InventoryItem::NATIVE_NOTES, NAME("Native Notes") );
	g_unlockableNotes.emplace_back( NOTES,       InventoryItem::CAVERN_NOTES, NAME("Cavern Notes") );
	g_unlockableNotes.emplace_back( NOTES,       InventoryItem::MOUNTAIN_NOTES, NAME("Mountain Notes") );




	/* Base moves (punch, roll, sneak). */
	if ( rando_config.itemRandoHarryActions ) {
		// Stage actions.
		// Note : randomizing jump is NOT viable. Turns out jumping is pretty much mandatory in a platformer game...
		EHarryActions::Enum actions[] = { EHarryActions::ATTACK, EHarryActions::CROUCH, EHarryActions::SNEAK };
		const char* actionNames[] = { "Punch", "Rolling", "Sneak" };
		const wchar_t* actionNamesW[] = { L"Punch", L"Rolling", L"Sneak" };
		for ( int i = 0; i < _countof(actions); i++ ) {
			g_unlockableHarryActions.emplace_back( HARRY_ACTION, actions[i], actionNames[i], actionNamesW[i] );
		}
	}
}

static void itemRandoCodeInjection()
{
	// Misc.
	injector::MakeRangedNOP( 0x4E9D6F, 0x4E9D83 );                        // Remove hotbar autoset for the first 4 items.
	injector::WriteMemory( 0x8EF35C, Script_HarryIsInInventory_custom );  // Disable Plane Cockpit's check for canteen.
	injector::MakeNOP( 0x52EC98, 6 );                                     // Super Sling's "you must have sling before purchasing" check.

	// Visuals patching (quality of life).
	injector::MakeCALL( 0x5973E9, EITreasureIdol_InitValues_custom );     // Set correct model on EITreasureIdol instances (regular idols).
	injector::MakeCALL( 0x59810D, EITreasureIdol_InitValues_custom );     // Set correct model on EITreasureIdol instances (explorer idols).
	injector::WriteMemory( 0x89E708, EUIDDStoreItem_InitItem_Custom );    // Set icon and text in shaman shop items.

	// Getting Raft from unintended locations.
	injector::MakeRangedNOP( 0x4E2EDE, 0x4E2EF4 );   // Cavern Lake to Jungle Canyon.
	injector::MakeRangedNOP( 0x4E3C33, 0x4E3C49 );   // Mountain Sled Run.
}

static void stageUnlockablesCategory( std::vector<Unlockable*>& dst, std::vector<Unlockable>& src )
{
	for ( Unlockable& u : src ) dst.push_back( &u );
}

void item_rando_init()
{
	// Stage item categories and shuffle them.
	std::vector<Unlockable*> original;
	std::vector<Unlockable*> shuffled;

	// No item rando option is on, so we don't need to enable anything here.
	if ( !isItemRandoActive() ) {
		return;
	}

	createUnlockables();

	// Shuffle and assign.
	// - We map the shaman shop first to avoid mapping idols to... more idols.
	// - Then process what is left in the pool.
	EHarryActions::enableAll();
	if ( rando_config.itemRandoHeroicSkills /* || rando_config.itemRandoNotes */ ) {
		if ( rando_config.itemRandoInventory ) for ( int i = 0; i < _countof(g_unlockableItems); i++ ) { original.push_back( g_unlockableItems + i ); }
		if ( rando_config.itemRandoHeroicSkills ) stageUnlockablesCategory( original, g_unlockableHeroicSkills );
		if ( rando_config.itemRandoHarryActions ) stageUnlockablesCategory( original, g_unlockableHarryActions );
		if ( rando_config.itemRandoNotesSlots ) stageUnlockablesCategory( original, g_unlockableNotes );

		shuffled = original;
		std::random_shuffle( shuffled.begin(), shuffled.end() );
		

		int shamanSlots = 3;

		// Map the 3 shaman skills.
		g_unlockablesMap.emplace( &g_unlockableHeroicSkills[1], shuffled[shuffled.size() - 1] );
		g_unlockablesMap.emplace( &g_unlockableHeroicSkills[4], shuffled[shuffled.size() - 2] );
		g_unlockablesMap.emplace( &g_unlockableHeroicSkills[5], shuffled[shuffled.size() - 3] );


		// Map the 4 notes.
		if (rando_config.itemRandoNotesSlots) {
			shamanSlots += 4;
			g_unlockablesMap.emplace( &g_unlockableNotes[0], shuffled[shuffled.size() - 4] );
			g_unlockablesMap.emplace( &g_unlockableNotes[1], shuffled[shuffled.size() - 5] );
			g_unlockablesMap.emplace( &g_unlockableNotes[2], shuffled[shuffled.size() - 6] );
			g_unlockablesMap.emplace( &g_unlockableNotes[3], shuffled[shuffled.size() - 7] );
		}


		// Unstage randomized shaman items manually.
		original.erase( std::remove(original.begin(), original.end(), &g_unlockableHeroicSkills[1]) );
		original.erase( std::remove(original.begin(), original.end(), &g_unlockableHeroicSkills[4]) );
		original.erase( std::remove(original.begin(), original.end(), &g_unlockableHeroicSkills[5]) );
		original.erase( std::remove(original.begin(), original.end(), &g_unlockableNotes[0]) );
		original.erase( std::remove(original.begin(), original.end(), &g_unlockableNotes[1]) );
		original.erase( std::remove(original.begin(), original.end(), &g_unlockableNotes[2]) );
		original.erase( std::remove(original.begin(), original.end(), &g_unlockableNotes[3]) );



		for ( size_t i = shuffled.size() - 1; i > shuffled.size() - shamanSlots - 1; i-- ) {
			const UnlockableType shType = shuffled[i]->m_type;
			if ( shType == HARRY_ACTION ) {
				EHarryActions::disableAction( shuffled[i]->m_harryAction );
			}
		}
		
		shuffled.erase( shuffled.end() - shamanSlots, shuffled.end() );

		// Prepare to randomize the rest.
		stageUnlockablesCategory( original, g_unlockableIdols );
		stageUnlockablesCategory( shuffled, g_unlockableIdols );
		stageUnlockablesCategory( original, g_unlockableExplorers );
		stageUnlockablesCategory( shuffled, g_unlockableExplorers );
	}
	else {
		if ( rando_config.itemRandoInventory ) for ( int i = 0; i < _countof(g_unlockableItems); i++ ) { original.push_back( g_unlockableItems + i ); }
		if ( rando_config.itemRandoHeroicSkills ) stageUnlockablesCategory( original, g_unlockableHeroicSkills );
		if ( rando_config.itemRandoHarryActions ) stageUnlockablesCategory( original, g_unlockableHarryActions );
		stageUnlockablesCategory( original, g_unlockableIdols );
		stageUnlockablesCategory( original, g_unlockableExplorers );

		shuffled = original;
	}

	std::random_shuffle( shuffled.begin(), shuffled.end() );


	for ( size_t i = 0; i < original.size(); i++ ) {
		g_unlockablesMap.emplace( original[i], shuffled[i] );

		const UnlockableType ogType = original[i]->m_type, shType = shuffled[i]->m_type;

		switch ( shType )
		{
		case INVENTORY_ITEM:
			g_itemLocations.emplace( shuffled[i]->m_itemHash, original[i]->getDefaultLocationCRC() );
			break;

		case HARRY_ACTION:
			// Make sure we disable those moves.
			EHarryActions::disableAction( shuffled[i]->m_harryAction );
			break;
		}
	}

	// Cache abilities to reapply in case the game rewrites them.
	g_itemRando_harryActions = EHarryActions::getActionsBitfield();


	g_itemRandoListener.init( Gizmod::getInstance()->getEventListener() );

	itemRandoCodeInjection();

	Gizmod::getInstance()->getLogger()->log( "Initialized item rando.\n" );
}


void log_item_rando( std::ostream& os )
{
	if ( g_unlockablesMap.empty() ) return;

	const int spacePad = 13;

	os << "Item Rando :\n";
	for ( const auto& p : g_unlockablesMap ) {
		Unlockable* original = p.first;
		Unlockable* shuffled = p.second;
		const UnlockableType ogType = original->m_type, shType = shuffled->m_type;
		const bool ogIsShaman = original->m_type == UnlockableType::HEROIC_SKILL && (original->m_heroicSkill == EHeroicSkill::SMASH_STRIKE || original->m_heroicSkill == EHeroicSkill::SUPER_SLING || original->m_heroicSkill == EHeroicSkill::BREAKDANCE) || (original->m_type == UnlockableType::NOTES);

		if ( shType == INVENTORY_ITEM || shType == HARRY_ACTION || shType == HEROIC_SKILL || shType == NOTES ) {
			os << " - " << shuffled->m_displayName;
			int pad = spacePad - strlen(shuffled->m_displayName);
			for ( ; pad > 0; pad-- ) os << ' ';

			if ( ogIsShaman ) {
				os << " from   Shaman (" << original->m_displayName << ")";
			}
			else {
				os << " from   " << original->m_displayName;
				pad = spacePad - strlen(original->m_displayName);
				for ( ; pad > 0; pad-- ) os << ' ';
				os << " -->   " << level_get_name(level_get_by_crc(original->getDefaultLocationCRC()));
			}

			os << '\n';
		}
	}
	os << '\n';
}

void get_item_locations( uint32_t (&out)[8] )
{
	if ( g_itemLocations.empty() ) {
		for ( int i = 0; i < _countof(g_unlockableItems); i++ ) {
			out[i] = get_item_default_location( g_unlockableItems[i].m_itemHash );
		}
	}
	else {
		for ( int i = 0; i < _countof(g_unlockableItems); i++ ) {
			out[i] = g_itemLocations[g_unlockableItems[i].m_itemHash];
		}
	}
}
