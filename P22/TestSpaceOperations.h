/**/
#pragma once

#include "SpaceOperations.h"
#include <iostream>

namespace P22
{

/**/
std::ostream& operator<<(std::ostream& os, const IntersectPoint& inIntersectPt)
{
	os << "(" << inIntersectPt.mMin << ", " << inIntersectPt.mMax << ")"; return os;
}

/**/
void TestPlaneOverlap()
{
	std::cout << DimOverlap(10, 15, 17, 18) << std::endl; // zero intersert
	std::cout << DimOverlap(17, 18, 10, 15) << std::endl; // zero interserct
	
	std::cout << DimOverlap(10, 19, 16, 18) << std::endl; // 16,18 // one contains two
	std::cout << DimOverlap(10, 19, 0, 100) << std::endl; // 10,19 // two contains one
	
	std::cout << DimOverlap(10, 19, 10, 19) << std::endl; // 10,19 // equal
	
	std::cout << DimOverlap(10, 19, 12, 20) << std::endl; // 12,19
	std::cout << DimOverlap(10, 19, 10, 20) << std::endl; // 10,19
	std::cout << DimOverlap(15, 20, 10, 20) << std::endl; // 15,20
	
	std::cout << DimOverlap(20, 25, 22, 30) << std::endl; // 22,25
	std::cout << DimOverlap(22, 25, 22, 30) << std::endl; // 22,25
	
	std::cout << DimOverlap(-12, 19, -10, 20) << std::endl; // -10, 19
	std::cout << DimOverlap(-19, -12, -13, 20) << std::endl; // -13, -12
	std::cout << DimOverlap(-19, -12, -19, 20) << std::endl; // -19, -12	
}


}	// P22
