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

	static const uint32_t VTABLE = 0x8A6258;


	TArray<CharacterBoneInfo> m_bones;

	Vector4f unknown;

	char padding2[0x04];
};
