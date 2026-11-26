#pragma once

#include "ptle/containers/Array/Array.h"
#include "ptle/containers/TreeMap/TreeMap.h"

class EIPlayer;
class EIResetPlane;
class ERLevel;
class ERScript;
class EUIHud;


struct LevelDescription
{
	int unknown0;

	uint32_t m_levelCRC;
	uint32_t unknown1;
	uint32_t m_scriptCRC;

	int unknown2;

	uint32_t unknown3;
	uint32_t unknown4;

	Array m_scriptCRCArray;   // array of uint32_t.

	uint32_t m_modelCRC0;
	uint32_t m_modelCRC1;
	uint32_t m_modelCRC2;
	uint32_t m_musicCRC;

	char padding[0x6D];

	bool m_backfaceCulling;

	char padding1[0xE];

	Vector3f m_color0;
	Vector3f m_color1;
};

class ELevelController
{
public:

	ERLevel* m_world;
	LevelDescription* m_levelDesc;

	char padding[0x4];

	TreeMap m_levelDescList;  // map of level crc (uint32_t) -> LevelDescription*
	Array m_scriptsArray;
	ERScript* unknown3;

	/* ... */
};

class GLOBAL_STRUCT_PRIVATE
{
public:

	static inline GLOBAL_STRUCT_PRIVATE* getInstance() { return (GLOBAL_STRUCT_PRIVATE*) 0x917028; }


	ELevelController* m_levelController;
	void* unknown0;
	EIPlayer* m_playerCharacter;

	void* unknown1;

	char padding0[0x10];

	EIResetPlane* m_resetPlane;
	void* m_weatherMan;

	char padding1[0x34];

	uint32_t m_currentLevelCRC;
	uint32_t m_previousLevelCRC;

	uint32_t m_playerStartHash;

	char padding2[0x20];

	EUIHud* m_hud;

	/* ... */


	virtual ~GLOBAL_STRUCT_PRIVATE();
};
