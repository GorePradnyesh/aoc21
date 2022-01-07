/*
**
*/
#include "P22.h"
#include "common/FileUtils.h"

#include <iostream>
#include <regex>

namespace P22
{
enum class RebootState {k_on, k_off};

/**/
std::ostream& operator<<(std::ostream& os, RebootState rs)
{
	if(rs == RebootState::k_on)
	{
		os << "on";
	}
	else
	{
		os << "off";
	}
	return os;
}

/**/
struct RebootStep
{
	RebootState mState;
	
	std::int32_t mXLow { 0 };
	std::int32_t mXHigh { 0 };
	std::int32_t mYLow { 0 };
	std::int32_t mYHigh { 0 };
	std::int32_t mZLow { 0 };
	std::int32_t mZHigh { 0 };
};

namespace
{


/**/
void ProcessLines(
	const std::list<std::string>& inLines,
	std::list<RebootStep>& outSteps)
{
	const std::regex rgx("(on|off) x=(\\d+)..(\\d+),y=(\\d+)..(\\d+),z=(\\d+)..(\\d+)");
	// const std::string s1("on x=10..12,y=10..12,z=10..12");
	std::smatch match;
	
	for(const std::string& line : inLines)
	{
		if(std::regex_search(line.begin(), line.end(), match, rgx))
		{
			RebootState state = !std::string("on").compare(match[1]) ? RebootState::k_on : RebootState::k_off;
			RebootStep step {
				state,
				std::stoi(match[2]),
				std::stoi(match[3]),
				std::stoi(match[4]),
				std::stoi(match[5]),
				std::stoi(match[6]),
				std::stoi(match[7])};
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


} // unnamed


/*
**
*/
void Process()
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
}

} // namespace P22
