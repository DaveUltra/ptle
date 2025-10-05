#pragma once

#include "Event.h"

#include <stdint.h>

class EITreasureIdol;


DECLARE_EVENT( CollectIdol );

class CollectIdolEvent : public Event<ICollectIdolListener>
{
public:

	CollectIdolEvent( EITreasureIdol* entity, int amount );

	inline EITreasureIdol* getEntity() const { return m_entity; }

	inline int getAmount() const { return m_amount; }

	void setAmount( int amount );


private:

	EITreasureIdol* m_entity;
	int m_amount;
};
