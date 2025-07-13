#pragma once

#include "Event.h"

#include "gizmod/shaman/ShamanShop.h"


DECLARE_EVENT( ShamanPurchase );

class ShamanPurchaseEvent : public Event<IShamanPurchaseListener>
{
public:

	ShamanPurchaseEvent( ShamanShop::PriceSlot item )
		: m_item(item)
	{
	}

	inline ShamanShop::PriceSlot getItem() const { return m_item; }

	inline int getPrice() const { return ShamanShop::getPrice( m_item ); }

	bool hasEnoughIdols() const;


private:

	ShamanShop::PriceSlot m_item;
};
