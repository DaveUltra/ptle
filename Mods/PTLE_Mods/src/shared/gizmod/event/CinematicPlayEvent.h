#pragma once

#include "Event.h"

class EICinematic;


DECLARE_EVENT( CinematicPlay );

class CinematicPlayEvent : public Event<ICinematicPlayListener>
{
public:

	CinematicPlayEvent( EICinematic* entity )
		: m_entity( entity )
	{
	}

	inline EICinematic* getEntity() const { return m_entity; }


private:

	EICinematic* m_entity;
};
