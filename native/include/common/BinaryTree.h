/*
**
**
*/
#pragma once
#include <iostream>
#include <memory>
#include <sstream>

template <typename T>
struct Node
{
public:
	Node(const T& inData)
	:mData(inData)
	{}
	
	inline bool isLeaf()
	{
		return !!mData;
	}

	static std::shared_ptr<Node<T>> CreateNode(
		const std::shared_ptr<Node<T>>& inLeftNode,
		const std::shared_ptr<Node<T>>& inRightNode,
		const T& inData)
	{
		std::shared_ptr<Node<T>> newNode
			= std::shared_ptr<Node<T>>(new Node(inLeftNode, inRightNode, inData));
		newNode->mLeft->mParent = newNode;	// create weak linkages
		newNode->mRight->mParent = newNode;	// create weak linkages
		return newNode;
	}
	
	static std::shared_ptr<Node<T>> CreateNode(const T& inData)
	{
		return std::make_shared<Node<T>>(inData);
	}

private:
	Node(
		const std::shared_ptr<Node<T>>& inLeftNode,
		const std::shared_ptr<Node<T>>& inRightNode,
		const T& inData)
	:
	mData(inData),
	mLeft(inLeftNode),
	mRight(inRightNode)
	{}
	
public:
	T mData;
	std::shared_ptr<Node<T>> mLeft;
	std::shared_ptr<Node<T>> mRight;
	
	// ?
	std::weak_ptr<Node<T>> mParent;
};

template <typename T>
using NodePtr = std::shared_ptr<Node<T>>;



