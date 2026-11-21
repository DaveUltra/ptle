#include "Micay.h"

#include "MoreMath.h"


const int bonesMap_micay[] = {
	0,
	1,
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
	14,
	20,
	38,
	39,
	-1,  // Ponytail
	-1,
	-1,
	-1,  // Ears???
	-1,
	46,  // Eyebrow L
	50,  // Eyebrow R
	45,  // Eye L
	40,  // Eyelids
	49,  // Eye R
	43,  // Jaw
	44,  // Lower lip
	48,  // Upper lip L
	52,  // Upper lip R
	54,  // Upper lip center
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
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
};


CharacterMicay::CharacterMicay()
	: CharacterInfo( 0x0954D8DF, 0x0954D8DF, 65, bonesMap_micay )
{
}

void CharacterMicay::postProcessPose( Transform* destPose, const Transform* harryPose ) const
{
	// Combine spine segments.
	destPose[14] = harryPose[19];
	TransformMulInv( &destPose[14], &harryPose[15] );
	TransformMulInv( &destPose[14], &harryPose[14] );

	// Combine neck segments.
	destPose[15] = harryPose[37];
	TransformMulInv( &destPose[15], &harryPose[20] );

	destPose[0].m_position.x += 0.4F;
}
