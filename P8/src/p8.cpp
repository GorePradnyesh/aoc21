#include <iostream>
#include <fstream>
#include <filesystem>

#include "include/P8.h"

/*
******************************
******************************
*/

void tokenize(const std::string& lineStr, std::vector<std::string>& outVector)
{
	
}

void extract(const std::fstream& fileStream)
{
	std::string lineStr;
	
}

int processP8()
{
	const char* filePath  = "/Users/pgore/dev/AOC21/P8/input/input0.txt";
	std::fstream fpHandle(filePath);
	if(!fpHandle)
	{
		std::cout << "Invalid file path\n";
		return -1;
	}
	extract(fpHandle);
	return 0;
}
