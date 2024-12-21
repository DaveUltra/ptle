#pragma once


class Vector3f
{
public:

	union {
		struct {
			float x;
			float y;
			float z;
		};
		float data[3];
	};
};
