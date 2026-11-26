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
	typedef struct { K m_key; V m_value; } Pair;

	Entry* m_iterateNext;
	Entry* m_right;
	Entry* m_bucketNext;
	union {
		Pair m_pair;
		struct { K m_key; V m_value; };
	};
};

template<typename K, typename V>
class THashMap
{
public:

	typedef THashMapEntry<K, V> Entry;

	struct ConstIterator
	{
		__forceinline bool operator!=(const ConstIterator& o) const { return m_cur != o.m_cur; }
		__forceinline void operator++() { m_cur = m_cur->m_iterateNext; }

		__forceinline const typename Entry::Pair* operator->() const { return &m_cur->m_pair; }
		__forceinline const typename Entry::Pair& operator*() const { return m_cur->m_pair; }

		mutable const Entry* m_cur;
	};

public:

	Entry* unknown0;
	Entry* m_iterateFirst;
	Entry** m_hashTable;
	uint32_t m_capacity;
	uint32_t m_size;


	__forceinline ConstIterator begin() const { return ConstIterator{m_iterateFirst}; }
	__forceinline ConstIterator end() const { return ConstIterator{(const Entry*) 0}; }
};
