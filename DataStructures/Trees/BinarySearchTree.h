#pragma once
#include <functional>//used for throw bad_function_call()

enum Color { doubleBlack = -1, black = 0, red = 1 };

template <class T>
struct Branch
{
	Branch(const int = 0, const T& = T(), Branch<T>* = nullptr);
	~Branch();
	void swap(Branch<T>*);
	bool isRightChild()const;

	int m_key;
	Color m_color = red;//not used in BinarySearchTree
	T m_val;
	Branch<T> * m_left;
	Branch<T> * m_right;
	Branch<T> * m_parent;
};

template <class T>
class BinarySearchTree
{
protected:
	BinarySearchTree(const BinarySearchTree&) = delete;
	const BinarySearchTree<T>& operator=(const BinarySearchTree&) = delete;


	//OverloadHelpers

	T& GetByKeyOverloadHelper(const int);
	Branch<T>* FindBranchOverloadHelper(const int);
	Branch<T>* FindBranchParentOverloadHelper(const int);
	Branch<T>*GetChildOverloadHelper(const bool, const Branch<T>*);


	//functions related to erase

	void SimpleErase(Branch<T>*);//simple erase means that at least one of the children of the branch is nullptr
	void SimpleEraseRoot();
	void SimpleEraseLeftSlip(const bool, Branch<T>*);//means that the left child is not nullptr
	void SimpleEraseRightSlip(const bool, Branch<T>*);//means that the left child is not nullptr
	void ComplicatedErase(Branch<T>*);//complicated erase means that both children of the branch are not nullptr


   //others

	Branch<T>*GetChild(const bool, const Branch<T>*);//if true returns right child
	const Branch<T>*GetChild(const bool, const Branch<T>*)const;
	Branch<T> * FindBiggestLeftChild(Branch<T>*);//returns the biggest child of the ->left branch of the param

	void RotateRight(Branch<T>*);//the param is the first element that is going to be rotated
	void RotateLeft(Branch<T>*);//the param is the first element that is going to be rotated
public:
	//Essentials

	BinarySearchTree(const int = 0, const T & = T());
	virtual ~BinarySearchTree();


	//Element Access
	T& GetByKey(const int);
	const T& GetByKey(const int)const;
	Branch<T> * FindBranch(const int);//returns nullptr if element was not found
	const Branch<T> * FindBranch(const int)const;
	Branch<T> * FindBranchParent(const int);//if it returns nullptr, then the tree is empty or the searched elements is the root
	const Branch<T> * FindBranchParent(const int)const;

	//Modifiers
	virtual void insert(const int, const T&);
	virtual void insert(const Branch<T>&);
	void erase(const int);
protected:
	Branch<T> * m_Root;
};

template<class T>
inline Branch<T>::Branch(const int k, const T & v, Branch<T>* p) : m_key(k), m_val(v), m_parent(p), m_left(nullptr), m_right(nullptr)
{
}

template<class T>
inline Branch<T>::~Branch()
{
	delete m_right;
	delete m_left;
}

template<class T>
inline void Branch<T>::swap(Branch<T>*rhs)
{
	std::swap(m_key, rhs->m_key);
	std::swap(m_val, rhs->m_val);
}

template<class T>
inline bool Branch<T>::isRightChild()const
{
	try
	{
		if (m_parent != nullptr)
			return this == this->m_parent->m_right;
		else
			throw std::bad_function_call();
	}
	catch (std::bad_function_call &bfc)
	{
		std::cerr << "bad_function_call exception thrown: (trying to find the parent of the root\n";
	}
}

template<class T>
inline T & BinarySearchTree<T>::GetByKeyOverloadHelper(const int numb)
{
	try
	{
		Branch<T> * tmp = FindBranch(numb);
		if (tmp == nullptr)
		{
			throw std::invalid_argument("no element with such a key found\n");
		}
		else
		{
			return tmp->m_val;
		}
	}
	catch (std::invalid_argument& ia)
	{
		std::cerr << "GetByKey() threw an exception: " << ia.what();
	}
}

template<class T>
inline Branch<T>* BinarySearchTree<T>::FindBranchOverloadHelper(const int numb)
{
	Branch<T> * tmp = FindBranchParentOverloadHelper(numb);//the parent of the searched element
	if (tmp == nullptr)
	{
		return (this->m_Root == nullptr) ? nullptr : this->m_Root;
	}
	return (tmp->m_key > numb) ? tmp->m_left : tmp->m_right;
}

template<class T>
inline Branch<T>* BinarySearchTree<T>::FindBranchParentOverloadHelper(const int numb)
{
	Branch<T> * tmp = this->m_Root;
	Branch<T> * tmp2 = nullptr;
	while (tmp != nullptr)
	{
		if (tmp->m_key == numb)
		{
			return tmp2;
		}
		else
		{
			tmp2 = tmp;
			if (tmp->m_key > numb)
			{
				tmp = tmp->m_left;
			}
			else
			{
				tmp = tmp->m_right;
			}
		}
	}
	return tmp2;
}

template<class T>
inline Branch<T>* BinarySearchTree<T>::GetChildOverloadHelper(const bool r, const Branch<T>* rhs)
{
	return r ? rhs->m_right : rhs->m_left;
}

template<class T>
inline void BinarySearchTree<T>::SimpleErase(Branch<T>*rhs)
{
	if (rhs == this->m_Root)
	{
		SimpleEraseRoot();
	}
	else
	{
		if (rhs->m_left != nullptr)
		{
			SimpleEraseLeftSlip(rhs->isRightChild(), rhs);
		}
		else if (rhs->m_right != nullptr)
		{
			SimpleEraseRightSlip(rhs->isRightChild(), rhs);
		}
		else
		{
			rhs->isRightChild() ? rhs->m_parent->m_right = nullptr : rhs->m_parent->m_left = nullptr;
			delete rhs;
		}
	}
}

template<class T>
inline void BinarySearchTree<T>::SimpleEraseRoot()
{
	if (this->m_Root->m_left != nullptr)
	{
		Branch<T> * tmp = this->m_Root;
		this->m_Root = this->m_Root->m_left;
		this->m_Root->m_parent = nullptr;
		tmp->m_left = nullptr;
		delete tmp;
	}
	else if (this->m_Root->m_right != nullptr)
	{
		Branch<T> * tmp = this->m_Root;
		this->m_Root = this->m_Root->m_right;
		this->m_Root->m_parent = nullptr;
		tmp->m_right = nullptr;
		delete tmp;
	}
	else
	{
		delete this->m_Root;
		this->m_Root = nullptr;
	}
}

template<class T>
inline void BinarySearchTree<T>::SimpleEraseLeftSlip(const bool isRightChild, Branch<T>* rhs)
{
	isRightChild ? rhs->m_parent->m_right = rhs->m_left : rhs->m_parent->m_left = rhs->m_left;
	rhs->m_left->m_parent = rhs->m_parent;
	rhs->m_left = nullptr;
	delete rhs;
}

template<class T>
inline void BinarySearchTree<T>::SimpleEraseRightSlip(const bool isRightChild, Branch<T>* rhs)
{
	isRightChild ? rhs->m_parent->m_right = rhs->m_right : rhs->m_parent->m_left = rhs->m_right;
	rhs->m_right->m_parent = rhs->m_parent;
	rhs->m_right = nullptr;
	delete rhs;
}

template<class T>
inline void BinarySearchTree<T>::ComplicatedErase(Branch<T>* rhs)
{
	Branch<T>* biggestLeftChild = FindBiggestLeftChild(rhs);
	rhs->swap(biggestLeftChild);
	SimpleErase(biggestLeftChild);
}

template<class T>
inline Branch<T>* BinarySearchTree<T>::GetChild(const bool r, const Branch<T>* rhs)
{
	return GetChildOverloadHelper(r, rhs);
}

template<class T>
inline const Branch<T>* BinarySearchTree<T>::GetChild(const bool r, const Branch<T>* rhs) const
{
	return GetChildOverloadHelper(r, rhs);
}

template<class T>
inline Branch<T>* BinarySearchTree<T>::FindBiggestLeftChild(Branch<T> * rhs)
{
	Branch<T> * tmp = rhs->m_left;
	while (tmp->m_right != nullptr)
	{
		tmp = tmp->m_right;
	}
	return tmp;
}

template<class T>
inline void BinarySearchTree<T>::RotateRight(Branch<T>* rhs)
{
	Branch<T>*eldest = rhs->m_parent;//the original parent of rhs has a very important role, using this to reduce code complexity
	rhs->m_parent = rhs->m_left;
	rhs->m_parent->m_parent = eldest;
	if (eldest != nullptr)
	{
		eldest->m_right == rhs ? eldest->m_right = rhs->m_parent : eldest->m_left = rhs->m_parent;
	}
	else
	{
		this->m_Root = rhs->m_parent;
	}
	Branch<T>* tmp = rhs->m_parent->m_right;
	rhs->m_parent->m_right = rhs;
	rhs->m_left = tmp;
	if (tmp != nullptr)
	{
		tmp->m_parent = rhs;
	}
}
template<class T>
inline void BinarySearchTree<T>::RotateLeft(Branch<T>*rhs)
{
	Branch<T>*eldest = rhs->m_parent;//the original parent of rhs has a very important role, using this to reduce code complexity
	rhs->m_parent = rhs->m_right;
	rhs->m_parent->m_parent = eldest;
	if (eldest != nullptr)
	{
		eldest->m_right == rhs ? eldest->m_right = rhs->m_parent : eldest->m_left = rhs->m_parent;
	}
	else
	{
		this->m_Root = rhs->m_parent;
	}
	Branch<T>* tmp = rhs->m_parent->m_left;
	rhs->m_parent->m_left = rhs;
	rhs->m_right = tmp;
	if (tmp != nullptr)
	{
		tmp->m_parent = rhs;
	}
}

template<class T>
inline BinarySearchTree<T>::BinarySearchTree(const int k, const T & v) : m_Root(new Branch<T>(k, v))
{
}

template<class T>
inline BinarySearchTree<T>::~BinarySearchTree()
{
	delete this->m_Root;
}

template<class T>
inline T & BinarySearchTree<T>::GetByKey(const int numb)
{
	return GetByKeyOverloadHelper(numb);
}

template<class T>
inline const T & BinarySearchTree<T>::GetByKey(const int numb) const
{
	return GetByKeyOverloadHelper(numb);
}

template<class T>
inline Branch<T>* BinarySearchTree<T>::FindBranch(const int numb)
{
	return FindBranchOverloadHelper(numb);
}

template<class T>
inline const Branch<T>* BinarySearchTree<T>::FindBranch(const int numb) const
{
	return FindBranchOverloadHelper(numb);
}

template<class T>
inline Branch<T>* BinarySearchTree<T>::FindBranchParent(const int numb)
{
	return FindBranchParentOverloadHelper(numb);
}

template<class T>
inline const Branch<T>* BinarySearchTree<T>::FindBranchParent(const int numb) const
{
	return FindBranchParentOverloadHelper(numb);
}

template<class T>
inline void BinarySearchTree<T>::insert(const int numb, const T & val)
{
	if (this->m_Root == nullptr)
	{
		this->m_Root = new Branch<T>(numb, val);
	}
	else
	{
		Branch<T> * tmp = FindBranchParent(numb);
		bool isRight = tmp->m_key < numb;
		if (GetChild(isRight, tmp) != nullptr)
		{
			std::cerr << "Operation insert failed, because there is already an element with such a key\n";
		}
		else
		{
			if (isRight)
			{
				tmp->m_right = new Branch<T>(numb, val, tmp);
			}
			else
			{
				tmp->m_left = new Branch<T>(numb, val, tmp);
			}
		}
	}
}

template<class T>
inline void BinarySearchTree<T>::insert(const Branch<T>&rhs)
{
	insert(rhs.m_key, rhs.m_val);
}

template<class T>
inline void BinarySearchTree<T>::erase(const int numb)
{
	Branch<T> * tmp = FindBranch(numb);
	if (tmp != nullptr)
	{
		if (tmp->m_left == nullptr || tmp->m_right == nullptr)
		{
			SimpleErase(tmp);
		}
		else
		{
			ComplicatedErase(tmp);
		}
	}
}
