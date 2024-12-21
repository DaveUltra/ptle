#pragma once


class Vector4f
{
public:

	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float data[4];
	};
};
