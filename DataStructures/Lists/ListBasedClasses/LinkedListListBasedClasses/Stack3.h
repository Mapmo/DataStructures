#pragma once
#include "..\..\LinkedLists\LinkedList2.h"

template<class T, class keyType = int>
class Stack3 : private LinkedList1<T, keyType>
{
public:

	//Element access

	T & top();
	const T& top()const;

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
inline T & Stack3<T, keyType>::top()
{
	return LinkedList1<T, keyType>::back();
}

template<class T, class keyType>
inline const T & Stack3<T, keyType>::top() const
{
	return LinkedList1<T, keyType>::back();
}

template<class T, class keyType>
inline bool Stack3<T, keyType>::empty() const
{
	return LinkedList1<T, keyType>::empty();
}

template<class T, class keyType>
inline unsigned int Stack3<T, keyType>::size() const
{
	return LinkedList1<T, keyType>::size();
}

template<class T, class keyType>
inline void Stack3<T, keyType>::clear() noexcept
{
	LinkedList1<T, keyType>::clear();
}

template<class T, class keyType>
inline T Stack3<T, keyType>::pop()
{
	T tmp = top();
	LinkedList1<T, keyType>::pop_back();
	return tmp;
}

template<class T, class keyType>
inline void Stack3<T, keyType>::push(const T & rhs)
{
	LinkedList1<T, keyType> tmp(rhs);
	LinkedList1<T, keyType>::push_back(tmp);
}

template<class T, class keyType>
inline void Stack3<T, keyType>::push(LinkedList1<T, keyType>& rhs)
{
	LinkedList1<T, keyType>::push_back(rhs);
}
