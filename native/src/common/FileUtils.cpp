#include <fstream>
#include <filesystem>
#include <memory>
#include "common/FileUtils.h"

namespace FileUtils
{

/*
**
*/
FileReaderUPtr CreateFileReader(const std::string& inFilePath)
{
	std::filesystem::path filePath(inFilePath);
	auto path_exists = std::filesystem::exists(inFilePath);
	if(path_exists)
	{
		std::ifstream fileStream(inFilePath.c_str());
		return std::make_unique<FileReader>(std::move(fileStream));
	}
	return std::unique_ptr<FileReader>();
}

/*
**
*/
void FileReader::GetNextLine(std::string& outLine)
{
	std::getline(mFileStream, outLine);
}


/*
**
*/
void FileReader::GetLines(std::list<std::string>& outLines)
{
	std::string line;
	while(std::getline(mFileStream, line))
	{
		outLines.push_back(std::move(line));
	}
}

} // namespace FileUtils
