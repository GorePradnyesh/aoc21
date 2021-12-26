#include "common/BinaryTree.h"
#include <iostream>

#include "common/FileUtils.h"

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
}

} // namespace P18
