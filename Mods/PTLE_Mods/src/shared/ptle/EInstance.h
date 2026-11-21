#pragma once

#include "EStorable.h"

#include "math/AABB.h"

class TreeMap;
class TreeMapNode;
class ERLevel;
class ERScript;
class Vector3f;
class Vector4f;
class Matrix4f;


struct OverlapStruct
{
	uint32_t m_flags;
	int padding[2];
	class EInstance* m_other;
};

class EInstance : public EStorable
{
public:

	static const uint32_t VTABLE = 0x8A52C0;


	uint32_t m_uniqueID;

	int unused1;
	ERLevel* m_world;
	int unused2;

	char padding0[0x08];

	float m_opacityFinal;
	float m_opacity2;
	float m_opacity3;
	float m_alphaUser;

	uint32_t m_flags;

	void* unknown4;
	TreeMapNode* m_updateRegionNode;
	void* unknown5;

	AABB m_bounds;

	char padding2[0x8];

	TreeMap* unknown3;
	uint32_t m_frameCount;



	virtual void Init();
	virtual void Tick();
	virtual int func_unknown1();
	virtual void Render(void*, void*);
	virtual void func_unknown3();
	virtual void func_unknown4(Matrix4f*);
	virtual int func_unknown5();
	virtual bool OverlapRelated0(OverlapStruct*, Vector3f*, Vector3f*, uint32_t);
	virtual bool OverlapRelated1(OverlapStruct*, Vector3f*, Vector3f*, float, int);
	virtual bool func_unknown8();
	virtual void func_unknown9(Vector3f*, float*, int*);
	virtual void func_unknown10();
	virtual void SetFlags(uint32_t);
	virtual void ClearFlags(uint32_t);
	virtual void func_unknown11(bool, bool, bool);
	virtual void func_unknown12(bool, bool);
	virtual void GetBoundsCenterAndExtent(Vector4f*);
	virtual void Deserialize(/*EStream*/ void*);
	virtual bool func_unknown13();
	virtual void CreateScriptContext(ERScript*, int);
	virtual void func_unknown14();
	virtual void func_unknown15();
	virtual void func_unknown16();
	virtual void func_unknown17(Vector4f*);
	virtual void GetBoundsCenter(Vector3f*);
	virtual void GetTransformMatrix(Matrix4f*);
	virtual void func_unknown20();
	virtual void func_unknown21();
	virtual void func_unknown22();
	virtual void func_unknown23();
	virtual void func_unknown24();
	virtual bool func_unknown25();
};
