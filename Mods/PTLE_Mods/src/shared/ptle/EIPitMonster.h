#pragma once

#include "EIBeast.h"

class EIMarker;


class EIPitMonster : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x890400;


	float unknown0;
	float unknown1;

	uint32_t m_markerHash;
	EIMarker* m_marker;

	Vector3f unknown2;
	Vector3f unknown3;
	Vector3f unknown4;
	Vector3f unknown5;
	Vector3f unknown6;

	bool unknown7;
	bool m_locked;

	float unknown8;
	float unknown9;
	float unknown10;

	int unknown11;

	bool unknown12;
	bool unknown13;
};
