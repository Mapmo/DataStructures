#pragma once
#include "BinarySearchTree.h"

template <class T>
class RedBlackTree : public BinarySearchTree<T>
{

	void InsertBalance(Branch<T>*);//used after insert, therefore it presumes that its param has red color
	void InsertBalanceBlackUncle(Branch<T>*);
	void InsertBalanceRedUncle(Branch<T>*, Branch<T>*, Branch<T>*);
public:
	RedBlackTree(const int = 0, const T & = T());

	void insert(const int, const T&) override;//altough it matches 90% of the original function it still needs to be override
	void insert(const Branch<T>&)override;
};

template<class T>
inline void RedBlackTree<T>::InsertBalance(Branch<T>* x)
{
	if (x->m_parent != this->m_Root)
	{
		if (x->m_parent->m_color != black)
		{
			//using these pointers to avoid code reading complexity
			Branch<T> * grandparent = x->m_parent->m_parent;//x->parent is not root, so we know that grandparent is not nullptr
			Branch<T> * uncle = x->m_parent->isRightChild() ? grandparent->m_left : grandparent->m_right;

			if (uncle != nullptr)
			{
				if (uncle->m_color == red)
				{
					InsertBalanceRedUncle(x, grandparent, uncle);
					return;
				}
			}
			InsertBalanceBlackUncle(x);
		}
	}
}

template<class T>
inline void RedBlackTree<T>::InsertBalanceBlackUncle(Branch<T>*x)
{
	if (x->m_parent->isRightChild() && x->isRightChild())//rr case
	{
		x->m_parent->m_parent->m_color = red;
		x->m_parent->m_color = black;
		BinarySearchTree<T>::RotateLeft(x->m_parent->m_parent);
	}
	else if (x->m_parent->isRightChild())//rl case
	{
		x->m_parent->m_parent->m_color = red;
		x->m_color = black;
		BinarySearchTree<T>::RotateRight(x->m_parent);
		BinarySearchTree<T>::RotateLeft(x->m_parent);//x's parent now is his prev grandparent
	}
	else if (x->isRightChild())//lr case
	{
		x->m_parent->m_parent->m_color = red;
		x->m_color = black;
		BinarySearchTree<T>::RotateLeft(x->m_parent);
		BinarySearchTree<T>::RotateRight(x->m_parent);//x's parent now is his prev grandparent
	}
	else//ll case
	{
		x->m_parent->m_parent->m_color = red;
		x->m_parent->m_color = black;
		BinarySearchTree<T>::RotateRight(x->m_parent->m_parent);
	}
}

template<class T>
inline void RedBlackTree<T>::InsertBalanceRedUncle(Branch<T>*x, Branch<T>*grandparent, Branch<T>* uncle)
{
	x->m_parent->m_color = black;
	uncle->m_color = black;
	if (grandparent != this->m_Root)
	{
		grandparent->m_color = red;
		InsertBalance(grandparent);
	}
}

template<class T>
inline RedBlackTree<T>::RedBlackTree(const int numb, const T &val) : BinarySearchTree<T>(numb, val)
{
	this->m_Root->color = black;
}

template<class T>
inline void RedBlackTree<T>::insert(const int numb, const T & val)
{
	if (this->m_Root == nullptr)
	{
		this->m_Root = new Branch<T>(numb, val);
		this->m_Root->color = black;//rule number 1 root is always black
	}
	else
	{
		Branch<T> * tmp = BinarySearchTree<T>::FindBranchParent(numb);
		bool isRight = tmp->m_key < numb;
		if (BinarySearchTree<T>::GetChild(isRight, tmp) != nullptr)
		{
			std::cerr << "Operation insert failed, because there is already an element with such a key\n";
		}
		else
		{
			if (isRight)
			{
				tmp->m_right = new Branch<T>(numb, val, tmp);
				InsertBalance(tmp->m_right);
			}
			else
			{
				tmp->m_left = new Branch<T>(numb, val, tmp);
				InsertBalance(tmp->m_left);
			}
		}
	}
}

template<class T>
inline void RedBlackTree<T>::insert(const Branch<T>& rhs)
{
	insert(rhs.m_key, rhs.m_val);
}

