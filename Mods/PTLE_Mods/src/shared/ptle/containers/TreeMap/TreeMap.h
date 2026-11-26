#pragma once

#include <stdint.h>


class TreeMapNode
{
public:

	TreeMapNode* m_left;
	TreeMapNode* m_right;
	char padding0[8];
	TreeMapNode* m_iterateNext;
	char padding1[4];
	uint32_t m_hash;
	void* m_ptr;
};

class TreeMap
{
public:

	TreeMapNode* m_iterateFirst;
	TreeMapNode* nodeUnknown;
	TreeMapNode* m_rootNode;
};


template<typename K, typename V>
class TTreeMapNode
{
public:

	typedef TTreeMapNode<K, V> Node;
	typedef struct { K m_key; V m_value; } Pair;

	Node* m_left;
	Node* m_right;
	char padding0[8];
	Node* m_iterateNext;
	char padding1[4];
	union {
		Pair m_pair;
		struct { K m_key; V m_value; };
	};
};

template<typename K, typename V>
class TTreeMap
{
public:

	typedef TTreeMapNode<K, V> Node;

	struct ConstIterator
	{
		__forceinline bool operator!=(const ConstIterator& o) const { return m_cur != o.m_cur; }
		__forceinline void operator++() { m_cur = m_cur->m_iterateNext; }

		__forceinline const typename Node::Pair* operator->() const { return &m_cur->m_pair; }
		__forceinline const typename Node::Pair& operator*() const { return m_cur->m_pair; }

		mutable const Node* m_cur;
	};

public:

	Node* m_iterateFirst;
	Node* nodeUnknown;
	Node* m_rootNode;


	__forceinline ConstIterator begin() const { return ConstIterator{m_iterateFirst}; }
	__forceinline ConstIterator end() const { return ConstIterator{(const Node*) 0}; }
};
