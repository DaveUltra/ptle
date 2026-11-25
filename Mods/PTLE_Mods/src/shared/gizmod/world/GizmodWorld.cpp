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

	THashMap<uint32_t, EInstance*>& entitiesByID = level->m_entitiesByID;

	out.reserve( entitiesByID.m_size );
	for ( THashMap<uint32_t, EInstance*>::Entry* e = entitiesByID.m_iterateFirst; e != 0; e = e->m_iterateNext ) {
		EInstance* inst = e->m_value;
		if ( !inst ) continue;

		out.push_back( inst );
	}
}

void GizmodWorld::getEntitiesOfType( std::vector<EInstance*>& out, const ETypeInfo* type ) const
{
	ERLevel* level = Gizmod::getLevel();
	if ( !level ) {
		return;
	}

	THashMap<uint32_t, EInstance*>& entitiesByID = level->m_entitiesByID;

	int i = entitiesByID.m_size;
	for ( THashMap<uint32_t, EInstance*>::Entry* e = entitiesByID.m_iterateFirst; i != 0; e = e->m_iterateNext, i-- ) {
		EInstance* inst = e->m_value;
		if ( !inst ) continue;

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
