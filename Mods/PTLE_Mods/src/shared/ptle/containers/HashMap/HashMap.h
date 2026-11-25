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


template<typename K, typename V>
class THashMapEntry
{
public:

	typedef THashMapEntry<K, V> Entry;

	Entry* m_iterateNext;
	Entry* m_right;
	Entry* m_bucketNext;
	K m_key;
	V m_value;
};

template<typename K, typename V>
class THashMap
{
public:

	typedef THashMapEntry<K, V> Entry;

	Entry* unknown0;
	Entry* m_iterateFirst;
	Entry** m_hashTable;
	uint32_t m_capacity;
	uint32_t m_size;
};
