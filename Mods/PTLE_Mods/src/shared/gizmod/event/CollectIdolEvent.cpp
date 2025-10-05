#include "CollectIdolEvent.h"


CollectIdolEvent::CollectIdolEvent( EITreasureIdol* entity, int amount )
	: m_entity( entity ), m_amount( amount )
{

}

void CollectIdolEvent::setAmount( int amount )
{
	m_amount = amount;
}
