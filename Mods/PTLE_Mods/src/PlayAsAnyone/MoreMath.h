#pragma once

#include "ptle/math/Vector3f.h"
#include "ptle/math/Vector4f.h"
#include "ptle/math/Matrix4f.h"

#include <stdint.h>


struct Transform
{
	Vector4f m_rotationQuat;
	Vector3f m_position;
	uint32_t u;
	Vector3f m_scale;
	uint32_t uu;
};


void IdentityTransform( Transform* t );

void QuatMul( Vector4f* q, const Vector4f* o );
void QuatMulInv( Vector4f* q, const Vector4f* o );

void TransformSetInv( Transform* t, const Transform* o );
void TransformMul( Transform* t, const Transform* o );
void TransformMulInv( Transform* t, const Transform* o );
