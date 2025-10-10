#pragma once

#include "Event.h"

class EInstance;


DECLARE_EVENT( BreakableBreak );

class BreakableBreakEvent : public Event<IBreakableBreakListener>
{
public:

	BreakableBreakEvent( EInstance* breakable )
		: m_breakable( breakable )
	{
	}

	inline EInstance* getBreakable() const { return m_breakable; }


private:

	EInstance* m_breakable;
};
