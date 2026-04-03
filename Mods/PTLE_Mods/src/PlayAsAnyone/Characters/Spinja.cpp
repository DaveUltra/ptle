#include "Spinja.h"

#include "MoreMath.h"

#include <math.h>


const int bonesMap_spinja[] = {
	0,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,  // Spine1
	20,  // Neck
	21,  // Start of left arm
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,  // End of left arm
	24,  // Sickle L
	-1,
	37,  // Neck1
	38,  // Head
	39,
	-1,  // Face_MaskBone
	-1,  // Face_L_EyeBrowBone
	-1,  // Face_R_EyeBrowBone
	55,  // Start of right arm
	56,
	57,
	58,
	59,
	60,
	61,
	62,
	63,
	64,
	65,
	66,
	67,  // End of right arm
	71,  // R_HandHold
	-1,  // Sickle R
	-1,
};


CharacterSpinja::CharacterSpinja()
	: CharacterInfo( 0xBF755D88, 0xBF755D88, 52, bonesMap_spinja )
{
}

void CharacterSpinja::postProcessPose( Transform* destPose, const Transform* harryPose ) const
{
	static float timer = 0.0F;

	// Hide the picks.
	// TODO : Scaling is ignored in this step.
	//destPose[28].m_scale = { 0.0F, 0.0F, 0.0F };
	//destPose[50].m_scale = { 0.0F, 0.0F, 0.0F };

	// Combine spine segments.
	/*destPose[14] = harryPose[14];
	TransformMul( &destPose[14], &harryPose[15] );
	TransformMul( &destPose[14], &harryPose[19] );

	// Combine neck segments.
	destPose[15] = harryPose[20];
	TransformMul( &destPose[15], &harryPose[37] );*/

	// Rotate eyes.
	float c = cos( timer * 0.5F ), s = sin( timer * 0.5F );
	destPose[34].m_rotationQuat = { -s * 0.5F, s * 0.866F, 0.0F,  c };
	destPose[35].m_rotationQuat = {  s * 0.5F, s * 0.866F, 0.0F, -c };
	timer += 0.3F;
}
