#pragma once

#include "EICharacter.h"

#include "math/Vector3f.h"


struct EState
{
	uint32_t m_id;
	const char* m_name;
	int m_animID;
	int sound;
	void* f0;
	void* f1;
	void* f2;
	int unkn;
	void* f3;
	void* f4;
	void* f5;
	int unkn1;
};

class EIPlayer : public EICharacter
{
public:

	// TODO : replace by a matrix4.
	Vector4f m_mat[3];
	Vector4f m_positionPlayer;

	char padding4[0x2E0];

	Vector3f unknown1;
	Vector3f unknown2;
	Vector3f unknown3;

	int m_healthMax;
	int m_healthValue;

	char padding8[0x118];

	Vector3f m_headRotation;
	Vector3f m_headTargetRotation;

	char padding5[0xD4];

	Matrix4f m_rigidBodyMatrix;

	char padding6[0xBC];

	void** m_itemHotbar;

	char padding9[0x1F0];



	virtual void SetSomeImportantValue(void*);
	virtual void func_unknown60(bool);
	virtual void PlayerInit(Vector3f* position, float orientation);
	virtual void func_unknown61();
	virtual void func_unknown62(Matrix4f*);
	virtual uint32_t func_unknown63();
	virtual float func_unknown64();
	virtual float func_unknown65();
	virtual Vector3f* func_unknown66(Vector3f*);
	virtual void func_unknown67(Matrix4f*);
	virtual void func_unknown68(bool);
	virtual void SetOrientationAngle(float);
	virtual void func_unknown69(bool);
	virtual bool func_unknown70();
	virtual void func_unknown71(bool);
	virtual void func_unknown72(Vector3f*, bool, float);
	virtual void Teleport(Matrix4f*, bool, float);
	virtual void func_unknown73(Matrix4f*);
	virtual void func_unknown74(int);
	virtual void func_unknown75(int);
	virtual void func_unknown76(int, int, int);
	virtual void PerformStateSwitch(int stateIndex, bool);
	virtual void func_unknown77(Vector3f*, Vector3f*);
	virtual int GetPreviousState();
	virtual int GetCurrentState();
	virtual int GetStateByName(const char*);
	virtual void SetTargetState(int);
	virtual int func_unknown78();
	virtual void func_unknown79(int);
	virtual float func_unknown80();
	virtual void func_unknown81(Vector3f*);
	virtual void func_unknown82();
	virtual void func_unknown83(EIBeast*);
	virtual void func_unknown84(void*);
	virtual EIBeast* func_unknown85();
	virtual void AddHealth(int);
	virtual void Damage(float);
	virtual int GetHealth();
	virtual void SetHealth(int);
	virtual int GetMaxHealth();
	virtual void func_unknown86();
	virtual Vector3f* func_unknown87(Vector3f*, bool);
	virtual void func_unknown88(Vector3f*);
	virtual void func_unknown89(void*);
	virtual EState* GetState(int stateIndex);
};
