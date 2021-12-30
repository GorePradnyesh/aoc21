#include "common/BinaryTreeOps.h"

#include "P18Operations.h"

#include <stack>
#include <vector>

namespace P18
{

	// TODO: tokenization code assumes int data type
	// TODO: tokenization code assumes all ints are [0,9]. will not work for values above 10
	// TODO: tokenization also assumes well formed lines ( complete, balanced and uncorrupted )
	/*
	** Stack based processing of the tokens encountered
	*/
	bool ProcessLine(const std::string& inLine, NodePtr<int>& outRootNode)
	{
		std::stack<char> paran_stack; // holds '['
		std::stack<NodePtr<int>> node_stack;
	
		for(size_t index = 0; index < inLine.size(); index++)
		{
			char token = inLine[index];
			if(token == '[')
			{
				paran_stack.push(token);
			}
			else if(isdigit(token))
			{
				std::string token_str(&token, 1);
				// this ensures that we only take a single char.
				// by ensuring that string is only single char followed by null terminatioon
				auto value = std::atoi(token_str.c_str());
				auto node = std::make_shared<Node<int>>(value);
				node_stack.push(node);
			}
			else if(token == ']')
			{
				if(node_stack.size() == 1)
				{
					// we might have reached the end of string
					// OR we have balanced parans. Do nothing in this block
				}
				else
				{
					auto op2 = node_stack.top(); node_stack.pop();
					auto op1 = node_stack.top(); node_stack.pop();
					
					//NOTE the order of operand args to the node/
					auto nonLeafNode = Node<int>::CreateNode(op1, op2);
					node_stack.push(nonLeafNode);
				}
				paran_stack.pop();
			}
			else if(token == ',')
			{
				; // valid char. Ignore.
			}
			else
			{
				std::cout << "Exiting . Unexpected token : " << token << "\n";
				return false;
			}
		}
		if(node_stack.size() == 1)
		{
			// remaining node is our root node
			outRootNode = node_stack.top(); node_stack.pop();
			return true;
		}
		else
		{
			std::cout << "Stack had more than 1 element remaining. Incorrect input. \n";
			return false;
		}
	}


	/*
	**
	*/
	void AddListOfNodes(std::vector<NodePtr<int>>& inNodes, NodePtr<int>& finalNode)
	{
		if(inNodes.size() == 1)
		{
			finalNode = inNodes[0];
			return;
		}
		auto nodeIter = inNodes.begin();
		finalNode = *nodeIter;
		// point to second node
		nodeIter++;
		for(; nodeIter != inNodes.end(); nodeIter++)
		{
			finalNode = Add(finalNode, *nodeIter);
		}		
	}

	int MaxSum(std::list<std::string>& inInputLines)
	{
		int maxMag = 0;
		for(auto curr_it = inInputLines.begin(); curr_it != inInputLines.end(); curr_it++)
		{
			for(auto other_it = inInputLines.begin(); other_it != inInputLines.end(); other_it++)
			{
				if(*curr_it == *other_it)
				{
					continue; // node cant be added to itself
				}
			
				NodePtr<int> curr_node;
				ProcessLine(*curr_it, curr_node);
				NodePtr<int> other_node;
				ProcessLine(*other_it, other_node);
				auto sumNode = Add(curr_node, other_node);
				auto magnitude = GetMagnitude(sumNode);
				if(magnitude > maxMag)
				{
					maxMag = magnitude;
				}
			}
		}
		return maxMag;
	}

} // P18
