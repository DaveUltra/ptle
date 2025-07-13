#include "CollectItemEvent.h"

#include "gizmod/Gizmod.h"


CollectItemEvent::CollectItemEvent( uint32_t itemHash )
	: m_item( InventoryItem::getItemByHash(itemHash) )
{

}

InventoryItem* CollectItemEvent::getItem() const
{
	return Gizmod::getInstance()->getInventoryItem( m_item );
}
