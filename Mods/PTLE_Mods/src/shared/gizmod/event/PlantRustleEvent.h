#pragma once

#include "Event.h"

class EIPlant;


DECLARE_EVENT( PlantRustle );

class PlantRustleEvent : public Event<IPlantRustleListener>
{
public:

	PlantRustleEvent( EIPlant* entity )
		: m_entity( entity )
	{
	}

	inline EIPlant* getEntity() const { return m_entity; }


private:

	EIPlant* m_entity;
};
