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
}
