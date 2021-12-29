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
	
		std::string::const_iterator it;
		for(it = inLine.begin(); it != inLine.end(); it++)
		{
			char token = *it;
			if(token == '[')
			{
				paran_stack.push(token);
			}
			else if(isdigit(token))
			{
				auto value = std::atoi(&token);
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
			std::cout << "----------------------------------------------\n";
		}		
	}

} // P18
