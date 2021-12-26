
/*
**
*/
#include <fstream>
#include <list>
namespace FileUtils
{
class FileReader;
using FileReaderUPtr = std::unique_ptr<FileReader>;
FileReaderUPtr CreateFileReader(const std::string& inFilePath);



class FileReader
{
public:
	/*
	**
	*/
	FileReader(std::ifstream&& inFileStream)
	:mFileStream(std::forward<std::ifstream>(inFileStream))
	{}
	
	FileReader(FileReader&& inOther)
	:mFileStream(std::move(inOther.mFileStream))
	{}
		
	FileReader& operator=(FileReader&& inOther)
	{
		std::swap<std::ifstream>(mFileStream, inOther.mFileStream);
		return *this;
	}
	
	/*
	**
	*/
	void GetNextLine(std::string& outLine);
	
	void GetLines(std::list<std::string>& outLines);
private:
	std::ifstream mFileStream;
};


} // namespace FileUtils
