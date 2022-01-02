/*
**
*/
#pragma once

#include "common/DataBuffer2D.h"
#include <utility>
#include <iostream>
/*
**
*/
template <typename T>
bool GetLocalElements(const DataBuffer2DPtr<T>& inBuffer, std::uint32_t inX, std::uint32_t inY, std::array<T, 9>& outArray)
{
	if(isEdgeElement(inBuffer, inX, inY))
	{
		return false;
	}
	outArray[0] = inBuffer->GetElement(inX - 1, inY - 1);
	outArray[1] = inBuffer->GetElement(inX, 	inY - 1);
	outArray[2] = inBuffer->GetElement(inX + 1, inY - 1);
	outArray[3] = inBuffer->GetElement(inX - 1, inY);
	outArray[4] = inBuffer->GetElement(inX, 	inY);
	outArray[5] = inBuffer->GetElement(inX + 1, inY);
	outArray[6] = inBuffer->GetElement(inX - 1, inY + 1);
	outArray[7] = inBuffer->GetElement(inX, 	inY + 1);
	outArray[8] = inBuffer->GetElement(inX + 1, inY + 1);

	return true;
}


/*
**
*/
template <typename T>
void CopyToBuffer(
	DataBuffer2DPtr<T>& fromBuffer,
	DataBuffer2DPtr<T>& toBuffer,
	std::uint32_t toOffsetX,
	std::uint32_t toOffsetY)
{
	//TODO: bounds checkig for "from" and "to" and offsetX, offsetY
	//TODO: use memcpy for the range copies. More efficient
	for(std::uint32_t rowIndex = 0; rowIndex < fromBuffer->mHeight; rowIndex++)
	{
		for(std::uint32_t colIndex = 0; colIndex < fromBuffer->mWidth; colIndex++)
		{
			std::uint32_t fromY = rowIndex;
			std::uint32_t fromX = colIndex;
			std::uint32_t toX = (rowIndex + toOffsetY);
			std::uint32_t toY = (colIndex + toOffsetX);
			toBuffer->SetElement(toX, toY, fromBuffer->GetElement(fromX, fromY));
		}
	}
}

/**/
void PrintFormattedBuffer(DataBuffer2DPtr<std::uint32_t>& inBuffer)
{
	// print formatted buffer
	auto printProc = [](std::ostream& stream, const std::uint32_t& inValue) -> void
		{
			char c = '.';
			if(inValue == 1)
			{ c = '#'; }
			stream << std::setw(1) << c;
		};
	PrintProcessedElements(inBuffer, printProc);
}

/**/
std::uint32_t CountOnes(DataBuffer2DPtr<std::uint32_t>& inBuffer)
{
	std::uint32_t oneCount = 0;
	auto oneCounter = [&oneCount](const std::uint32_t& inValue) -> void
		{
			if(inValue == 1)
			{
				oneCount++;
			}
		};
	ProcessBufferElements(inBuffer, oneCounter);
	return oneCount;
}
