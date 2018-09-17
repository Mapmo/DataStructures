#pragma once

template <class T>
struct linkedData
{
	linkedData(const T&, const int, linkedData* = nullptr, linkedData* = nullptr);
	linkedData(const linkedData&) = delete;
	linkedData&operator=(const linkedData&) = delete;
	~linkedData();
	T m_data;
	const int m_key;
	linkedData * m_next;
	linkedData * m_prev;
};

template<class T>
inline linkedData<T>::linkedData(const T & data, const int key, linkedData * next, linkedData * prev) 
	: m_data(data), m_key(key), m_next(next), m_prev(prev)
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
