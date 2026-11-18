#pragma once

#include "ptle/math/Matrix4f.h"

#include <stdint.h>

class ERModel;
class EResource;
class EIHarry;


// Note : "EItemBase" is an unofficial name.
class EItemBase
{
public:

	static const uint32_t VTABLE = 0x882AB8;


	// TODO: MSVC has a bug where a virtual class containing aligned items higher than 4 bytes
	// will add extra padding, thus invalidating member offsets below.
	// We'd like to have those virtual functions, in case they are actually useful.
	void** vtable;

	char padding0[0x10];

	ERModel* m_itemModel;
	EResource* unknown0;
	EResource* unknown1;
	EIHarry* m_harry;

	Matrix4f m_matrix;

	int m_state;

	char padding1[0x1D];

	bool m_active;
};
