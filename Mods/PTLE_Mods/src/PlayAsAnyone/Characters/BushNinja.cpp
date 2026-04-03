#include "BushNinja.h"


const int bonesMap_bushNinja[] = {
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
	14,  // Spine
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
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,  // WigMaster
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
	-1,
	-1,
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
	58,//71,
	-1,
	-1,
	-1,
};


CharacterBushNinja::CharacterBushNinja()
	: CharacterInfo( 0x8A0B3E7A, 0x8A0B3E7A, 70, bonesMap_bushNinja )
{
}
