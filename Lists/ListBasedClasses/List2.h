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
		bool operator==(const linkedData<T>*) const;
		bool operator==(const Iterator&)const;
		T& operator*() const;
	};


	//Big 4

	List2();
	List2(const T&);
	List2(const List2<T>&);
	List2<T>& operator=(const List2<T>&);
	~List2();

	//Iterator
	const typename List2<T>::Iterator& begin()const;
	const typename List2<T>::Iterator& end()const;
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
	void push_back(const linkedData<T>&);
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
inline List2<T>::List2(const List2<T>& rhs) : m_Begin(new linkedData<T>(rhs.m_Begin->m_data)), m_End(this->m_Begin), m_Size(rhs.m_Size)
{
	linkedData<T> * tmp = rhs.m_Begin;
	while (tmp->m_next != rhs.m_End)//prefer this way rather than using the m_Size element(seems cleaner)
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
		this->m_Size = rhs.m_Size;
		linkedData<T> * tmp = rhs.m_Begin;
		while (tmp->m_next != rhs.m_End)
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
inline const typename List2<T>::Iterator & List2<T>::begin() const
{
	return List2<T>::Iterator(this->m_Begin);
}

template<class T>
inline const typename List2<T>::Iterator & List2<T>::end() const
{
	return List2<T>::Iterator(this->m_End);
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
inline void List2<T>::push_back(const linkedData<T>&rhs)
{
	this->m_End->m_next = new linkedData<T>(rhs.m_data);
	this->m_End->m_next->m_prev = this->m_End;
	this->m_End = this->m_End->m_next;
	++this->m_Size;
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
