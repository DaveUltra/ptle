#include "gizmod/Gizmod.h"
#include "GizmodWorld.h"

#include "utils/func.h"

#include "ptle/ERLevel.h"
#include "ptle/EICharacter.h"
#include "ptle/EIProjectile.h"

#include "ptle/containers/HashMap/HashMap.h"


GET_FUNC( 0x557840, EIProjectile*, InstantiateProjectile, uint32_t );

GET_METHOD( 0x559640, void, EIProjectile_Throw, EIProjectile*, const Vector3f*, const Vector3f*, EICharacter*, float );


void GizmodWorld::getAllEntities( std::vector<EInstance*>& out ) const
{
	ERLevel* level = Gizmod::getLevel();
	if ( !level ) {
		return;
	}

	HashMap& entitiesByID = level->m_entitiesByID;

	out.reserve( entitiesByID.m_size );
	for ( HashMapEntry* e = entitiesByID.m_iterateFirst; e != 0; e = e->m_iterateNext ) {
		if ( !e->m_ptr ) {
			continue;
		}

		EInstance* inst = reinterpret_cast<EInstance*>(e->m_ptr);
		out.push_back( inst );
	}
}

void GizmodWorld::getEntitiesOfType( std::vector<EInstance*>& out, const ETypeInfo* type ) const
{
	ERLevel* level = Gizmod::getLevel();
	if ( !level ) {
		return;
	}

	HashMap& entitiesByID = level->m_entitiesByID;

	for ( HashMapEntry* e = entitiesByID.m_iterateFirst; e != 0; e = e->m_iterateNext ) {
		if ( !e->m_ptr ) {
			continue;
		}

		EInstance* inst = reinterpret_cast<EInstance*>(e->m_ptr);
		if ( is_type_derived_from(inst->GetType(), type) ) {
			out.push_back( inst );
		}
	}
}

EIProjectile* GizmodWorld::spawnProjectile( ProjectileType::Enum type, const Vector3f& position, const Vector3f& direction, EICharacter* source, float shotAccuracy )
{
	EIProjectile* projectile = InstantiateProjectile( type );
	EIProjectile_Throw( projectile, &position, &direction, source, shotAccuracy );
	return projectile;
}
