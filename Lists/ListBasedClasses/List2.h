#pragma once

template <class T>
struct linkedData
{
	linkedData(const T&, linkedData* = nullptr, linkedData* = nullptr);
	linkedData(const linkedData&) = delete;
	linkedData&operator=(const linkedData&) = delete;
	~linkedData();
	T m_data;
	linkedData * m_next;
	linkedData * m_prev;
};

template<class T>
inline linkedData<T>::linkedData(const T & data, linkedData * next, linkedData * prev)
	: m_data(data), m_next(next), m_prev(prev)
{
}

template<class T>
inline linkedData<T>::~linkedData()
{
	if (this->m_next != nullptr)
	{
		m_next->m_prev = nullptr;
		delete m_next;
	}
	if (this->m_prev != nullptr)
	{
		m_prev->m_next = nullptr;
		delete m_prev;
	}
}

template <class T>
class List2
{
	T& frontOverloadHelper();
	T& backOverloadHelper();
	void insertMultipleElements(linkedData<T>*,linkedData<T>*, const unsigned int, const T&, const bool);
public:
	//iterator 

	class Iterator
	{
	private:
		linkedData<T>* tmp;
	public:
		Iterator(linkedData<T>*);
		bool operator!=(const linkedData<T>*)const;
		bool operator!=(const Iterator&)const;
		Iterator& operator++();
		Iterator& operator--();
		Iterator operator+(const unsigned int);
		Iterator operator-(const unsigned int);
		bool operator==(const linkedData<T>*) const;
		bool operator==(const Iterator&)const;
		T& operator*() const;
		friend linkedData<T>* List2::itData(const typename List2<T>::Iterator&);
	};


	//Big 4

	List2();
	List2(const T&);
	List2(const List2<T>&);
	List2<T>& operator=(const List2<T>&);
	~List2();


	//Iterator

	typename List2<T>::Iterator begin()const;
	typename List2<T>::Iterator end()const;


	//used to get the tmp class member of the iterator, needs to be public, so it can be a friend to Iterator
	linkedData<T> * itData(const typename List2<T>::Iterator&);


	//Element Access

	T& front();
	const T& front()const;
	T&back();
	const T& back()const;


	//Capacity

	bool empty() const;
	unsigned int size()const;


	//Modifiers

	void clear();
	void insert(const typename List2<T>::Iterator&, const T&);//had to note that insert cannot add a last element, if you want to do so, use push_back
	void insert(const typename List2<T>::Iterator&, const unsigned int, const T&);
	void pop_back();
	void pop_front();
	void push_back(const linkedData<T>&);
	void push_front(const linkedData<T>&);

	//non-class members
	template <class T1>
	friend bool operator==(const List2<T1>&, const List2<T1>&);
	template <class T1>
	friend bool operator!=(const List2<T1>&, const List2<T1>&);
private:
	unsigned int m_Size;
	linkedData<T> * m_Begin;
	linkedData<T> * m_End;
};

template<class T>
inline T & List2<T>::frontOverloadHelper()
{
	return this->m_Begin->m_data;
}

template<class T>
inline T & List2<T>::backOverloadHelper()
{
	return this->m_End->m_data;
}

template<class T>
inline List2<T>::List2() : m_Begin(nullptr), m_End(nullptr), m_Size(0)
{
}

template<class T>
inline List2<T>::List2(const T & val) : m_Begin(new linkedData<T>(val)), m_End(this->m_Begin), m_Size(1)
{
}

template<class T>
inline List2<T>::List2(const List2<T>& rhs) : m_Begin(new linkedData<T>(rhs.m_Begin->m_data)), m_End(this->m_Begin), m_Size(1)
{
	linkedData<T> * tmp = rhs.m_Begin;
	while (tmp != rhs.m_End)//prefer this way rather than using the m_Size element(seems cleaner)
	{
		tmp = tmp->m_next;
		push_back(*tmp);
	}
}

template<class T>
inline List2<T> & List2<T>::operator=(const List2<T> & rhs)
{
	if (this != &rhs)
	{
		delete this->m_Begin;
		this->m_Begin = new linkedData<T>(rhs.m_Begin->m_data);
		this->m_End = this->m_Begin;
		this->m_Size = 1;
		linkedData<T> * tmp = rhs.m_Begin;
		while (tmp != rhs.m_End)
		{
			tmp = tmp->m_next;
			push_back(*tmp);
		}
	}
	return *this;
}

template<class T>
inline List2<T>::~List2()
{
	delete this->m_Begin;
}

template<class T>
inline typename List2<T>::Iterator List2<T>::begin() const
{
	return List2<T>::Iterator(this->m_Begin);
}

template<class T>
inline typename List2<T>::Iterator List2<T>::end() const
{
	return List2<T>::Iterator(this->m_End);
}

template<class T>
inline linkedData<T>* List2<T>::itData(const typename List2<T>::Iterator& it)
{
	return it.tmp;
}

template<class T>
inline void List2<T>::insertMultipleElements(linkedData<T>* posDataList, linkedData<T>* lastNewElement, const unsigned int count, const T& val, const bool emptyList)
{
	linkedData<T> * tmp;
	for (unsigned int i = 1; i < count; ++i)
	{
		tmp = new linkedData<T>(val);
		lastNewElement->m_next = tmp;
		tmp->m_prev = lastNewElement;
		lastNewElement = tmp;
	}

	if (!emptyList)
	{
		lastNewElement->m_next = posDataList;
		posDataList->m_prev = lastNewElement;
	}
	else
	{
		this->m_End = lastNewElement;
	}
}

template<class T>
inline T & List2<T>::front()
{
	return frontOverloadHelper();
}

template<class T>
inline const T & List2<T>::front() const
{
	return frontOverloadHelper();
}

template<class T>
inline T & List2<T>::back()
{
	return backOverloadHelper();
}

template<class T>
inline const T & List2<T>::back() const
{
	return backOverloadHelper();
}

template<class T>
inline bool List2<T>::empty() const
{
	return this->m_Begin == nullptr;
}

template<class T>
inline unsigned int List2<T>::size() const
{
	return this->m_Size;
}

template<class T>
inline void List2<T>::clear()
{
	delete this->m_End;
	this->m_Begin = nullptr;
	this->m_End = nullptr;
	this->m_Size = 0;
}

template<class T>
inline void List2<T>::insert(const typename List2<T>::Iterator & pos, const T & val)
{
	++this->m_Size;
	linkedData<T> * tmp = itData(pos);
	linkedData<T> * tmp2 = new linkedData<T>(val);
	if (tmp != nullptr)
	{
		if (tmp == this->m_Begin)
		{
			this->m_Begin = tmp2;
		}
		else
		{
			//this code is separated here, because it will be erone prone if pos == begin
			tmp->m_prev->m_next = tmp2;
			tmp2->m_prev = tmp->m_prev;
		}
		tmp2->m_next = tmp;
		tmp->m_prev = tmp2;
	}
	else
	{
		this->m_Begin = tmp2;
		this->m_End = tmp2;
	}
}

template<class T>
inline void List2<T>::insert(const typename List2<T>::Iterator & pos, const unsigned int count, const T & val)
{
	if (count != 0)
	{
		this->m_Size += count;
		linkedData<T> * posDataList = itData(pos);
		linkedData<T> * firstNewElement = new linkedData<T>(val);
		linkedData<T> * lastNewElement = firstNewElement;
		bool emptyList = false;//used at the end so that the last new element can be specified as m_Last

		if (posDataList != nullptr)
		{
			if (posDataList == this->m_Begin)
			{
				this->m_Begin = firstNewElement;
			}
			else
			{
				posDataList->m_prev->m_next = firstNewElement;
				firstNewElement->m_prev = posDataList->m_prev;
			}
		}
		else
		{
			this->m_Begin = firstNewElement;
			emptyList = true;
		}

		insertMultipleElements(posDataList, lastNewElement, count, val, emptyList);

		
	}
}

template<class T>
inline void List2<T>::pop_back()
{
	try
	{
		if (empty())
		{
			throw std::domain_error("list is empty\n");
		}
		else
		{
			if (this->m_End == this->m_Begin)
			{
				clear();
			}
			else
			{
				this->m_End->m_prev->m_next = nullptr;
				linkedData<T> * tmp = this->m_End->m_prev;
				this->m_End->m_prev = nullptr;
				delete this->m_End;
				this->m_End = tmp;
				--this->m_Size;
			}
		}
	}
	catch (std::domain_error & de)
	{
		std::cerr << de.what();
	}
}

template<class T>
inline void List2<T>::pop_front()
{
	try
	{
		if (empty())
		{
			throw std::domain_error("list is empty\n");
		}
		else
		{
			if (this->m_End == this->m_Begin)
			{
				clear();
			}
			else
			{
				this->m_Begin->m_next->m_prev = nullptr;
				linkedData<T> * tmp = this->m_Begin->m_next;
				this->m_Begin->m_next = nullptr;
				delete this->m_Begin;
				this->m_Begin = tmp;
				--this->m_Size;
			}
		}
	}
	catch (std::domain_error & de)
	{
		std::cerr << de.what();
	}
}

template<class T>
inline void List2<T>::push_back(const linkedData<T>&rhs)
{
	this->m_End->m_next = new linkedData<T>(rhs.m_data);
	this->m_End->m_next->m_prev = this->m_End;
	this->m_End = this->m_End->m_next;
	++this->m_Size;
}

template<class T>
inline void List2<T>::push_front(const linkedData<T>& rhs)
{
	this->m_Begin->m_prev = new linkedData<T>(rhs.m_data);
	this->m_Begin->m_prev->m_next = this->m_Begin;
	this->m_Begin = this->m_Begin->m_prev;
	++this->m_Size;
}

template<class T>
inline bool operator==(const List2<T>& lhs, const List2<T>& rhs)
{
	if (lhs.m_Size != rhs.m_Size)
	{
	return false;
	}
	else
	{
		typename List2<T>::Iterator tmp1 = lhs.begin();
		typename List2<T>::Iterator tmp2 = rhs.begin();
		for (unsigned int i = 0; i < lhs.m_Size; ++i)
		{
			if (*tmp1 != *tmp2)
			{
				return false;
			}
			else if (i = lhs.m_Size - 1)
			{
				return true;
			}
			else
			{
				++tmp1;
				++tmp2;
			}
		}
	}
}

template<class T1>
inline bool operator!=(const List2<T1>&lhs, const List2<T1>&rhs)
{
	return !(lhs==rhs);
}

template<class T>
inline List2<T>::Iterator::Iterator(linkedData<T>*rhs) : tmp(rhs)
{
}

template<class T>
inline bool List2<T>::Iterator::operator!=(const linkedData<T>* rhs) const
{
	return this->tmp != rhs;
}

template<class T>
inline bool List2<T>::Iterator::operator!=(const Iterator & rhs)const
{
	return this->operator !=(rhs.tmp);
}

template<class T>
inline typename List2<T>::Iterator& List2<T>::Iterator::operator++()
{
	try
	{
		if (this->tmp->m_next == nullptr)
		{
			throw std::out_of_range("iterator oor\n");
		}
		this->tmp = this->tmp->m_next;
	}
	catch (std::out_of_range & oor)
	{
		std::cerr << oor.what();
	}
	return  *this;
}

template<class T>
inline typename List2<T>::Iterator& List2<T>::Iterator::operator--()
{
	try
	{
		if (this->tmp->m_prev == nullptr)
		{
			throw std::out_of_range("iterator oor\n");
		}
		this->tmp = this->tmp->m_prev;
	}
	catch (std::out_of_range & oor)
	{
		std::cerr << oor.what();
	}
	return *this;
}

template<class T>
inline typename List2<T>::Iterator List2<T>::Iterator::operator+(const unsigned int numb)
{
	typename List2<T>::Iterator tmpIt(this->tmp);
	for (unsigned int i = 0; i < numb; ++i)
	{
		++tmpIt;
	}
	return tmpIt;
}

template<class T>
inline typename List2<T>::Iterator List2<T>::Iterator::operator-(const unsigned int numb)
{
	typename List2<T>::Iterator tmpIt(this->tmp);
	for (unsigned int i = 0; i < numb; ++i)
	{
		--tmpIt;
	}
	return tmpIt;
}

template<class T>
inline bool List2<T>::Iterator::operator==(const linkedData<T>*rhs) const
{
	return !this->operator!=(rhs);
}

template<class T>
inline bool List2<T>::Iterator::operator==(const Iterator & rhs) const
{
	return !this->operator!=(rhs);
}

template<class T>
inline T& List2<T>::Iterator::operator*() const
{
	return this->tmp->m_data;
}
