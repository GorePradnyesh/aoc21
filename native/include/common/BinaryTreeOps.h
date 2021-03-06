/*
**
*/
#pragma once

#include "common/BinaryTree.h"
#include <math.h>
#include <list>
// =============== Stream Operations ===============

template <typename T>
using NodeList = std::list<NodePtr<T>>;

/*
**
*/
template<typename T, typename ST>
void GetNodeStream(const NodePtr<T>& inNode, ST& ss)
{
	if(inNode)
	{
		if(inNode->isLeaf())
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

// =============== Tree Search Operations ===============

// Depth First search for first occurrence using provided predicate function
template <typename T, typename UnaryPredicate>
NodePtr<T> SearchDF(const NodePtr<T>& inNode, UnaryPredicate predicate_op)
{
	// if node matches predicate, return node, else check children
	NodePtr<T> resNode;
	if(predicate_op(inNode))
	{
		resNode = inNode;
	}
	else
	{
		if(inNode->mLeft)
		{
			resNode = SearchDF(inNode->mLeft, predicate_op);
		}
		if(!resNode && inNode->mRight)
		{
			resNode = SearchDF(inNode->mRight, predicate_op);
		}
	}
	return resNode;
}


// =============== Tree Edit Operations ===============



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
	else if(inNode->mRight)
	{
		retNode = GetRightMostLeaf(inNode->mRight);
	}
	else if(inNode->mLeft)
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
** Function walks the ancestory recursively to find the rightmost of a left child.
** if the exclude node is provided then it continues up one level to find the
** left child of the higher node and if found its right most leaf
*/
template <typename T>
NodePtr<T> GetRightmostOfLeftChild(const NodePtr<T>& inNode, const NodePtr<T>& excludeNode)
{
	NodePtr<T> retNode;
	if(inNode && excludeNode)
	{
		if(inNode->mLeft != excludeNode)
		{
			retNode = GetRightMostLeaf(inNode->mLeft);
		}
		else if(inNode->mParent.lock())
		{
			retNode = GetRightmostOfLeftChild(inNode->mParent.lock(), inNode);
		}
	}
	return retNode;
}

/*
**
*/
template <typename T>
NodePtr<T> GetLeftSibling(const NodePtr<T>& inNode)
{
	NodePtr<T> retNode;
	if(inNode->isLeaf() && inNode->mParent.lock())
	{
		// node is leaf node with parent
		retNode = GetRightmostOfLeftChild(inNode->mParent.lock(), inNode);
	}
	return retNode;
}


// =============== Tree Edit Operations ===============


/*
**
*/
template <typename T>
void ReplaceNode(
	const NodePtr<T>& inNodeToBeReplaced,
	const NodePtr<T>& inReplacementNode)
{
	auto nodeDepth = inNodeToBeReplaced->mDepth;
	// Set the depth of the replacement node to be the depth of the original node
	// depth of the children of replcement nodes will also be set in relation to their parents
	inReplacementNode->SetDepth(nodeDepth);
	
	auto parentNode = inNodeToBeReplaced->mParent.lock();
	if(parentNode)
	{
		// fix the child relationship of parent
		if(parentNode->mRight == inNodeToBeReplaced)
		{
			parentNode->mRight = inReplacementNode;
		}
		else if(parentNode->mLeft == inNodeToBeReplaced)
		{
			parentNode->mLeft = inReplacementNode;
		}
		
		// establish parent relationship of child
		inReplacementNode->mParent = parentNode;
	}
	
}

/*
**
*/
template <typename T>
void ReplaceNodeWithZeroLeaf(const NodePtr<T>& inNode)
{
	auto zeroLeaf = Node<T>::CreateNode(0);
	ReplaceNode(inNode, zeroLeaf);
}


