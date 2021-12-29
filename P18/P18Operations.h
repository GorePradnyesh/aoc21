/*
**
*/
#pragma once

#include "common/BinaryTree.h"

#include <vector>


namespace P18
{
	/*
	**
	*/
	bool ProcessLine(const std::string& inLine, NodePtr<int>& outRootNode);
	
	/*
	**
	*/
	void AddListOfNodes(std::vector<NodePtr<int>>& inNodes, NodePtr<int>& finalNode);
	
	
	
	/*
	**
	*/
	template <typename T>
	bool Explode(const NodePtr<T>& inNode)
	{
		if(!inNode->mLeft->isLeaf() || !inNode->mRight->isLeaf())
		{
			std::cout << "Cannot explode parent of non-leaf nodes\n";
			return false;
		}
		
		NodePtr<T> rightNode = GetRightSibling(inNode->mRight);
		NodePtr<T> leftNode = GetLeftSibling(inNode->mLeft);
		
		if(rightNode)
		{
			rightNode->mData += inNode->mRight->mData;
			// TODO: Needs split ?
		}
		if(leftNode)
		{
			leftNode->mData += inNode->mLeft->mData;
			// TODO: Needs split ?
		}
		
		// Replace with Zero leaf
		ReplaceNodeWithZeroLeaf(inNode);
		
		return true;
	}

	/*
	**
	*/
	template <typename T>
	void Split(const NodePtr<T>& inNode)
	{
		auto dataValue = inNode->mData;
		
		auto leftValue = static_cast<T>(dataValue/2);
		auto leftNode = Node<T>::CreateNode(leftValue);
		
		auto rightValue = static_cast<T>((dataValue+1)/2);
		auto rightNode = Node<T>::CreateNode(rightValue);
		
		auto joinedNode = Node<T>::CreateNode(leftNode, rightNode);
		ReplaceNode(inNode, joinedNode);
	}

	/*
	**
	*/
	template <typename T>
	void Process(const NodePtr<T>& inNode)
	{
		bool treeChanged = false;
		do
		{
			auto explodeNode = GetFirstToExplode(inNode);
			
			bool didExplode = false;
			bool didSplit = false;
			
			if(explodeNode)
			{
				didExplode = Explode(explodeNode);
				std::cout << "After Explode:\t"; PrintNode(inNode);
			}
			auto splitNode = GetFirstToSplit(inNode);
			if(splitNode)
			{
				Split(splitNode);
				didSplit = true;
				std::cout << "After Split:\t"; PrintNode(inNode);
			}
			treeChanged = didExplode || didSplit;
		}while(treeChanged);
		// if something changed. Check again
	}
	
	/*
	**
	*/
	template <typename T>
	NodePtr<T> Add(const NodePtr<T>& input1, const NodePtr<T>& input2)
	{
		// Step 1.
		// Nest the inputs under 1 node
		auto newNode = Node<T>::CreateNode(input1, input2);
		
		std::cout << "After Addition:\t"; PrintNode(newNode);
		
		// Process until no new reductions are found
		Process(newNode);
			
		return newNode;
	}
}
