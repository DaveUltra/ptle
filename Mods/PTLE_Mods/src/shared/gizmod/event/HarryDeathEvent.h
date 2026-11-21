#pragma once

#include "Event.h"

class EIHarry;


DECLARE_EVENT( HarryDeath );

class HarryDeathEvent : public Event<IHarryDeathListener>
{
public:

	HarryDeathEvent( EIHarry* harry )
		: m_harry( harry )
	{
	}

	EIHarry* getHarry() const { return m_harry; }


private:

		// Storing pointer to Harry in case multiple Harrys exist.
	EIHarry* m_harry;

	// TODO: death source (drowning, enemy attack, quicksand...)
};
