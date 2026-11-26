#pragma once

#include <d3d9.h>


class EGraphics
{
public:

	static inline EGraphics* getInstance() { return (EGraphics*) 0x920E00; }


	char padding0[0xCC];

	IDirect3D9* m_d3d9;
	int* m_adapters;
	int m_numAdapters;
	int m_adapterID;


	virtual ~EGraphics();
};
