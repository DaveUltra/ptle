#include "Chief.h"


const int bonesMap_chief[] = {
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
	20,  // Neck
	38,  // Head
	39,
	-1,
	-1,
	-1,
	-1,  // Center brow?
	-1,  // Earing L
	-1,  // Earing R
	45,  // Eye L
	49,  // Eye R
	46,  // Eyebrow L
	50,  // Eyebrow R
	-1,
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
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
};


CharacterChief::CharacterChief()
	: CharacterInfo( 0xEA2B4CFC, 0xEA2B4CFC, 73, bonesMap_chief )
{
}
