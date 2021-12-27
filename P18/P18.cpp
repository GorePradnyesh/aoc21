#include "common/BinaryTree.h"
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
					auto nonLeafNode = std::make_shared<Node<int>>(op1, op2, 0);
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
		// std::cout << *rootNode << std::endl;
		PrintNode(rootNode);
	}
		
	// Example Usage
	/*
	NodePtr<int> node_l = std::make_shared<Node<int>>(45);
	NodePtr<int> node_r = std::make_shared<Node<int>>(46);
	NodePtr<int> node_root = std::make_shared<Node<int>>(node_l, node_r, 0);
	
	PrintNode(node_root);
	*/
	
}

} // namespace P18
