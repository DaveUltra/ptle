#pragma once

#include "EInstance.h"


class EIGameInstance : public EInstance
{
public:

	char padding2[0x50];


	virtual void GetOrientationAngle(float*);
	virtual float func_unknown27();
	virtual Vector3f* func_unknown28(Vector3f*);
	virtual void func_unknown29(Vector3f*);
	virtual void GetBoneMatrix(Matrix4f*, int);
	virtual void func_unknown31(float, uint32_t);
	virtual void func_unknown32(float);
	virtual float func_unknown33();
	virtual void func_unknown34();
	virtual void func_unknown35();
	virtual void func_unknown36();
	virtual void func_unknown37();
	virtual void func_unknown38();
	virtual void func_unknown39();
	virtual void func_unknown40();
	virtual void func_unknown41();
	virtual void func_unknown42();
	virtual void func_unknown43();
	virtual void func_unknown44(uint32_t);
	virtual void func_unknown45();
	virtual uint32_t func_GetUnknown();
	virtual void func_SetUnknown(uint32_t);
	virtual void func_unknown46();
	virtual void func_unknown47();
	virtual void func_unknown48(float);
	virtual void func_unknown49(float*, float*);
};
