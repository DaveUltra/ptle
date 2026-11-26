#pragma once

#include "ThreadBase.h"

#include <stdint.h>

#include <d3d9.h>


struct RenderStruct1
{
	char padding[0x128];
};

struct RenderStruct2
{
	int unknown0[128][4];
	uint32_t unknown1;
	uint32_t unknown2;
	int unknown3;
	int unknown4;
};

class ThreadRenderer : public ThreadBase
{
public:

	static inline ThreadRenderer* getInstance() { return (ThreadRenderer*) 0x926790; }


	bool unknown0;

	uint32_t m_renderWidth;
	uint32_t m_renderHeight;

	char padding0[0x44];

	IDirect3DDevice9* m_d3dDevice;

	char padding1[0x524];

	RenderStruct1 m_renderStruct1;
	RenderStruct2 m_renderStruct2;

	D3DPRESENT_PARAMETERS m_d3dPresentParams;

	/* ... */
};
