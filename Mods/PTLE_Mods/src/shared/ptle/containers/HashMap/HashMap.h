#pragma once

#include <stdint.h>


class HashMapEntry
{
public:

	HashMapEntry* m_iterateNext;
	HashMapEntry* m_right;
	HashMapEntry* m_bucketNext;
	uint32_t m_key;
	void* m_ptr;
};

class HashMap
{
public:

	HashMapEntry* unknown0;
	HashMapEntry* m_iterateFirst;
	HashMapEntry** m_hashTable;
	uint32_t m_capacity;
	uint32_t m_size;
};
