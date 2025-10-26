#pragma once

#include <stdint.h>

struct Transform;


class CharacterInfo
{
public:

	CharacterInfo( uint32_t modelCRC, uint32_t characterCRC, int numBones, const int* bonesMap )
		: m_modelCRC( modelCRC ), m_characterCRC( characterCRC ), m_bonesMap( bonesMap ), m_numBones( numBones )
	{
	}


	virtual void postProcessPose( Transform* destPose, const Transform* harryPose ) const { };

	uint32_t m_modelCRC;      // ERModel asset.
	uint32_t m_characterCRC;  // ERCharacter asset.
	const int* m_bonesMap;    // Map of Harry's skeleton to the target character's.
	int m_numBones;
};
