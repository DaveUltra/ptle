#pragma once

#include "Event.h"

class EInstance;


DECLARE_EVENT( EntitySpawn );

class EntitySpawnEvent : public Event<IEntitySpawnListener>
{
public:

	EntitySpawnEvent( EInstance* in )
		: m_entity( in )
	{
	}

	EInstance* getEntity() const { return m_entity; }


private:

	EInstance* m_entity;
};
