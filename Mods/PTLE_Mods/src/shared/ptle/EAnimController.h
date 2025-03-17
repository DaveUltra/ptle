#pragma once

#include "ptle/containers/TreeMap/TreeMap.h"

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

	ERCharacter* m_characterResource;

	char padding0[0x1C];

	TreeMap m_treeMap1;
	TreeMap m_treeMap2;
	void* m_listPtr0;
	void* m_listPtr1;

	char padding1[0x14];
};
