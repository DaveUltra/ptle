#pragma once


struct LinkedListElem
{
	void* m_data;
	LinkedListElem* m_prev;
	LinkedListElem* m_next;
};

class LinkedList
{
public:

	// Don't really know which is who yet, code is very confusing...
	LinkedListElem* m_iterateFirst;
	LinkedListElem* m_first;
};
