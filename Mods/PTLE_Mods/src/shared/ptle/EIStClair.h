#pragma once

#include "EIBeast.h"

#include "math/Matrix4f.h"

class EIProjectile;
class EIStaticModel;


class EIStClair : public EIBeast
{
public:

		/// Hand bones and matrices, used to correctly position the spawned TNTs.
	uint32_t m_boneTNT_R;
	Matrix4f m_boneMatrix_R;
	uint32_t m_boneTNT_L;
	Matrix4f m_boneMatrix_L;

		/// Projectile type ID.
	char* m_projectileQuickdatasPath;
	char* m_projectileQuickdatasName;
	uint32_t m_projectileType;

		/// Projectile instances and animation values.
	EIProjectile* m_projectile_R;
	bool m_threwTNT_R;
	bool m_loadedTNT_R;
	EIProjectile* m_projectile_L;
	bool m_threwTNT_L;
	bool m_loadedTNT_L;

		/// Static models used to display TNTs in St.Claire's hands before throwing.
	EIStaticModel* m_staticModel_R;
	EIStaticModel* m_staticModel_L;

	bool unknown7;

	uint32_t m_modelCRC0;
	uint32_t m_modelCRC1;

		/// Current phase (0, 1, 2, 3).
	int m_bossPhase;

	Vector4f unknown8;
	Vector4f unknown9;

		/// Is St.Claire throwing one or two TNTs?
		/// - in MoI, he'll throw one then two at phase 3 and 4
		/// - in Crystal Cavern, he always throws two.
	bool m_throwingTwoTNT;

	char padding12[0xB];
};
