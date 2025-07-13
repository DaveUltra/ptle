#pragma once

#include "gizmod/types/Projectile.h"

#include "ptle/math/Vector3f.h"
#include "ptle/types/types.h"

#include <stdint.h>
#include <vector>

class EInstance;
class EIProjectile;
class EICharacter;


class GizmodWorld
{
public:

	void getAllEntities( std::vector<EInstance*>& out ) const;

	void getEntitiesOfType( std::vector<EInstance*>& out, const ETypeInfo* type ) const;

	template<class T>
	inline void getEntitiesOfType( std::vector<T*>& out, const ETypeInfo* type ) const;

	EIProjectile* spawnProjectile( ProjectileType::Enum type, const Vector3f& position, const Vector3f& direction, EICharacter* source, float shotAccuracy );
};



template<class T>
inline void GizmodWorld::getEntitiesOfType( std::vector<T*>& out, const ETypeInfo* type ) const
{
	this->getEntitiesOfType( reinterpret_cast<std::vector<EInstance*>&>(out), type );
}
