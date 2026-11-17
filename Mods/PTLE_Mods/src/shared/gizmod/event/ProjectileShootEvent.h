#pragma once

#include "Event.h"

#include "gizmod/types/Projectile.h"

#include "ptle/EIProjectile.h"
#include "ptle/math/Vector3f.h"

class EInstance;
class EIProjectile;


DECLARE_EVENT( ProjectileShoot );

class ProjectileShootEvent : public Event<IProjectileShootListener>
{
public:

	ProjectileShootEvent( EIProjectile* projectile, EInstance* source, const Vector3f& pos, const Vector3f& dir )
		: m_projectile( projectile ), m_source( source ), m_position( pos ), m_direction( dir )
	{
		m_projectileTypeCRC = (ProjectileType::Enum) projectile->m_projectileTypeInfo->m_typeID;
	}

	inline ProjectileType::Enum getProjectileTypeCRC() const { return m_projectileTypeCRC; }

	inline EIProjectile* getProjectile() const { return m_projectile; }
	inline EInstance* getSource() const { return m_source; }

	inline const Vector3f& getPosition() const { return m_position; }
	inline const Vector3f& getDirection() const { return m_direction; }
	inline void setPosition( const Vector3f& p ) { m_position = p; }
	inline void setDirection( const Vector3f& d ) { m_direction = d; }


private:

	ProjectileType::Enum m_projectileTypeCRC;

	EIProjectile* m_projectile;
	EInstance* m_source;

	Vector3f m_position;
	Vector3f m_direction;
};
