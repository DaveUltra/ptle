#include "CharacterInfo.h"

#include "PlayAsAnyone.h"

#include <stdlib.h>


int CharacterInfo::findCharBoneID(int harryBoneID) const
{
	for ( int i = 0; i < m_numBones; i++ ) {
		if ( m_bonesMap[i] == harryBoneID ) {
			return i;
		}
	}
	return -1;
}

void CharacterInfo::calcInversePose()
{
	int* map = (int*) malloc( NUM_HARRY_BONES * sizeof(int) );
	for ( int i = 0; i < NUM_HARRY_BONES; i++ ) {
		map[i] = findCharBoneID(i);
	}

	m_inverseBonesMap = map;
}
