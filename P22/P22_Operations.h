/*
**
*/
#pragma once

#include <iostream>
#include <regex>
#include <map>

#include "SpaceOperations.h"

namespace P22
{

enum class RebootState {k_on, k_off};


/**/
std::ostream& operator<<(std::ostream& os, RebootState rs)
{
	if(rs == RebootState::k_on)
	{ os << "on"; }
	else
	{ os << "off"; }
	return os;
}

/**/
class RebootStep
{
public:
	RebootStep(
		RebootState inState,
		std::int32_t inXLow,
		std::int32_t inXHigh,
		std::int32_t inYLow,
		std::int32_t inYHigh,
		std::int32_t inZLow,
		std::int32_t inZHigh)
		:
		mState(inState),
		mXLow(inXLow),
		mXHigh(inXHigh),
		mYLow(inYLow),
		mYHigh(inYHigh),
		mZLow(inZLow),
		mZHigh(inZHigh)
	{}

	RebootState mState;
	
	std::int32_t mXLow { 0 };
	std::int32_t mXHigh { 0 };
	std::int32_t mYLow { 0 };
	std::int32_t mYHigh { 0 };
	std::int32_t mZLow { 0 };
	std::int32_t mZHigh { 0 };
};


/**/
Cuboid CreateCuboid(const RebootStep& inStep)
{
	return CreateCuboid(inStep.mXLow, inStep.mXHigh, inStep.mYLow, inStep.mYHigh, inStep.mZLow, inStep.mZHigh);
}

/**/
void ProcessLines(
	const std::list<std::string>& inLines,
	std::list<RebootStep>& outSteps)
{
	const std::regex rgx("(on|off) x=(-?\\d+)..(-?\\d+),y=(-?\\d+)..(-?\\d+),z=(-?\\d+)..(-?\\d+)");
	std::smatch match;
	
	for(const std::string& line : inLines)
	{
		if(std::regex_search(line.begin(), line.end(), match, rgx))
		{
			RebootState state = !std::string("on").compare(match[1]) ? RebootState::k_on : RebootState::k_off;
			RebootStep step(
				state,
				std::stoi(match[2]),
				std::stoi(match[3]),
				std::stoi(match[4]),
				std::stoi(match[5]),
				std::stoi(match[6]),
				std::stoi(match[7]));
			outSteps.push_back(step);
		}
	}
}

/**/
std::ostream& operator<<(std::ostream& os, RebootStep step)
{
	os << step.mState
		<< " x=" << step.mXLow << ".." << step.mXHigh
		<< ",y=" << step.mYLow << ".." << step.mYHigh
		<< ",z=" << step.mZLow << ".." <<  step.mZHigh;
	return os;
}
} // P22
