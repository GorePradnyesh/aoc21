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

	
	void UpdateDepth(std::uint8_t inDepth)
	{
		mDepth += inDepth;
		if(mLeft)
		{
			mLeft->UpdateDepth(inDepth);
		}
		if(mRight)
		{
			mRight->UpdateDepth(inDepth);
		}
	}

	// static
	static std::shared_ptr<Node<T>> CreateNode(
		const std::shared_ptr<Node<T>>& inLeftNode,
		const std::shared_ptr<Node<T>>& inRightNode,
		const T& inData)
	{
		std::shared_ptr<Node<T>> newNode
			= std::shared_ptr<Node<T>>(new Node(inLeftNode, inRightNode, inData));
		//Create weak parent linkages
		newNode->mLeft->mParent = newNode;
		newNode->mRight->mParent = newNode;
		
		//increase depth by 1
		newNode->mLeft->UpdateDepth(1);
		newNode->mRight->UpdateDepth(1);
		
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
	std::uint8_t mDepth { 0 };
	// ?
	std::weak_ptr<Node<T>> mParent;
};

template <typename T>
using NodePtr = std::shared_ptr<Node<T>>;



