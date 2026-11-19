#pragma once

#include "Event.h"

#include "ptle/math/Vector3f.h"

class EInstance;
class EIHarry;


DECLARE_EVENT( HarryDamage );

class HarryDamageEvent : public Event<IHarryDamageListener>
{
public:

	HarryDamageEvent( EIHarry* harry, EInstance* sourceInstance, float damage, uint32_t flags, const Vector3f& direction, float force, int p7 )
		: m_harry( harry )
		, m_sourceInstance( sourceInstance ), m_damage( damage ), m_flags( flags ), m_direction( direction ), m_force( force ), m_unknown( p7 )
	{
	}

	EIHarry* getHarry() const { return m_harry; }

	EInstance* getSourceInstance() const { return m_sourceInstance; }
	float getDamage() const { return m_damage; }
	float getForce() const { return m_force; }

	void setDamage( float f ) { m_damage = f; }
	void setForce( float f ) { m_force = f; }


private:

	// Storing pointer to Harry in case multiple Harrys exist.
	EIHarry* m_harry;

	EInstance* m_sourceInstance;
	float m_damage;
	uint32_t m_flags;
	Vector3f m_direction;
	float m_force;
	int m_unknown;
};
