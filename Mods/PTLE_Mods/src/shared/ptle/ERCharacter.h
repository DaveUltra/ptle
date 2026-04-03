#pragma once

#include "EResource.h"

#include "ptle/math/Vector3f.h"
#include "ptle/math/Vector4f.h"
#include "ptle/math/Matrix4f.h"
#include "ptle/containers/Array/Array.h"


struct CharacterBoneInfo
{
	int m_parentBoneIndex;
	Array m_childBoneIndices;

	Vector3f m_translation;
	Vector4f m_rotation;

	Matrix4f unknown2;
	Matrix4f unknown3;

	bool unknown4;

	const char* m_name;
};

class ERCharacter : public EResource
{
public:

	// These 3 are part of an Array.
	CharacterBoneInfo* m_boneList;
	int m_numBones;
	int m_arrayCapacity;

	Vector4f unknown;

	char padding2[0x04];
};
