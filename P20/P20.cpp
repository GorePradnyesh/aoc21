#include "P20.h"
#include <utility>
#include <array>
#include "common/DataBuffer2D.h"
#include "common/FileUtils.h"
#include "P20_DataBufferOps.h"

namespace
{

void BasicBufferTest()
{
	std::vector<std::uint32_t> buffer {0, 1, 2, 3, 4, 5, 6, 7, 8};
	auto bufferPtr = CreateBuffer(std::move(buffer), 3, 3);

	std::cout << *bufferPtr << std::endl;
	std::array<std::uint32_t, 9> arr;
	GetLocalElements(bufferPtr, 1, 1, arr);
	
	auto targetBufferPtr = CreateInitBuffer<std::uint32_t>(9, 9, 0);
	std::cout << "---- Target Buffer ----\n";
	std::cout << *targetBufferPtr << std::endl;
	
	CopyToBuffer(bufferPtr, targetBufferPtr, 3, 3);
	std::cout << "---- Target Buffer After Copy ----\n";
	std::cout << *targetBufferPtr << std::endl;
}

void OstreamTest(std::ostream& stream, std::uint32_t &inValue, std::uint8_t inWidth)
{
	stream << std::setw(inWidth) << inValue << std::endl;
}

/*
**
*/
std::array<std::uint8_t,512> ProcessLegend(const std::string& inString)
{
	std::array<std::uint8_t, 512> legend;
	if(inString.size() != 512)
	{
		std::cout << "invalid legend size !! ";
	}
	else
	{
		for(std::uint32_t index = 0; index < inString.size(); index++)
		{
			char legend_char = inString[index];
			if(legend_char == '.')
			{
				legend[index] = 0;
			}
			else if (legend_char == '#')
			{
				legend[index] = 1;
			}
			else
			{
				std::cout << "Invalid char encoundtered : " << legend_char << "\n";
			}
		}
	}
	return legend;
}

/*
**
*/
DataBuffer2DPtr<std::uint32_t> ProcessLines(const std::list<std::string>& inLines, std::uint8_t inPadding = 0)
{
	//Get Buffer
	size_t lineSize = inLines.front().size();
	size_t lineCount = inLines.size();
	DataBuffer2DPtr<std::uint32_t> innerBuffer = CreateInitBuffer<std::uint32_t>(lineSize, lineCount, 0);
	std::uint32_t yIndex = 0;
	for(auto dataLine: inLines)
	{
		for(std::uint32_t xIndex =0; xIndex < dataLine.size(); xIndex++)
		{
			char lineChar = dataLine[xIndex];
			if(lineChar == '#')
			{
				innerBuffer->SetElement(xIndex, yIndex, 1);
			}
			else if (lineChar != '.')
			{
				std::cout << "Invalid char found while processing data line: " << lineChar << "\n";
			}
		}
		yIndex++;
	}
	
	// Extend buffer to include additional padding
	std::uint8_t padding = 0;
	if(inPadding)
	{
		padding = inPadding;
	}
	else
	{
		padding = 4;
	}
	DataBuffer2DPtr<std::uint32_t> outerBuffer
		= CreateInitBuffer<std::uint32_t>(lineSize + padding * 2, lineCount + padding * 2, 0);
	// to copy inner to center of outer buffer, the offsets are the same as padding
	CopyToBuffer(innerBuffer, outerBuffer, padding, padding);
	return outerBuffer;
}


}	// unnamed

namespace P20
{

/*
**
*/
void Process()
{

	// File operation
	std::string filePath("/Users/pgore/dev/AOC21/P20/input/input0.txt");
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
	
	//Process input
	auto legend_str = lines.front(); lines.pop_front();
	auto legend = ProcessLegend(legend_str);
	
	auto empty_line = lines.front(); lines.pop_front();
	if(!empty_line.empty())
	{
		std::cout <<"Exiting. Invalid input.\n";
		return;
	}
	auto buffer = ProcessLines(lines);
	
	// print formatted buffer
	auto printProc = [](std::ostream& stream, const std::uint32_t& inValue) -> void
		{
			char c = '.';
			if(inValue == 1)
			{ c = '#'; }
			stream << std::setw(1) << c;
		};
	PrintProcessedElements(buffer, printProc);

	// Count number of ones
	std::uint32_t oneCount = 0;
	auto oneCounter = [&oneCount](const std::uint32_t& inValue) -> void
		{
			if(inValue == 1)
			{
				oneCount++;
			}
		};
	ProcessBufferElements(buffer, oneCounter);
	std::cout << "One Count: "  << oneCount << "\n";
	
}

} // namespace P18
