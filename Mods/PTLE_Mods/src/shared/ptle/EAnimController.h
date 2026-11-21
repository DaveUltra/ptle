#pragma once

#include "ptle/containers/TreeMap/TreeMap.h"
#include "ptle/containers/LinkedList/LinkedList.h"

class Matrix4f;

class ERAnim;
class ERCharacter;


struct AnimLayer
{
	float m_fps;
	float unknown1;
	uint32_t unknown2;
	bool unknown3;
	bool m_atEnd;
	bool unknown5;
	int unknown6;
	int unknown7;
	float unknown8;
	float unknown9;
	float unknown10;

	char padding0[0x14];

	float m_time;
	float m_frame;

	float unknown13;
	float unknown14;

	float m_speed;
	float m_weight;

	char padding1[0x10];

	float m_targetWeight;

	char padding3[0x14];

	ERAnim* m_anim;

	Vector3f unknown11;
	TreeMap unknown12;

	char padding2[0x8];

	bool m_fading;
	bool m_ended;
	bool m_ticked;
};

// Exact size of this type is still unknown, but is at least 0x60 bytes.
class EAnimController
{
public:

	// This is a matrix array.
	Matrix4f* m_pose;

	int unknown0;

	Matrix4f* unknown1;

	Matrix4f* m_poseBuffers[2];

	ERCharacter* m_characterResource;

	char padding1[0x8];

	uint32_t m_frameStamp;

	char padding4[0x4];

	TreeMap m_animMap;     // Map of anim asset CRC -> AnimLayer
	TreeMap m_treeMap2;
	LinkedList m_list;

	float unknown2;
	float unknown3;

	char padding2[0x04];

	bool unknown4;
	bool unknown5;
	bool unknown6;

	char padding3[0x05];


	virtual ~EAnimController();
	virtual void func_unknown0(Matrix4f*);
};
