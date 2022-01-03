/*
**
*/
#pragma once

#include "common/DataBuffer2D.h"
#include <utility>
#include <iostream>

#define PRINT_FORMATTED_BUFFERS 1
/*
**
*/
template <typename T>
bool GetLocalElements(
	const DataBuffer2DPtr<T>& inBuffer,
	std::uint32_t inX,
	std::uint32_t inY,
	std::array<T, 9>& outArray)
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
std::uint32_t GetIndexFromKey(const std::array<std::uint32_t, 9>& inKey)
{
	std::uint32_t index = 0;
	for(int pos = 8; pos >=0; pos--)
	{
		index += inKey[pos] * ( 1 << (8 - pos));
	}
	return index;
}

/**/
std::uint32_t GetLegendValue(
	const std::array<std::uint32_t, 512>& legend,
	const std::array<std::uint32_t, 9>& inKey)
{
	std::uint32_t index = GetIndexFromKey(inKey);
	return legend[index];
}

/**/
std::uint32_t GetConsultedValue(
	const DataBuffer2DPtr<std::uint32_t>& inBuffer,
	const std::array<std::uint32_t, 512> inLegend,
	std::uint32_t inX,
	std::uint32_t inY)
{
	//TODO: reallocation of this stack memory for each element is expensive.
	std::array<std::uint32_t, 9> locals;
	
	// Edge elements will always have 0 value, since
	// they are outside the immediate consultation zone.
	if(isEdgeElement(inBuffer, inX, inY))
	{
		return 0;
	}
	
	GetLocalElements(inBuffer, inX, inY, locals);
	return GetLegendValue(inLegend, locals);
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
			std::uint32_t toY = (rowIndex + toOffsetY);
			std::uint32_t toX = (colIndex + toOffsetX);
			toBuffer->SetElement(toX, toY, fromBuffer->GetElement(fromX, fromY));
		}
	}
}

/**/
void PrintFormattedBuffer(const DataBuffer2DPtr<std::uint32_t>& inBuffer)
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
	auto oneCounter = [&oneCount](
							std::uint32_t inX,
							std::uint32_t inY,
							const std::uint32_t& inValue) -> void
		{
			if(inValue == 1)
			{
				oneCount++;
			}
		};
	ProcessBufferElements(inBuffer, oneCounter);
	return oneCount;
}


/**/
void IterateOverBuffer(
	const DataBuffer2DPtr<std::uint32_t>& inBuffer,
	const std::array<std::uint32_t, 512>& inLegend,
	std::uint32_t iterCount,
	std::uint32_t inPadding)
{
	#if PRINT_FORMATTED_BUFFERS
		std::cout << "---\n Before Iteration Processed Vector:\n";
		PrintFormattedBuffer(inBuffer);
	#endif
	
	std::vector<std::uint32_t>& vectorHandle = inBuffer->GetDataHandle();
	auto bufferWidth = inBuffer->mWidth;
	
	// create swap vector the same size as the one held by buffer and zeroFill
	std::vector<uint32_t> swapVector(vectorHandle.size(), 0);
	
	// process iteratively
	for(std::uint32_t iter = 0; iter < iterCount; iter++)
	{
		auto processLocals
			= [&inBuffer, &inLegend, &swapVector, bufferWidth](
				std::uint32_t inX,
				std::uint32_t inY,
				const uint32_t inElement)->void
			{
				auto val = GetConsultedValue(inBuffer, inLegend, inX, inY);
				swapVector[inY * bufferWidth + inX] = val;
				
			};
		ProcessBufferElements(inBuffer, processLocals);
		std::swap(vectorHandle, swapVector);
		
		#if PRINT_FORMATTED_BUFFERS
			std::cout << "---\n After Iteration #" << iter << " , Processed Vector:\n";
			PrintFormattedBuffer(inBuffer);
		#endif
	}
}
