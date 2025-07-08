#include "ShamanPurchaseEvent.h"

#include "gizmod/Gizmod.h"

#include "ptle/EIHarry.h"


bool ShamanPurchaseEvent::hasEnoughIdols() const
{
	return Gizmod::getHarry()->m_idolsCollected >= this->getPrice();
}
