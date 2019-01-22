#pragma once
template <class T>
class Queue
{
	void grow();
	void halt();
	unsigned CalculateSize()const;
	unsigned backIndexCalculator()const;
	bool isOverlapped()const;//used to see if there is a cycle in the queue
public:
	Queue();
	Queue(const Queue<T>&);
	Queue<T>& operator = (const Queue<T>&);
	~Queue();


	//Element Access
	T& front();
	const T& front()const;
	T& back();
	const T& back()const;
	unsigned size()const;
	bool empty()const;


	//Modifiers

	void pop();
	void push(T);

private:
	const static char MIN_M_SIZE = 16;//used to guarantee that we won't use too small arrays
	bool m_IsEmpty;
	unsigned m_Back;//the position after the last element
	unsigned m_Front;
	unsigned m_Size;
	T * m_Data;
};

//default constructor
template <class T>
Queue<T>::Queue() : m_IsEmpty(true), m_Back(0), m_Front(0), m_Size(MIN_M_SIZE)
{
	this->m_Data = new T[m_Size];
}

//copy constructor
template <class T>
Queue<T>::Queue(const Queue<T>& rhs) :m_IsEmpty(rhs.m_IsEmpty), m_Front(rhs.m_Front), m_Size(rhs.m_Size), m_Back(rhs.m_Back)
{
	this->m_Data = new T[m_Size];
	for (int i = 0; i < m_Size; i++)
	{
		this->m_Data[i] = rhs.m_Data[i];
	}
}

template <class T>
Queue<T>::~Queue()
{
	delete[] this->m_Data;
}

//operator =
template <class T>
Queue<T>& Queue<T>::operator =(const Queue<T>& old)
{
	if (this != &old)
	{
		this->m_Front = old.m_Front;
		this->m_Back = old.m_Back;
		this->m_Size = old.m_Size;

		delete[] this->m_Data;
		this->m_Data = new T[m_Size];

		for (int i = 0; i < this->m_Size; i++)
		{
			this->m_Data[i] = old.m_Data[i];
		}
	}
	return *this;
}

//if the stackSize of the stack is reached, this will increment the limit
template <class T>
void Queue<T>::grow()
{
	T * newArr = new T[m_Size * 2];

	if (isOverlapped())
	{
		for (unsigned i = this->m_Front; i < this->m_Size; ++i)
		{
			newArr[i - this->m_Front] = this->m_Data[i];
		}
		unsigned startPosition = this->m_Size - this->m_Front;
		for (unsigned i = 0; i < this->m_Back; ++i)
		{
			newArr[startPosition + i] = this->m_Data[i];
		}
		this->m_Front = 0;
	}
	else
	{
		//full without overlap means that we just need to copy all data
		for (unsigned i = 0; i < this->m_Size; ++i)
		{
			newArr[i] = this->m_Data[i];
		}
	}
	delete[] this->m_Data;
	this->m_Data = newArr;
	this->m_Back = this->m_Size;
	this->m_Size *= 2;
}

template<class T>
inline void Queue<T>::halt()
{
	T *newArr = new T[m_Size / 2];
	if (isOverlapped())
	{
		for (unsigned i = this->m_Front; i < this->m_Size; ++i)
		{
			newArr[i - this->m_Front] = this->m_Data[i];
		}
		for (unsigned i = 0; i < this->m_Back; ++i)
		{
			newArr[this->m_Size + i] = this->m_Data[i];
		}
		this->m_Back += this->m_Size - this->m_Front;
	}
	else
	{
		for (unsigned i = this->m_Front; i < this->m_Back; ++i)
		{
			newArr[i - this->m_Front] = this->m_Data[i];
		}
		this->m_Back -= this->m_Front;
	}
	delete[] this->m_Data;
	this->m_Data = newArr;
	this->m_Front = 0;
	this->m_Size /= 2;
}

template<class T>
inline unsigned Queue<T>::CalculateSize() const
{
	if (!isOverlapped()) //if m_Back is 0 then we don't need additional calculations
	{
		return this->m_Back - this->m_Front;
	}
	else
	{
		unsigned frontToEndOfArray = this->m_Size - this->m_Front;	//used to improve code reading

		return this->m_Back + frontToEndOfArray;	//because the distance between m_Back and the start of arr is m_Back - 0
	}
}

template<class T>
inline unsigned Queue<T>::backIndexCalculator() const
{
	if (this->m_Back == 0)
	{
		return this->m_Size - 1;
	}
	else
	{
		return this->m_Back - 1;
	}
}

template<class T>
inline bool Queue<T>::isOverlapped() const
{
	if (this->m_Front < this->m_Back || this->m_Back == 0)
	{
		return false;
	}
	return true;
}

template <class T>
void Queue<T>::push(T a)
{
	if (this->m_Front == this->m_Back)
	{
		if (this->m_IsEmpty == true)
		{
			this->m_IsEmpty = false;
		}
		else
		{
			grow();
		}

	}
	this->m_Data[this->m_Back++] = a;
	if (this->m_Back == this->m_Size)
	{
		this->m_Back = 0;
	}
}

template<class T>
inline T & Queue<T>::front()
{
	return this->m_Data[this->m_Front];
}


//returns the top of the stack and then decrements length
template <class T>
void Queue<T>::pop()
{
	if (m_IsEmpty)
	{
		throw std::out_of_range("trying to pop from empty queue\n");
	}
	else
	{
		if (this->m_Front == this->m_Size - 1)
		{
			this->m_Front = 0;
		}
		else
		{
			++this->m_Front;
		}
		if (this->m_Size > MIN_M_SIZE && this->m_Size / 4 > size())
		{
			halt();
		}
		if (this->m_Back == this->m_Front)
		{
			this->m_Back = 0; //improves performance
			this->m_Front = 0;
			this->m_IsEmpty = true;
		}
	}
}

//returns the top of the array
template <class T>
const T& Queue<T>::front() const
{
	return this->m_Data[this->m_Front];
}

template<class T>
inline T & Queue<T>::back()
{
	return this->m_Data[backIndexCalculator()];
}

template<class T>
inline const T & Queue<T>::back() const
{
	return this->m_Data[backIndexCalculator()];
}

template<class T>
inline unsigned Queue<T>::size() const
{
	if (this->m_IsEmpty && this->m_Front == this->m_Back)
	{
		return 0;
	}												//There are only two cases when front and back are equal
	else if (this->m_Front == this->m_Back)			//when the queue is empty and when it is full
	{
		return this->m_Size;
	}
	else
	{
		return CalculateSize();
	}
}

template <class T>
bool Queue<T>::empty()const
{
	return m_IsEmpty;
}