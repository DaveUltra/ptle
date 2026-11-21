#pragma once

#include <stdint.h>

struct Transform;


class CharacterInfo
{
public:

	CharacterInfo( uint32_t modelCRC, uint32_t characterCRC, int numBones, const int* bonesMap )
		: m_modelCRC( modelCRC ), m_characterCRC( characterCRC ), m_bonesMap( bonesMap ), m_numBones( numBones )
	{
		this->calcInversePose();
	}


	virtual void postProcessPose( Transform* destPose, const Transform* harryPose ) const { };

	int findCharBoneID(int harryBoneID) const;

	void calcInversePose();


	uint32_t m_modelCRC;      // ERModel asset.
	uint32_t m_characterCRC;  // ERCharacter asset.
	const int* m_bonesMap;    // Map of Harry's skeleton to the target character's.
	int* m_inverseBonesMap;   // Map of the character's skeleton to Harry's.
	int m_numBones;
};
