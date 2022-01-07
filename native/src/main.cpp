#include <iostream>

#include "boost/program_options.hpp"
#include "P18.h"
#include "P20.h"
#include "P21.h"
#include "P22.h"

namespace
{
	enum class PuzzleNumber{kP_18 = 18, kP_19, kP_20, kP_21, kP_22};

	void invokePuzzle(PuzzleNumber inNumber)
	{
		switch (inNumber) {
			case PuzzleNumber::kP_18:
				P18::Process();
				break;
			
			case PuzzleNumber::kP_20:
				P20::Process();
				break;
			
			case PuzzleNumber::kP_21:
				P21::Process();
				break;
			
			case PuzzleNumber::kP_22:
				P22::Process();
				break;
			
			default:
				break;
		}
	}
}

int main(int argc, const char *argv[])
{
	boost::program_options::options_description desc{"Options"};
	desc.add_options()
		("help,h", "Help screen")
		("puzzle-number,p", boost::program_options::value<int>(),"Puzzle Number (e.g. 18 )");

	boost::program_options::variables_map vm;
	boost::program_options::store(parse_command_line(argc, argv, desc), vm);
	notify(vm);
	
	if (vm.count("help"))
	{
		  std::cout << desc << '\n';
	}
	if(vm.count("puzzle-number"))
	{
		auto input_puzzle_number = vm["puzzle-number"].as<int>();
		invokePuzzle(PuzzleNumber(input_puzzle_number));
	}
    return 0;
}
