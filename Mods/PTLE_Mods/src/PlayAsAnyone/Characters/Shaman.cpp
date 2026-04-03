#include "Shaman.h"


const int bonesMap_shaman[] = {
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
	37,  // Neck1
	38,  // Head
	39,
	-1,  // Mask
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
};


CharacterShaman::CharacterShaman()
	: CharacterInfo( 0x4D0ADF40, 0x70B7FB6A, 53, bonesMap_shaman )
{

}
