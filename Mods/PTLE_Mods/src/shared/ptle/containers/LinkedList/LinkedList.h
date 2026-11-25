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


template<typename T>
struct TLinkedListElem
{
	T m_data;
	TLinkedListElem<T>* m_prev;
	TLinkedListElem<T>* m_next;
};

template<typename T>
class TLinkedList
{
public:

	typedef TLinkedListElem<T> Elem;

	// Don't really know which is who yet, code is very confusing...
	Elem* m_iterateFirst;
	Elem* m_first;
};
