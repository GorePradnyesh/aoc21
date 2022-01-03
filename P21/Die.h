/*
**
**/
#pragma once
#include <cstdint>
#include <memory>

/**/
class Die
{
public:
	Die() = default;
	virtual ~Die() = default;

	std::uint32_t GenerateNext()
	{
		mRollCount++;
		return GenerateNextInner();
	}

	std::uint32_t GetRollCount()
	{
		return mRollCount;
	}
	
protected:
	virtual std::uint32_t GenerateNextInner() = 0;
	std::uint32_t mRollCount { 0 };	// TODO: Atomic is overkill ?
};

using DiePtr = std::shared_ptr<Die>;

/**/
class DeterministicDie: public Die
{
public:
	
	DeterministicDie() = default;

protected:
	
	virtual std::uint32_t GenerateNextInner()
	{
		std::uint32_t retVal = mCurrentGenCount;
		mCurrentGenCount = (mCurrentGenCount + 1 ) % maxGen;
		return retVal;
	}
	
private:
	std::uint32_t mCurrentGenCount { 1 };
	const std::uint32_t maxGen { 100 };
};
