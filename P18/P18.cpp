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
	
	
	std::list<std::string> lines;
	fileReader->GetLines(lines);
	if(!lines.size())
	{
		std::cout << "Exiting puzzle. Empty file encountered : " << filePath << "\n";
		return;
	}
	
	// Check line processing
	/*
	auto line = lines.front();
	NodePtr<int> rootNode;
	bool construction_success = ProcessLine(line, rootNode);
	if(construction_success)
	{
		std::cout << rootNode << std::endl;
	}
	*/
	AddTest2();
}

} // namespace P18
