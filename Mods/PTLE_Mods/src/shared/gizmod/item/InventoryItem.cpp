#include "InventoryItem.h"

#include "gizmod/Gizmod.h"
#include "gizmod/save/SaveManager.h"

#include "ptle/EIHarry.h"

#include "utils/func.h"


GET_FUNC( 0x5E36C0, uint32_t, GetHarrySaveGameVariable, int );


static int getNotesSaveGameVarID( InventoryItem::Notes notes )
{
	switch (notes)
	{
	case InventoryItem::JUNGLE_NOTES: return 0x25;
	case InventoryItem::NATIVE_NOTES: return 0x26;
	case InventoryItem::CAVERN_NOTES: return 0x28;
	case InventoryItem::MOUNTAIN_NOTES: return 0x27;
	default: return -1;
	}
}

bool InventoryItem::isNotesUnlocked( Notes notes )
{
	int id = getNotesSaveGameVarID( notes );
	if ( id == -1 ) {
		return false;
	}

	int val;
	uint32_t hash = GetHarrySaveGameVariable(id);
	Gizmod::getInstance()->getSaveManager()->getNativeManager()->GetVar( 0, hash, &val );
	return val != 0;
}

void InventoryItem::setNotesUnlocked( Notes notes, bool b )
{
	int id = getNotesSaveGameVarID( notes );
	if ( id == -1 ) {
		return;
	}

	uint32_t hash = GetHarrySaveGameVariable(id);
	Gizmod::getInstance()->getSaveManager()->getNativeManager()->SetVar( 0, hash, b );
}

bool InventoryItem::setItemInHand( Item item )
{
	EIHarry* harry = Gizmod::getHarry();
	if ( !harry ) {
		return false;
	}

	ItemStruct* itemStruct = Gizmod::getInstance()->getInventoryItem( item )->m_itemStruct;
	harry->m_itemHotbar[5] = itemStruct->m_object;
	return true;
}

bool InventoryItem::setItemInHotbar( int slotID, Item item )
{
	if ( slotID < 0 || slotID >= 4 ) return false;

	EIHarry* harry = Gizmod::getHarry();
	if ( !harry ) {
		return false;
	}

	if ( item == Item::UNKNOWN ) {
		harry->m_itemHotbar[slotID] = 0;
	}
	else {
		ItemStruct* itemStruct = Gizmod::getInstance()->getInventoryItem( item )->m_itemStruct;
		harry->m_itemHotbar[slotID] = itemStruct->m_object;
	}
	return true;
}
