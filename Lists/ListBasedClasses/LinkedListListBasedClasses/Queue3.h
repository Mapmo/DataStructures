#pragma once
#include "..\..\LinkedLists\LinkedList2.h"

template<class T, class keyType = int>
class Queue3 : private LinkedList1<T, keyType>
{
public:

	//ElementAccess

	T& front();
	const T& front() const;
	T& back();
	const T& back() const;


	//Capacity

	bool empty()const;
	unsigned int size()const;//the amount of lists that are linked to the list


	//Modifiers

	void clear()noexcept;
	T pop();
	void push(const T&);
	void push(LinkedList1<T, keyType>&);
};

template<class T, class keyType>
inline T & Queue3<T, keyType>::front()
{
	return LinkedList1<T, keyType>::front();
}

template<class T, class keyType>
inline const T & Queue3<T, keyType>::front() const
{
	return LinkedList1<T, keyType>::front();
}

template<class T, class keyType>
inline T & Queue3<T, keyType>::back()
{
	return LinkedList1<T, keyType>::back();
}

template<class T, class keyType>
inline const T & Queue3<T, keyType>::back() const
{
	return LinkedList1<T, keyType>::back();
}

template<class T, class keyType>
inline bool Queue3<T, keyType>::empty() const
{
	return LinkedList1<T, keyType>::empty();
}

template<class T, class keyType>
inline unsigned int Queue3<T, keyType>::size() const
{
	return LinkedList1<T, keyType>::size();
}

template<class T, class keyType>
inline void Queue3<T, keyType>::clear() noexcept
{
	LinkedList1<T, keyType>::clear();
}

template<class T, class keyType>
inline T Queue3<T, keyType>::pop()
{
	T tmp = front();
	LinkedList1<T, keyType>::pop_front();
	return tmp;
}

template<class T, class keyType>
inline void Queue3<T, keyType>::push(const T & rhs)
{
	LinkedList1<T, keyType> tmp(rhs);
	LinkedList1<T, keyType>::push_back(tmp);
}

template<class T, class keyType>
inline void Queue3<T, keyType>::push(LinkedList1<T, keyType>& rhs)
{
	LinkedList1<T, keyType>::push_back(rhs);
}
