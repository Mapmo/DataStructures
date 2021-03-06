#pragma once

template <class T, class keyType>
class LinkedList1
{
	T& frontOverloadHelper();
	T& backOverloadHelper();
	void insertOverloadHelper(LinkedList1<T, keyType>* tmp, LinkedList1<T, keyType>&val);

	//used for operator= and insert() to make sure that the 2 lists are not part of the same list,
	//as it is unknown which element may be first, then both elements' m_Next sequences are inspected
	//it is not expected to be triggered if the class is used correctly, but it is still a useful protection from undefined behavior
	bool ValidateListsNeverCross(const LinkedList1<T, keyType> &, const LinkedList1<T, keyType> &)const;
	void eraseNextElement(LinkedList1<T, keyType>&);//as there is no m_Prev class member, it needs to track and delete elements one step earlier
	void eraseOverloadHelper(const keyType&);
public:
	LinkedList1();
	LinkedList1(const T&, const keyType&);
	LinkedList1(const T&, const keyType&, LinkedList1&);
	explicit LinkedList1(const T&);
	LinkedList1(const LinkedList1&);
	LinkedList1<T, keyType> & operator=(const LinkedList1<T, keyType>&);
	~LinkedList1();


	//Element Access

	T& front();
	const T& front() const;
	T& back();
	const T& back() const;


	//Capacity

	bool empty()const;//the list is considered empty if it doesn't point to another list
	unsigned int size()const;//the amount of lists that are linked to the list


							 //Modifiers

	void clear()noexcept;
	void insert(const keyType& srPos, LinkedList1<T, keyType>& val);
	void insert(const keyType& srPo, const keyType& trPos, const T& val);
	void erase(const keyType&);
	void erase(LinkedList1<T, keyType>&);
	void pop_back();
	void pop_front();
	void push_back(LinkedList1<T, keyType>&);
	void push_front(LinkedList1<T, keyType>&);


	//Operations

	void merge(LinkedList1&);
	void reverse();


	//non-member functions

	template <class T2, class keyType2>
	friend bool operator==(const LinkedList1<T2, keyType2>&, const LinkedList1<T2, keyType2>&);
	template <class T2, class keyType2>
	friend bool operator!=(const LinkedList1<T2, keyType2>&, const LinkedList1<T2, keyType2>&);
	template <class T2, class keyType2>
	friend void swap(LinkedList1<T2, keyType2>&, LinkedList1<T2, keyType2>&);
private:
	T m_Data;
	keyType m_Key;
	LinkedList1<T, keyType>* m_Next;
};

template<class T, class keyType>
inline T& LinkedList1<T, keyType>::frontOverloadHelper()
{
	return this->m_Data;
}

template<class T, class keyType>
inline T & LinkedList1<T, keyType>::backOverloadHelper()
{
	if (this->m_Next != nullptr)
	{
		LinkedList1 * ptr = this->m_Next;

		while (ptr->m_Next != nullptr)
		{
			ptr = ptr->m_Next;
		}
		return ptr->m_Data;
	}
	return this->m_Data;
}

template<class T, class keyType>
inline bool LinkedList1<T, keyType>::ValidateListsNeverCross(const LinkedList1<T, keyType>& lhs, const LinkedList1<T, keyType>& rhs) const
{
	LinkedList1<T, keyType> * tmp = lhs.m_Next;
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
inline void LinkedList1<T, keyType>::insertOverloadHelper(LinkedList1<T, keyType>* tmp, LinkedList1<T, keyType>&val)
{
	LinkedList1<T, keyType> * tmp2 = new LinkedList1<T, keyType>(val);
	LinkedList1<T, keyType> * tmp3 = tmp->m_Next;
	tmp->m_Next = tmp2;
	while (tmp->m_Next != nullptr)
	{
		tmp = tmp->m_Next;
	}
	tmp->m_Next = tmp3;
}

template<class T, class keyType>
inline void LinkedList1<T, keyType>::eraseNextElement(LinkedList1<T, keyType>& rhs)
{
	LinkedList1<T, keyType> * tmp2 = rhs.m_Next;
	rhs.m_Next = tmp2->m_Next;
	tmp2->m_Next = nullptr;
	delete tmp2;
}

template<class T, class keyType>
inline void LinkedList1<T, keyType>::eraseOverloadHelper(const keyType & rhs)
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
			LinkedList1<T, keyType> * tmp = this;
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
		std::cerr << "Cannot delete the first element, because the list is empty\n";
		return;
	}
	std::cerr << "Element not found\n";
}

template<class T, class keyType>
inline LinkedList1<T, keyType>::LinkedList1() : m_Key(), m_Data(), m_Next(nullptr)
{
}

template<class T, class keyType>
inline LinkedList1<T, keyType>::LinkedList1(const T & data, const keyType & key) : m_Data(data), m_Key(key), m_Next(nullptr)
{
}

template<class T, class keyType>
inline LinkedList1<T, keyType>::LinkedList1(const T& data, const keyType & key, LinkedList1<T, keyType>& next) : m_Data(data), m_Key(key)
{
	this->m_Next = new LinkedList1<T, keyType>(next);
}

template<class T, class keyType>
inline LinkedList1<T, keyType>::LinkedList1(const T & rhs) : m_Data(rhs), m_Key(), m_Next(nullptr)
{
}

template<class T, class keyType>
inline LinkedList1<T, keyType>::LinkedList1(const LinkedList1<T, keyType> & rhs) : m_Data(rhs.m_Data), m_Key(rhs.m_Key)
{
	if (rhs.m_Next != nullptr)
	{
		this->m_Next = new LinkedList1<T, keyType>(*(rhs.m_Next));
	}
	else
	{
		this->m_Next = nullptr;
	}
}

template<class T, class keyType>
inline LinkedList1<T, keyType> & LinkedList1<T, keyType>::operator=(const LinkedList1<T, keyType> & rhs)
{
	if (ValidateListsNeverCross(*this, rhs))
	{
		if (this != &rhs)
		{
			this->m_Key = rhs.m_Key;
			this->m_Data = rhs.m_Data;
			delete this->m_Next;
			if (rhs.m_Next != nullptr)
			{
				this->m_Next = new LinkedList1<T, keyType>(*(rhs.m_Next));
			}
			else
			{
				this->m_Next = nullptr;
			}
		}
	}
	else
	{
		std::cerr << "Operator = failed, because the two lists cross somewhere\n";
	}
	return *this;
}

template<class T, class keyType>
inline LinkedList1<T, keyType>::~LinkedList1()
{
	delete this->m_Next;
}

template<class T, class keyType>
inline T& LinkedList1<T, keyType>::front()
{
	return frontOverloadHelper();
}

template<class T, class keyType>
inline const T & LinkedList1<T, keyType>::front() const
{
	return frontOverloadHelper();
}

template<class T, class keyType>
inline T & LinkedList1<T, keyType>::back()
{
	return backOverloadHelper();
}

template<class T, class keyType>
inline const T & LinkedList1<T, keyType>::back() const
{
	return backOverloadHelper();
}

template<class T, class keyType>
inline bool LinkedList1<T, keyType>::empty() const
{
	return this->m_Next == nullptr;
}

template<class T, class keyType>
inline unsigned int LinkedList1<T, keyType>::size() const
{
	LinkedList1<T, keyType> * tmp = this->m_Next;
	unsigned int i = 0;
	while (tmp != nullptr)
	{
		++i;
		tmp = tmp->m_Next;
	}
	return i;
}

template<class T, class keyType>
inline void LinkedList1<T, keyType>::clear() noexcept
{
	delete this->m_Next;
	this->m_Next = nullptr;
}

template<class T, class keyType>
inline void LinkedList1<T, keyType>::insert(const keyType & srKey, LinkedList1<T, keyType>& val)
{
	if (ValidateListsNeverCross(*this, val))
	{
		LinkedList1<T, keyType> * tmp = this;
		if (this->m_Key == srKey)
		{
			insertOverloadHelper(tmp, val);
			return;
		}
		while (tmp->m_Next != nullptr)
		{
			if (tmp->m_Next->m_Key == srKey)
			{
				insertOverloadHelper(tmp, val);
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
inline void LinkedList1<T, keyType>::insert(const keyType & srKey, const keyType & valKey, const T & val)
{
	LinkedList1<T, keyType> * tmp = this;
	if (this->m_Key == srKey)
	{
		LinkedList1<T, keyType> * tmp2 = new LinkedList1<T, keyType>(val, valKey);
		insertOverloadHelper(tmp, *tmp2);
		return;
	}
	while (tmp->m_Next != nullptr)
	{
		if (tmp->m_Next->m_Key == srKey)
		{
			LinkedList1<T, keyType> * tmp2 = new LinkedList1<T, keyType>(val, valKey);
			insertOverloadHelper(tmp, *tmp2);
			return;
		}
		tmp = tmp->m_Next;
	}
	std::cerr << "Insert operation failed, no such key has been found\n";
}

template<class T, class keyType>
inline void LinkedList1<T, keyType>::erase(const keyType & rhs)
{
	eraseOverloadHelper(rhs);
}

template<class T, class keyType>
inline void LinkedList1<T, keyType>::erase(LinkedList1<T, keyType>& rhs)
{
	eraseOverloadHelper(rhs.m_Key);
}

template<class T, class keyType>
inline void LinkedList1<T, keyType>::pop_back()
{
	if (this->m_Next == nullptr)
	{
		std::cerr << "List is already empty\n";
	}
	else
	{
		LinkedList1<T, keyType> * tmp = this;
		while (tmp->m_Next->m_Next != nullptr)
		{
			tmp = tmp->m_Next;
		}
		delete tmp->m_Next;
		tmp->m_Next = nullptr;
	}
}

template<class T, class keyType>
inline void LinkedList1<T, keyType>::pop_front()
{
	if (this->m_Next == nullptr)
	{
		std::cerr << "Cannot erase the front element because the list is empty\n";
	}
	else
	{
		this->m_Data = this->m_Next->m_Data;
		this->m_Key = this->m_Next->m_Key;
		LinkedList1<T, keyType> *  tmp = this->m_Next;
		this->m_Next = tmp->m_Next;
		tmp->m_Next = nullptr;
		delete tmp;
	}
}

template<class T, class keyType>
inline void LinkedList1<T, keyType>::push_back(LinkedList1<T, keyType>& rhs)
{
	LinkedList1<T, keyType> * tmp = this;
	while (tmp->m_Next != nullptr)
	{
		tmp = tmp->m_Next;
	}
	tmp->m_Next = new LinkedList1<T, keyType>(rhs.m_Data, rhs.m_Key);
}

template<class T, class keyType>
inline void LinkedList1<T, keyType>::push_front(LinkedList1<T, keyType>& rhs)
{
	LinkedList1<T, keyType> * tmp = this->m_Next;
	this->m_Next = new LinkedList1<T, keyType>(rhs.m_Data, rhs.m_Key);
	this->m_Next->m_Next = tmp;
	swap(*this, *tmp);
}

template<class T, class keyType>
inline void LinkedList1<T, keyType>::merge(LinkedList1 & rhs)
{
	if (ValidateListsNeverCross(*this, rhs))
	{
		LinkedList1<T, keyType> * tmp = this;
		while (tmp->m_Next != nullptr)
		{
			tmp = tmp->m_Next;
		}
		tmp->m_Next = new LinkedList1<T, keyType>(rhs);
	}
	else
	{
		std::cerr << "Function merge() cannot proceed, because the lists cross somewhere\n";
	}
}

template<class T, class keyType>
inline void LinkedList1<T, keyType>::reverse()
{
	LinkedList1<T, keyType> * mover = this;//this is like an iterator moving from left to right
	LinkedList1<T, keyType> * rBorder = this;//this is like iterator's end()
	while (rBorder->m_Next != nullptr)
	{
		rBorder = rBorder->m_Next;
	}
	LinkedList1<T, keyType> * lBorder = mover;//this is like iterator's begin()

	while (lBorder != rBorder)//this is for uneven lists
	{
		while (mover->m_Next != rBorder)
		{
			mover = mover->m_Next;//iterates until it reaches the right border
		}

		swap(*lBorder, *rBorder); //swaps the left and right border

		if (lBorder->m_Next == rBorder)//this is for even lists
		{
			break;
		}

		//tightens the list for reversing
		rBorder = mover;//one step behind
		lBorder = lBorder->m_Next;//one step further
		mover = lBorder;
	}
}

template<class T2, class keyType2>
inline bool operator==(const LinkedList1<T2, keyType2>& lhs, const LinkedList1<T2, keyType2>& rhs)
{
	return (lhs.m_Data == rhs.m_Data && lhs.m_Key == rhs.m_Key);
}

template<class T2, class keyType2>
inline bool operator!=(const LinkedList1<T2, keyType2>& lhs, const LinkedList1<T2, keyType2>& rhs)
{
	return !operator==(lhs, rhs);
}

template<class T2, class keyType2>
inline void swap(LinkedList1<T2, keyType2>& lhs, LinkedList1<T2, keyType2>& rhs)
{
	LinkedList1<T2, keyType2> tmp = lhs;
	lhs.m_Data = rhs.m_Data;
	lhs.m_Key = rhs.m_Key;
	rhs.m_Data = tmp.m_Data;
	rhs.m_Key = tmp.m_Key;
}