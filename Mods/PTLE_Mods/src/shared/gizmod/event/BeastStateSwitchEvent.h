#pragma once

#include "Event.h"

#include "ptle/EIBeast.h"
#include "ptle/EBeastStateMachine.h"

#include <string>

class EIBeast;


DECLARE_EVENT( BeastStateSwitch );

class BeastStateSwitchEvent : public Event<IBeastStateSwitchListener>
{
public:

	BeastStateSwitchEvent( EIBeast* entity, int stateIndex )
		: m_beast( entity ), m_stateIndex( stateIndex )
	{
	}

	inline EIBeast* getBeast() const { return m_beast; }

	inline const EBeastState* getBeastState() const { return &m_beast->GetStateMachine()->m_states[m_stateIndex]; }


private:

	EIBeast* m_beast;
	int m_stateIndex;
};
