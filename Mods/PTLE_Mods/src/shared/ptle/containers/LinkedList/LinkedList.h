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

	struct ConstIterator
	{
		__forceinline bool operator!=(const ConstIterator& o) const { return m_cur != o.m_cur; }
		__forceinline void operator++() { m_cur = m_cur->m_next; }

		__forceinline const T* operator->() const { return &m_cur->m_data; }
		__forceinline const T& operator*() const { return m_cur->m_data; }

		mutable const Elem* m_cur;
	};

public:

	// Don't really know which is who yet, code is very confusing...
	Elem* m_iterateFirst;
	Elem* m_first;


	__forceinline ConstIterator begin() const { return ConstIterator{m_iterateFirst}; }
	__forceinline ConstIterator end() const { return ConstIterator{(const Elem*) 0}; }
};
