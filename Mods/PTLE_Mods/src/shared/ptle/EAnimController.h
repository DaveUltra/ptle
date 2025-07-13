#pragma once

#include "ptle/containers/TreeMap/TreeMap.h"
#include "ptle/containers/LinkedList/LinkedList.h"

class Matrix4f;

class ERCharacter;


// Exact size of this type is still unknown, but is at least 0x60 bytes.
class EAnimController
{
public:

	void* vtable;

	// This is a matrix array.
	Matrix4f* m_pose;

	int unknown0;

	Matrix4f* unknown1;

	char padding0[0x08];

	ERCharacter* m_characterResource;

	char padding1[0x10];

	TreeMap m_treeMap1;
	TreeMap m_treeMap2;
	LinkedList m_list;

	float unknown2;
	float unknown3;

	char padding2[0x04];

	bool unknown4;
	bool unknown5;
	bool unknown6;

	char padding3[0x05];
};
