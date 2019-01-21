#pragma once

template <class T, class keyType>
class LinkedList2
{
	T& frontOverloadHelper();
	T& backOverloadHelper();

	//used insert() to make sure that the 2 lists are not part of the same list,
	//as it is unknown which element may be first, then both elements' m_Next sequences are inspected
	//it is not expected to be triggered if the class is used correctly, but it is still a useful protection from undefined behavior
	bool ValidateListsNeverCross(const LinkedList2<T, keyType> &, const LinkedList2<T, keyType> &)const;
	void eraseNextElement(LinkedList2<T, keyType>&);
	void eraseOverloadHelper(const keyType&);
public:
	LinkedList2();
	LinkedList2(const T&, const keyType&);
	LinkedList2(const T&, const keyType&, LinkedList2&);
	explicit LinkedList2(const T&);
	LinkedList2(const LinkedList2&);
	LinkedList2<T, keyType> & operator=(const LinkedList2<T, keyType>&);
	~LinkedList2();


	//ElementAccess

	T& front();
	const T& front() const;
	T& back();
	const T& back() const;


	//Capacity

	bool empty()const;//the list is considered empty if it doesn't point to another list
	unsigned int size()const;//the amount of lists that are linked to the list


	//Modifiers

	void clear()noexcept;
	void insert(const keyType&, LinkedList2<T, keyType>&);
	void erase(const keyType&);
	void erase(LinkedList2<T, keyType>&);
	void pop_back();
	void pop_front();
	void push_back(LinkedList2<T, keyType>&);
	void push_front(LinkedList2<T, keyType>&);


	//Operations

	void merge(LinkedList2&);
	void reverse();

	//non-member functions

	template <class T2, class keyType2>
	friend bool operator==(const LinkedList2<T2, keyType2>&, const LinkedList2<T2, keyType2>&);
	template <class T2, class keyType2>
	friend bool operator!=(const LinkedList2<T2, keyType2>&, const LinkedList2<T2, keyType2>&);
	template <class T2, class keyType2>
	friend void swap(LinkedList2<T2, keyType2>&, LinkedList2<T2, keyType2>&);
private:
	T m_Data;
	keyType m_Key;
	LinkedList2<T, keyType> * m_Next;
};


template<class T, class keyType>
inline T& LinkedList2<T, keyType>::frontOverloadHelper()
{
	return this->m_Data;
}

template<class T, class keyType>
inline T & LinkedList2<T, keyType>::backOverloadHelper()
{
	if (this->m_Next != nullptr)
	{
		LinkedList2 * ptr = this->m_Next;

		while (ptr->m_Next != nullptr)
		{
			ptr = ptr->m_Next;
		}
		return ptr->m_Data;
	}
	return this->m_Data;
}

template<class T, class keyType>
inline LinkedList2<T, keyType>::LinkedList2() : m_Key(), m_Data(), m_Next(nullptr)
{
}

template<class T, class keyType>
inline LinkedList2<T, keyType>::LinkedList2(const T & data, const keyType & key) : m_Data(data), m_Key(key), m_Next(nullptr)
{
}

template<class T, class keyType>
inline LinkedList2<T, keyType>::LinkedList2(const T& data, const keyType & key, LinkedList2<T, keyType>& next) : m_Data(data), m_Key(key)
{
	this->m_Next = &next;
}

template<class T, class keyType>
inline LinkedList2<T, keyType>::LinkedList2(const T & rhs) : m_Data(rhs), m_Key(), m_Next(nullptr)
{
}

template<class T, class keyType>
inline LinkedList2<T, keyType>::LinkedList2(const LinkedList2<T, keyType> & rhs) : m_Data(rhs.m_Data), m_Key(rhs.m_Key), m_Next(nullptr)
{
}

template<class T, class keyType>
inline LinkedList2<T, keyType> & LinkedList2<T, keyType>::operator=(const LinkedList2<T, keyType> & rhs)
{
	if (this != &rhs)
	{
		this->m_Key = rhs.m_Key;
		this->m_Data = rhs.m_Data;
	}
	return *this;
}

template<class T, class keyType>
inline LinkedList2<T, keyType>::~LinkedList2()
{
}

template<class T, class keyType>
inline T& LinkedList2<T, keyType>::front()
{
	return frontOverloadHelper();
}

template<class T, class keyType>
inline const T & LinkedList2<T, keyType>::front() const
{
	return frontOverloadHelper();
}

template<class T, class keyType>
inline T & LinkedList2<T, keyType>::back()
{
	return backOverloadHelper();
}

template<class T, class keyType>
inline const T & LinkedList2<T, keyType>::back() const
{
	return backOverloadHelper();
}

template<class T, class keyType>
inline bool LinkedList2<T, keyType>::empty() const
{
	return this->m_Next == nullptr;
}

template<class T, class keyType>
inline unsigned int LinkedList2<T, keyType>::size() const
{
	LinkedList2<T, keyType> * tmp = this->m_Next;
	unsigned int i = 0;
	while (tmp != nullptr)
	{
		++i;
		tmp = tmp->m_Next;
	}
	return i;
}

template<class T, class keyType>
inline void LinkedList2<T, keyType>::clear() noexcept
{
	this->m_Next = nullptr;
}

template<class T, class keyType>
void LinkedList2 <T, keyType>::insert(const keyType & srPos, LinkedList2<T, keyType>& val)
{
	if (ValidateListsNeverCross(*this, val))
	{
		if (this->m_Key == srPos)
		{
			push_front(val);
			return;
		}
		LinkedList2<T, keyType> * tmp = this;
		while (tmp->m_Next != nullptr)
		{
			if (tmp->m_Next->m_Key == srPos)
			{
				LinkedList2<T, keyType> * tmp2 = tmp->m_Next;
				tmp->m_Next = &val;
				val.m_Next = tmp2;
				return;
			}
			tmp = tmp->m_Next;
		}
		std::cerr << "Insert operation failed, no such key has been found\n";
	}
	else
	{
		std::cerr << "Insert operation failed, because the two lists are linked and this will cause an infinite loop\n";
	}
}

template<class T, class keyType>
inline void LinkedList2<T, keyType>::erase(const keyType & rhs)
{
	eraseOverloadHelper(rhs);
}

template<class T, class keyType>
inline void LinkedList2<T, keyType>::erase(LinkedList2<T, keyType>& rhs)
{
	eraseOverloadHelper(rhs.m_Key);
}

template<class T, class keyType>
inline void LinkedList2<T, keyType>::pop_back()
{
	if (this->m_Next == nullptr)
	{
		std::cerr << "List is already empty\n";
	}
	else
	{
		LinkedList2<T, keyType> * tmp = this;
		while (tmp->m_Next->m_Next != nullptr)
		{
			tmp = tmp->m_Next;
		}
		tmp->m_Next = nullptr;
	}
}

template<class T, class keyType>
inline void LinkedList2<T, keyType>::pop_front()
{
	if (this->m_Next == nullptr)
	{
		std::cerr << "Cannot erase the front element because the list is empty\n";
	}
	else
	{
		this->m_Data = this->m_Next->m_Data;
		this->m_Key = this->m_Next->m_Key;
		LinkedList2<T, keyType> *  tmp = this->m_Next;
		this->m_Next = tmp->m_Next;
		tmp->m_Next = nullptr;
	}
}

template<class T, class keyType>
inline void LinkedList2<T, keyType>::push_back(LinkedList2<T, keyType>& rhs)
{
	if (ValidateListsNeverCross(*this, rhs))
	{
		LinkedList2<T, keyType> * tmp = this;
		while (tmp->m_Next != nullptr)
		{
			tmp = tmp->m_Next;
		}
		tmp->m_Next = &rhs;
	}
	else
	{
		std::cerr << "push_back operation failed, because the two lists are linked and this will cause an infinite loop\n";
	}
}

template<class T, class keyType>
inline void LinkedList2<T, keyType>::push_front(LinkedList2<T, keyType>& rhs)
{
	if (ValidateListsNeverCross(*this, rhs))
	{
		LinkedList2<T, keyType> * tmp = this->m_Next;
		this->m_Next = &rhs;
		push_back(*tmp);
		swap(*this, *(this->m_Next));
	}
	else
	{
		std::cerr << "push_front operation failed, because the two lists are linked and this will cause an infinite loop\n";
	}
}

template<class T, class keyType>
inline void LinkedList2<T, keyType>::merge(LinkedList2 & rhs)
{
	if (ValidateListsNeverCross(*this, rhs))
	{
		LinkedList2<T, keyType> * tmp = this;
		while (tmp->m_Next != nullptr)
		{
			tmp = tmp->m_Next;
		}
		tmp->m_Next = &rhs;
	}
	else
	{
		std::cerr << "Function merge() cannot proceed, because the lists cross somewhere\n";
	}
}

template<class T, class keyType>
inline void LinkedList2<T, keyType>::reverse()
{
	LinkedList2<T, keyType> * mover = this;
	LinkedList2<T, keyType> * rBorder = this;
	while (rBorder->m_Next != nullptr)
	{
		rBorder = rBorder->m_Next;
	}
	LinkedList2<T, keyType> * lBorder = mover;

	while (lBorder != rBorder)
	{
		while (mover->m_Next != rBorder)
		{
			mover = mover->m_Next;
		}
		
		swap(*lBorder, *rBorder);
		if (lBorder->m_Next == rBorder)
		{
			break;
		}

		rBorder = mover;//one step behind
		lBorder = lBorder->m_Next;//one step further
		mover = lBorder;
	}
}

template<class T, class keyType>
inline bool LinkedList2<T, keyType>::ValidateListsNeverCross(const LinkedList2<T, keyType>& lhs, const LinkedList2<T, keyType>& rhs) const
{
	LinkedList2<T, keyType> * tmp = lhs.m_Next;
	while (tmp != nullptr)
	{
		if (tmp == &rhs)
		{
			return false;
		}
		tmp = tmp->m_Next;
	}
	tmp = rhs.m_Next;
	while (tmp != nullptr)
	{
		if (tmp == &lhs)
		{
			return false;
		}
		tmp = tmp->m_Next;
	}
	return true;
}

template<class T, class keyType>
inline void LinkedList2<T, keyType>::eraseNextElement(LinkedList2<T, keyType>& rhs)
{
	LinkedList2<T, keyType> * tmp2 = rhs.m_Next;
	rhs.m_Next = tmp2->m_Next;
	tmp2->m_Next = nullptr;
}

template<class T, class keyType>
inline void LinkedList2<T, keyType>::eraseOverloadHelper(const keyType & rhs)
{
	if (this->m_Next != nullptr)
	{
		if (this->m_Key == rhs)
		{
			pop_front();
			return;
		}
		else
		{
			LinkedList2<T, keyType> * tmp = this;
			if (tmp->m_Key == rhs)
			{
				eraseNextElement(*tmp);
				return;
			}
			if (tmp->m_Next != nullptr)
			{

				while (tmp->m_Next != nullptr)
				{
					if (tmp->m_Next->m_Key == rhs)
					{
						eraseNextElement(*tmp);
						return;
					}
					tmp = tmp->m_Next;
				}
				if (tmp->m_Key == rhs)
				{
					pop_back();
					return;
				}
			}
			else if (tmp->m_Key == rhs)//in case this->m_Next is nullptr
			{
				pop_back();
				return;
			}
		}
	}
	else if (this->m_Key == rhs)
	{
		std::cerr << "Cannot delete the first elemment, because the list is empty\n";
		return;
	}
	std::cerr << "Element not found\n";
}

template<class T2, class keyType2>
inline bool operator==(const LinkedList2<T2, keyType2>& lhs, const LinkedList2<T2, keyType2>& rhs)
{
	return (lhs.m_Data == rhs.m_Data && lhs.m_Key == rhs.m_Key);
}

template<class T2, class keyType2>
inline bool operator!=(const LinkedList2<T2, keyType2>& lhs, const LinkedList2<T2, keyType2>& rhs)
{
	return !operator==(lhs, rhs);
}

template<class T2, class keyType2>
inline void swap(LinkedList2<T2, keyType2>& lhs, LinkedList2<T2, keyType2>& rhs)
{
	LinkedList2<T2, keyType2> tmp = lhs;
	lhs = rhs;
	rhs = tmp;
	//m_Next are still the same
}
