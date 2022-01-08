/*
**
*/
#include "common/FileUtils.h"

#include "P22.h"
#include "P22_Operations.h"
#include "SpaceOperations.h"
#include "TestSpaceOperations.h"

namespace P22
{

/*
**
*/
void ProcessInner()
{
	// File operation
	std::string filePath("/Users/pgore/dev/AOC21/P22/input/input0.txt");
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
	std::list<RebootStep> rebootSteps;
	ProcessLines(lines, rebootSteps);
	
	std::cout << "Steps :\n";
	for(auto step: rebootSteps)
	{
		std::cout << step << "\n";
	}
} // ProcessInner

/**/
void Process()
{
	TestPlaneOverlap();
	
	// ProcessInner();
}

} // P22
