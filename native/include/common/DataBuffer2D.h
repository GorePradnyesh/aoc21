/*
**
*/
#pragma once

#include <vector>
#include <algorithm>
#include <memory>
#include <utility>
#include <iostream>

template <typename T>
class DataBuffer2D
{
public:
	/**/
	DataBuffer2D(DataBuffer2D&& inOtherBuffer)
	:
	mDataBuffer(std::move(inOtherBuffer.mDataBuffer)),
	mWidth(inOtherBuffer.mWidth),
	mHeight(inOtherBuffer.mHeight)
	{}
	
	DataBuffer2D() = default;

	/**/
	T GetElement(std::uint32_t inX, std::uint32_t inY)
	{
		return mDataBuffer[inY * mWidth + inX];
	}
	
	/**/
	void SetElement(std::uint32_t inX, std::uint32_t inY, const T&& inValue)
	{
		mDataBuffer[inY * mWidth + inX] = std::forward<const T>(inValue);
	}
	
	std::vector<T>& GetDataHandle()
	{
		return mDataBuffer;
	}
	
	const std::uint32_t mWidth;
	const std::uint32_t mHeight;
	
private:
	/**/
	DataBuffer2D(
		std::vector<T>&& inDataBuffer,
		const std::uint32_t inWidth,
		const std::uint32_t inHeight)
	:
	mWidth(inWidth),
	mHeight(inHeight),
	mDataBuffer(std::forward<std::vector<T>>(inDataBuffer))
	{}
	
private:
	
	
	std::vector<T> mDataBuffer;
	
	template <typename U>
	friend std::shared_ptr<DataBuffer2D<U>> CreateBuffer(
		std::vector<U>&& inData,
		std::uint32_t inWidth,
		std::uint32_t inHeight);

	
	template <typename U>
	friend std::ostream& operator<<(std::ostream& os, const DataBuffer2D<U>& inBuffer);
	
	template <typename U, typename ProcOp>
	friend void ProcessBufferElements(std::shared_ptr<DataBuffer2D<U>>& inBuffer, const ProcOp& unaryOp);
};

template <typename T>
using DataBuffer2DPtr = std::shared_ptr<DataBuffer2D<T>>;

/*
**
*/
template <typename T>
DataBuffer2DPtr<T> CreateBuffer(
	std::vector<T>&& inData,
	std::uint32_t inWidth,
	std::uint32_t inHeight)
{
	DataBuffer2DPtr<T> dataBuffer;
	if(inWidth * inHeight > inData.size())
	{
		std::cout << "Invalid input. Buffer size does not match height width\n";
		return dataBuffer;
	}
	return DataBuffer2DPtr<T>(
		new DataBuffer2D<T>(std::forward<std::vector<T>>(inData), inWidth, inHeight)
		);
}

/**/
template <typename T>
DataBuffer2DPtr<T> CreateInitBuffer(
	std::uint32_t inWidth,
	std::uint32_t inHeight,
	const T& initValue)
{
	std::vector<T> initialBuffer(inWidth * inHeight, initValue);
	return CreateBuffer(std::move(initialBuffer), inWidth, inHeight);
}



/**/
template <typename T, typename ProcOp>
void ProcessBufferElements(DataBuffer2DPtr<T>& inBuffer, const ProcOp& unaryOp)
{
	for(std::uint32_t y = 0; y < inBuffer->mHeight; y++)
	{
		for(std::uint32_t x = 0; x < inBuffer->mWidth; x++)
		{
			unaryOp(inBuffer->GetDataHandle()[y * inBuffer->mWidth + x]);
		}
	}
}

/**/
template <typename T, typename ElementOp>
void PrintProcessedElements(
	DataBuffer2DPtr<T>& inBuffer,
	const ElementOp& elementProcOp)
{
	for(std::uint32_t y = 0; y < inBuffer->mHeight; y++)
	{
		for(std::uint32_t x = 0; x < inBuffer->mWidth; x++)
		{
			elementProcOp(std::cout, inBuffer->GetDataHandle()[y * inBuffer->mWidth + x]);
		}
		std::cout << "\n";
	}
}

/*
**
*/
template <typename T>
std::ostream& operator<<(std::ostream& os, const DataBuffer2D<T>& inBuffer)
{
	for(std::uint32_t y = 0; y < inBuffer.mHeight; y++)
	{
		for(std::uint32_t x = 0; x < inBuffer.mWidth; x++)
		{
			os << inBuffer.mDataBuffer[y * inBuffer.mWidth + x];
			os << " ";
		}
		os << "\n";
	}
	return os;
}

/*
**
*/
template <typename T>
bool isEdgeElement(const DataBuffer2DPtr<T>& inBuffer, std::uint32_t inX, std::uint32_t inY)
{
	return (
		inX == 0 || inX == (inBuffer->mWidth - 1) ||
		inY == 0 || (inY == inBuffer->mHeight - 1)) ;
}

