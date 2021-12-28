#include "common/BinaryTree.h"
#include "common/BinaryTreeOps.h"
#include <iostream>

#include "common/FileUtils.h"


namespace
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
					auto nonLeafNode = Node<int>::CreateNode(op1, op2, 0);
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
	
} // unnamed


namespace P18
{

// TODO: use google test really
void SimpleNavigationTest()
{
	// Example Usage
	
	NodePtr<int> node_left = std::make_shared<Node<int>>(45);
	NodePtr<int> node_right = std::make_shared<Node<int>>(46);
	NodePtr<int> node_root = Node<int>::CreateNode(node_left, node_right, 0);
	
	PrintNode(node_root);
	auto rightSibling = GetLeftmostOfRightChild(node_root, node_left);
	PrintNode(rightSibling);
	
	std::cout << "------------ \n";
	
	/*
	// Tree construction
	// new line is new level
	// nodes without children are leaf nodes with valid data values
							A
				B						C
			D		E				F		G
		H	I		J	K		L	M
	N	O				P Q
	*/
	// leaf nodes
	auto node_n = Node<int>::CreateNode(10);
	auto node_o = Node<int>::CreateNode(11);
	auto node_i = Node<int>::CreateNode(12);
	auto node_j = Node<int>::CreateNode(13);
	auto node_p = Node<int>::CreateNode(14);
	auto node_q = Node<int>::CreateNode(15);
	auto node_l = Node<int>::CreateNode(16);
	auto node_m = Node<int>::CreateNode(17);
	auto node_g = Node<int>::CreateNode(18);
	
	// non-leaf
	auto node_h = Node<int>::CreateNode(node_n, node_o, 0);
	auto node_d = Node<int>::CreateNode(node_h, node_i, 0);
	auto node_k = Node<int>::CreateNode(node_p, node_q, 0);
	auto node_e = Node<int>::CreateNode(node_j, node_k, 0);
	auto node_b = Node<int>::CreateNode(node_d, node_e, 0);
	
	auto node_f = Node<int>::CreateNode(node_l, node_m, 0);
	auto node_c = Node<int>::CreateNode(node_f, node_g, 0);
	
	auto node_a = Node<int>::CreateNode(node_b, node_c, 0);
	
	PrintNode(node_a);
	
	// get right siblings
	PrintNode(GetRightSibling(node_n)); // should be node_o, 11
	PrintNode(GetRightSibling(node_o)); // should be node_i, 12
	PrintNode(GetRightSibling(node_i)); // should be node_j, 13
	PrintNode(GetRightSibling(node_j)); // should be node_p, 14
	PrintNode(GetRightSibling(node_p)); // should be node_q, 15
	PrintNode(GetRightSibling(node_q)); // should be node_l, 16
	PrintNode(GetRightSibling(node_l)); // should be node_m, 17
	PrintNode(GetRightSibling(node_m)); // should be node_g, 18
	auto retNode = GetRightSibling(node_g); // retNode should be empty
	;
	
	// get left siblings
	
	
	
}

/**/
void Process()
{

	std::string filePath("/Users/pgore/dev/AOC21/P18/input/input0.txt");
	FileUtils::FileReaderUPtr fileReader(FileUtils::CreateFileReader(filePath)); // copy elision
	
	
	std::list<std::string> lines;
	fileReader->GetLines(lines);
	if(!lines.size())
	{
		std::cout << "Exiting puzzle. Empty file encountered : " << filePath << "\n";
		return;
	}
	
	// Check line processing
	auto line = lines.front();
	NodePtr<int> rootNode;
	bool construction_success = ProcessLine(line, rootNode);
	if(construction_success)
	{
		std::cout << rootNode << std::endl;
	}
	
	SimpleNavigationTest();
	
	
	//
	
}

} // namespace P18
