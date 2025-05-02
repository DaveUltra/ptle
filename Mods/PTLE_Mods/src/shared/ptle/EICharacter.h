#pragma once

#include "EIGameInstance.h"

#include "EAnimController.h"

#include "math/Matrix4f.h"
#include "math/Vector4f.h"
#include "math/Vector3f.h"

class ERCharacter;
class ERModel;


class EICharacter : public EIGameInstance
{
public:

	EAnimController m_animController;

	/*-----*/ char padding4[0x4];

	bool m_onGround;
	Vector4f unused4;
	int unused5;
	
	uint32_t m_modelCRC;
	Vector3f m_position;
	Vector3f m_rotationEuler;
	Vector3f m_scale;

	int unused7;
	int unused8;

	Matrix4f m_transformMatrix;
	Vector3f unknown;

	char padding5[0x8];

	ERModel* m_model;

	/*-----*/ char padding6[0x78];

	uint32_t m_collisionFilter;

	/*-----*/ char padding7[0xBC];



	virtual void SetPositionAndRotation(Vector3f* pos, Vector3f* rot);
	virtual void func_unknown50();
	virtual void func_unknown51();
	virtual void func_unknown52();
	virtual void SyncTransformMatrix();
	virtual void UpdateBounds();
	virtual uint32_t func_unknown53();
	virtual uint32_t func_unknown54();
	virtual bool func_unknown55();
	virtual Vector3f* func_unknown56(Vector3f*);
	virtual void* func_unknown57();
	virtual void func_unknown58();
	virtual void func_unknown59();
};
