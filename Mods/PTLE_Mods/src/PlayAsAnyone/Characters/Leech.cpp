#include "Leech.h"


static const int bonesMap_leech[] = {
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
	10,  // 10
	11,
	12,
	13,
	14,
	15,
	20,
	21,
	22,
	23,
	24,  // 20
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	37,  // 30
	38,
	39,
	40,  // Eyelids
	-1,
	-1,
	-1,
	43,
	44,
	45,
	46,  // 40
	47,
	48,
	49,  // Eyeball R
	50,
	51,
	-1,
	52,
	53,
	54,
	55,  // 50
	56,
	57,
	58,
	59,
	60,
	61,
	62,
	63,
	64,
	65,  // 60
	66,
	67,
};


CharacterLeech::CharacterLeech()
	: CharacterInfo( 0xE27FA508, 0xE27FA508, 63, bonesMap_leech )
{
}
