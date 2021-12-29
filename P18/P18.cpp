#include "common/BinaryTree.h"
#include "common/BinaryTreeOps.h"
#include <iostream>

#include "common/FileUtils.h"

#include "P18Operations.h"
#include "P18SimpleTests.h"

namespace
{
	
	
} // unnamed


namespace P18
{

/*
**
*/
void Process()
{
	std::string filePath("/Users/pgore/dev/AOC21/P18/input/input0.txt");
	FileUtils::FileReaderUPtr fileReader(FileUtils::CreateFileReader(filePath)); // copy elision
	
	if(!fileReader)
	{
		std::cout << "Exiting. Invalid file:" << filePath << "\n";
		return;
	}
	
	std::list<std::string> lines;
	fileReader->GetLines(lines);
	if(!lines.size())
	{
		std::cout << "Exiting puzzle. Empty file encountered : " << filePath << "\n";
		return;
	}
	
	std::vector<NodePtr<int>> inputNodes;
	inputNodes.reserve(lines.size());
	
	for(auto inputLine: lines)
	{
		NodePtr<int> inputNode;
		bool construction_success = ProcessLine(inputLine, inputNode);
		if(construction_success)
		{
			inputNodes.push_back(inputNode);
		}
		else
		{
			std::cout << "Exiting. Construction Error for line: " << inputLine << "\n";
		}
	}
	
	NodePtr<int> finalSum;
	AddListOfNodes(inputNodes, finalSum);
	std::cout << "Final Sum node:\t"; PrintNode(finalSum);
	std::cout << "Final Magnitude :" << GetMagnitude(finalSum) << "\n";
	// AddTest2();
}

} // namespace P18
