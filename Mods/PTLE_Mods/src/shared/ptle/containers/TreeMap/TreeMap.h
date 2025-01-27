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
