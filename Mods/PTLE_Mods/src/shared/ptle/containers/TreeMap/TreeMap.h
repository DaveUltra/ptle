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

	TTreeMapNode<K, V>* m_left;
	TTreeMapNode<K, V>* m_right;
	char padding0[8];
	TTreeMapNode<K, V>* m_iterateNext;
	char padding1[4];
	K m_key;
	V m_value;
};

template<typename K, typename V>
class TTreeMap
{
public:

	typedef TTreeMapNode<K, V> Node;

	Node* m_iterateFirst;
	Node* nodeUnknown;
	Node* m_rootNode;
};
