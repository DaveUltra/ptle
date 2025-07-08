#pragma once

#include "Event.h"

#include "gizmod/item/InventoryItem.h"

#include <stdint.h>


DECLARE_EVENT( CollectItem );

class CollectItemEvent : public Event<ICollectItemListener>
{
public:

	CollectItemEvent( uint32_t itemHash );

	InventoryItem* getItem() const;

	inline uint32_t getItemHash() const { return this->getItem()->getHash(); }


private:

	InventoryItem::Item m_item;
};
