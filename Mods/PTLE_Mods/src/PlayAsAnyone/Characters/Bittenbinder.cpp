#include "Bittenbinder.h"

#include "MoreMath.h"


static const int bonesMap_bb[] = {
	0,   // 0, Root
	1,   // Footsteps
	2,   // Pelvis
	3,   // Spine
	4,   // Start of left leg
	5,
	6,
	7,
	8,   // End of left leg
	9,   // Start of right leg
	10,  // 10
	11,
	12,
	13,  // End of right leg
	14,  // Spine
	20,  // Neck
	38,  // Head
	-1,
	-1,
	46,  // Eyebrow L
	50,  // 20, Eyebrow R
	45,  // Eye L
	49,  // Eye R
	40,  // Eyelids (both of em).
	-1,
	-1,
	43,  // Jaw
	44,  // Lower lip
	48,  // Mustache L
	52,  // Mustache R
	54,  // 30, Upper lip center.
	21,  // Start of left arm.
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,  // 40
	31,
	32,
	33,  // End of left arm.
	55,  // Start of right arm.
	56,
	57,
	58,
	59,
	60,
	61,  // 50
	62,
	63,
	64,
	65,
	66,
	67,  // End of right arm.
	-1,
	-1,
	-1,
	-1,  // 60
};


CharacterBittenbinder::CharacterBittenbinder()
	: CharacterInfo( 0x8874E1AE, 0xA011CEFC, 61, bonesMap_bb )
{
}

void CharacterBittenbinder::postProcessPose( Transform* destPose, const Transform* harryPose ) const
{
	TransformMul( &destPose[14], &harryPose[15] );   // Combine spine segments.
	TransformMul( &destPose[15], &harryPose[37] );   // Combine neck segments.
}
