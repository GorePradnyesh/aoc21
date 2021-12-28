/*
**
*/
#pragma once

#include "common/BinaryTree.h"

// =============== Stream Operations ===============

/*
**
*/
template<typename T, typename ST>
void GetNodeStream(const NodePtr<T>& inNode, ST& ss)
{
	if(inNode)
	{
		if(inNode->mData)
		{
			ss << inNode->mData;
		}
		else
		{
			ss << "[";
			if(inNode->mLeft)
			{
				GetNodeStream(inNode->mLeft, ss);
				ss << ",";
			}
			if(inNode->mRight)
			{
				GetNodeStream(inNode->mRight, ss);
			}
			ss << "]";
		}
	} // if inNode
}

/*
**
*/
template<typename T>
void PrintNode(const NodePtr<T>& inNode)
{
	std::stringstream ss;
	GetNodeStream(inNode, ss);
	std::cout << ss.str() << "\n";
}

/*
**
*/
template <typename T>
std::ostream& operator<<(std::ostream& os, const NodePtr<T>& inNode)
{
	GetNodeStream<int, std::ostream>(inNode, os);
	return os;
}

// =============== Tree Edit Operations ===============

/*
**
*/
template <typename T>
bool AddLeft(const NodePtr<T>& node, const NodePtr<T>& inOther)
{
	if(node->mLeft)
	{
		std::cout << "Bad Op. Node already contains left \n";
		return false;
	}
	node->mLeft = inOther;
	return false;
}

/*
**
*/
template <typename T>
bool AddRight(const NodePtr<T>& node, const NodePtr<T>& inOther)
{
	if(node->mRight)
	{
		std::cout << "Bad Op. Node already contains left \n";
		return false;
	}
	node->mRight = inOther;
	return false;
}

// =============== Tree Navigation Operations ===============

/*
** If inNode is a leaf node, return inNode itself.
** otherwise recursively hunt for left most leaf
** IMPORTANT: this assumes that if a non-leaf node exists it is bound to have a balanced structure
** i.e. a non-leaf node always either has a leaf child or non-leaf child.
*/
template <typename T>
NodePtr<T> GetLeftMostLeaf(const NodePtr<T>& inNode)
{
	NodePtr<T> retNode;
	if(inNode->isLeaf())
	{
		retNode = inNode;
	}
	else if(inNode->mLeft)
	{
		retNode = GetLeftMostLeaf(inNode->mLeft);
	}
	else if(inNode->mRight)
	{
		retNode = GetLeftMostLeaf(inNode->mLeft);
	}
	return retNode;
}

/*
**
*/
template <typename T>
NodePtr<T> GetRightMostLeaf(const NodePtr<T>& inNode)
{
	NodePtr<T> retNode;
	if(inNode->isLeaf())
	{
		retNode = inNode;
	}
	else if(inNode->mLeft)
	{
		retNode = GetRightMostLeaf(inNode->mLeft);
	}
	else if(inNode->mRight)
	{
		retNode = GetRightMostLeaf(inNode->mLeft);
	}
	return retNode;
}

/*
** This function returns the right sibling of a given leaf node
** It walks up the parent, while maintaining the last visited child.
** returns empty pointer if no right sibling is found. O(log n)
*/
template <typename  T>
NodePtr<T> GetLeftmostOfRightChild(const NodePtr<T>& inNode, const NodePtr<T>& excludeNode)
{
	NodePtr<T> retNode;
	if(inNode && excludeNode)
	{
		if(inNode->mRight != excludeNode)
		{
			retNode = GetLeftMostLeaf(inNode->mRight);
		}
		else if(inNode->mParent.lock())
		{
			retNode = GetLeftmostOfRightChild(inNode->mParent.lock(), inNode);
		}
	}
	return retNode;
}

/*
**
*/
template <typename T>
NodePtr<T> GetRightSibling(const NodePtr<T>& inNode)
{
	NodePtr<T> retNode;
	if(inNode->isLeaf() && inNode->mParent.lock())
	{
		// node is leaf node with parent
		retNode = GetLeftmostOfRightChild(inNode->mParent.lock(), inNode);
	}
	return retNode;
}

/*
**
*/
template <typename T>
NodePtr<T> GetRightmostOfLeftChild(const NodePtr<T>& inNode, const NodePtr<T>& excludeNode)
{
	NodePtr<T> retNode;
	return retNode;
}
