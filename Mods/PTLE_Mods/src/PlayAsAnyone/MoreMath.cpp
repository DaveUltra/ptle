#include "MoreMath.h"



void IdentityTransform( Transform* t )
{
	t->m_rotationQuat = { 0.0F, 0.0F, 0.0F, 1.0F };
	t->m_position     = { 0.0F, 0.0F, 0.0F };
	t->m_scale        = { 1.0F, 1.0F, 1.0F };
}


void QuatMul( Vector4f* q, const Vector4f* o )
{
	Vector4f r;
	r.x = q->y * o->z - q->z * o->y;
	r.y = q->z * o->x - q->x * o->z;
	r.z = q->x * o->y - q->y * o->x;

	r.x += q->x * o->w - o->x * q->w;
	r.y += q->y * o->w - o->y * q->w;
	r.z += q->z * o->w - o->z * q->w;

	r.w = q->x * o->x + q->y * o->y + q->z * o->z + q->w * o->w;

	*q = r;
}

void QuatMulInv( Vector4f* q, const Vector4f* o )
{
	Vector4f r;
	r.x = q->z * o->y - q->y * o->z;
	r.y = q->x * o->z - q->z * o->x;
	r.z = q->y * o->x - q->x * o->y;

	r.x += q->x * o->w + o->x * q->w;
	r.y += q->y * o->w + o->y * q->w;
	r.z += q->z * o->w + o->z * q->w;

	r.w = -q->x * o->x - q->y * o->y - q->z * o->z + q->w * o->w;

	*q = r;
}


void TransformSetInv( Transform* t, const Transform* o )
{
	*t = *o;
	t->m_rotationQuat.x = -t->m_rotationQuat.x;
	t->m_rotationQuat.y = -t->m_rotationQuat.y;
	t->m_rotationQuat.z = -t->m_rotationQuat.z;
}

void TransformMul( Transform* t, const Transform* o )
{
	Vector4f quat = o->m_rotationQuat;
	QuatMul( &quat, &t->m_rotationQuat );
	t->m_rotationQuat = quat;
}

void TransformMulInv( Transform* t, const Transform* o )
{
	Vector4f quat = o->m_rotationQuat;
	QuatMulInv( &quat, &t->m_rotationQuat );
	t->m_rotationQuat = quat;
}
