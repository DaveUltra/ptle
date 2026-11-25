#pragma once

#include "EResource.h"

#include "ptle/containers/Array/Array.h"
#include "ptle/math/Vector3f.h"
#include "ptle/math/AABB.h"


class ERShader;

struct ModelPartD3D
{
	int m_vertexCount;
	int unknown0;
	int unknown1;
	int* unknown2;
	uint8_t* unknown3;
	int* unknown4;
	uint16_t* m_indices;
	int m_indexCount;
	int* unknown5;
	uint32_t* m_colors;
	int* unknown6;
	struct IDirect3DVertexBuffer9* m_d3dVertexBuffer;
	struct IDirect3DIndexBuffer9* m_d3dIndexBuffer;
	int unknown7;
	int m_vertexSize;
	uint32_t m_fvf;
};

struct ModelDataArray
{
	int unknown0;
	TArray<ModelPartD3D> m_parts;
	int unknown1;
	uint32_t m_flags;
};

struct ModelChunkData
{
	ERShader* m_shader;
	ModelDataArray m_data;
	char padding[0x8];
};

struct ModelChunk
{
	TArray<ModelChunkData> m_array;
	int unknown0;
};

class ERModel : public EResource
{
public:

	static const uint32_t VTABLE = 0x8A5B4C;


	TArray<ModelChunk> m_modelChunks;
	Vector3f unknown0;

	float unknown1;

	AABB m_bounds;

	Vector3f unknown2;
	Vector3f unknown3;

	char padding1[0x08];

	bool unknown4;
	bool unknown5;
	bool unknown6;
	bool unknown7;

	char padding2[0x04];

	Array unknown8[3];

	char padding3[0x04];
};
