/*
**
*/
#pragma once

#include "common/BinaryTree.h"

#include <vector>

#define PRINT_INTERMEDIATES 0

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
	NodePtr<T> GetFirstToExplode(const NodePtr<T>& inNode)
	{
		const std::uint8_t kExplodeDepthThreshold = 4;
		auto depthExceeds = [](const NodePtr<T>& inNode) -> bool
			{
				if(!inNode->isLeaf())
				{
					return inNode->mDepth >= kExplodeDepthThreshold;
				}
				return false;
			};
		return SearchDF(inNode, depthExceeds);
	}

	template <typename T>
	NodePtr<T> GetFirstToSplit(const NodePtr<T>& inNode)
	{
		const std::uint8_t kSplitThreshold = 9;
		auto depthExceeds = [](const NodePtr<T>& inNode) -> bool
			{
				if(inNode->isLeaf())
				{
					return (inNode->mData > kSplitThreshold);
				}
				return false;
			};
		return SearchDF(inNode, depthExceeds);
	}


	/*
	** DFS to find all nodes which satisfy the predicate
	*/
	template <typename T, typename UnaryPredicate>
	void FindAll(
		const NodePtr<T>& inNode,
		UnaryPredicate predicate_op,
		std::list<NodePtr<T>>& qualifyingNodes)
	{
		if(predicate_op(inNode))
		{
			qualifyingNodes.push_back(inNode);
		}
		if(inNode->mLeft)
		{
			FindAll(inNode->mLeft, predicate_op, qualifyingNodes);
		}
		if(inNode->mRight)
		{
			FindAll(inNode->mRight, predicate_op, qualifyingNodes);
		}
	}

	template <typename T>
	void FindAllExplodes(const NodePtr<T>& inNode, NodeList<T>& ioList)
	{
		const std::uint8_t kExplodeDepthThreshold = 4;
		auto depthExceeds = [](const NodePtr<T>& inNode) -> bool
			{
				if(!inNode->isLeaf())
				{
					return inNode->mDepth >= kExplodeDepthThreshold;
				}
				return false;
			};
		FindAll(inNode, depthExceeds, ioList);
	}

	template <typename T>
	void FindAllSplits(const NodePtr<T>& inNode, NodeList<T> ioList)
	{
		const std::uint8_t kSplitThreshold = 9;
		auto splitCheck = [](const NodePtr<T>& inNode) -> bool
			{
				if(inNode->isLeaf())
				{
					return (inNode->mData > kSplitThreshold);
				}
				return false;
			};
		FindAll(inNode, splitCheck, ioList);
	}
	
	
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
	To reduce a tree, you must repeatedly do the first action in this list that applies to the snailfish number:

	- If any pair is nested inside four pairs, the leftmost such pair explodes.
	- If any regular number is 10 or greater, the leftmost such regular number splits.
	
	Once no action in the above list applies, the snailfish number is reduced.

	During reduction, at most one action applies, after which the process returns to the top of the list of actions.
	For example, if split produces a pair that meets the explode criteria, that pair explodes before other splits occur.
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
				#if PRINT_INTERMEDIATES
					std::cout << "After Explode:\t"; PrintNode(inNode);
				#endif
			}
			
			// if explode action already takes place, one action cap is reached
			// move to top of list again
			if(!didExplode)
			{
				auto splitNode = GetFirstToSplit(inNode);
				if(splitNode)
				{
					Split(splitNode);
					didSplit = true;
					#if PRINT_INTERMEDIATES
						std::cout << "After Split:\t"; PrintNode(inNode);
					#endif
				}
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
		
		#if PRINT_INTERMEDIATES
		std::cout << "After Addition:\t"; PrintNode(newNode);
		#endif
		
		// Process until no new reductions are found
		Process(newNode);
			
		return newNode;
	}
	
	/*
	**
	*/
	template <typename T>
	T GetMagnitude(const NodePtr<T>& inNode)
	{
		if(inNode->isLeaf())
		{
			return inNode->mData;
		}
		else
		{
			return 3 * GetMagnitude(inNode->mLeft) + 2 * GetMagnitude(inNode->mRight);
		}
	}
}
