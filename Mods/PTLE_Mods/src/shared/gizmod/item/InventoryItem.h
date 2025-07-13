#pragma once

#include <stdint.h>

#include "ItemModelsCRC.h"


struct ItemStruct
{
	void (*m_factory)();
	void* m_object;
	const char* m_name;
	uint32_t m_hash;
	int unknown0;
	int m_unlocked;
	uint32_t m_unlockedHash;
};

class InventoryItem
{
	friend class Gizmod;

public:

	enum Item
	{
		SLING,
		TORCH,
		PICKAXES,
		TNT,
		SHIELD,
		RAFT,
		GAS_MASK,
		CANTEEN,

		UNKNOWN = -1
	};

	static inline Item getItemByHash( uint32_t itemHash );


private:

	InventoryItem() { }

public:

	inline const char* getName() const;

	inline uint32_t getHash() const;

	inline uint32_t getModelCRC() const;

	inline bool isUnlocked() const;
	inline void setUnlocked( bool b );


private:

	ItemStruct* m_itemStruct;
	Item m_itemType;
};


#include "InventoryItem.inl"
