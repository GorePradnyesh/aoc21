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
	return os;
}

std::ostream& operator<<(std::ostream& os, const Coord& inCoord)
{
	os << "(" << inCoord.x << ", " << inCoord.y << ", " << inCoord.z << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Cuboid& inCuboid)
{
	os << inCuboid.minCorner << "[" << inCuboid.mXDim <<", " << inCuboid.mYDim << ", " << inCuboid.mZDim << "]";
	return os;
}

/**/
void TestPlaneOverlap()
{
	IntersectPoint intersectPt;
	bool intersects;
	intersects = DimOverlap(10, 15, 17, 18, intersectPt); std::cout << std::boolalpha <<  intersects << ", " << intersectPt  << std::endl; // zero intersert
	intersects = DimOverlap(17, 18, 10, 15, intersectPt); std::cout << std::boolalpha <<  intersects << ", " << intersectPt  << std::endl; // zero interserct

	intersects = DimOverlap(10, 19, 16, 18, intersectPt); std::cout << std::boolalpha <<  intersects << ", " << intersectPt  << std::endl; // 16,18 // one contains two
	intersects = DimOverlap(10, 19, 0, 100, intersectPt); std::cout << std::boolalpha <<  intersects << ", " << intersectPt  << std::endl; // 10,19 // two contains one

	intersects = DimOverlap(10, 19, 10, 19, intersectPt); std::cout << std::boolalpha <<  intersects << ", " << intersectPt  << std::endl; // 10,19 // equal

	intersects = DimOverlap(10, 19, 12, 20, intersectPt); std::cout << std::boolalpha <<  intersects << ", " << intersectPt  << std::endl; // 12,19
	intersects = DimOverlap(10, 19, 10, 20, intersectPt); std::cout << std::boolalpha <<  intersects << ", " << intersectPt  << std::endl; // 10,19
	intersects = DimOverlap(15, 20, 10, 20, intersectPt); std::cout << std::boolalpha <<  intersects << ", " << intersectPt  << std::endl; // 15,20

	intersects = DimOverlap(20, 25, 22, 30, intersectPt); std::cout << std::boolalpha <<  intersects << ", " << intersectPt  << std::endl; // 22,25
	intersects = DimOverlap(22, 25, 22, 30, intersectPt); std::cout << std::boolalpha <<  intersects << ", " << intersectPt  << std::endl; // 22,25

	intersects = DimOverlap(-12, 19, -10, 20, intersectPt); std::cout << std::boolalpha <<  intersects << ", " <<  intersectPt << std::endl; // -10, 19
	intersects = DimOverlap(-19, -12, -13, 20, intersectPt); std::cout << std::boolalpha <<  intersects << ", " <<  intersectPt << std::endl; // -13, -12
	intersects = DimOverlap(-19, -12, -19, 20, intersectPt); std::cout << std::boolalpha <<  intersects << ", " <<  intersectPt << std::endl; // -19, -12
}


void TestCubeGeneration()
{
	auto pLambda = [](bool intersects, Cuboid& inCuboid)->void
		{
			if(intersects)
			{ std::cout << std::boolalpha << intersects << ", " << inCuboid << "\n"; }
			else
			{
				std::cout << std::boolalpha << intersects << "\n";
			}
		};
		
	Cuboid intersectingCuboid;
	bool intersects;
	
	intersects = GetIntersection(CreateCuboid(0, 9, 0, 9, 0, 9), CreateCuboid(5, 9, 5, 9, 5, 9), intersectingCuboid);
	pLambda(intersects, intersectingCuboid);
	intersects = GetIntersection(CreateCuboid(0, 9, 0, 9, 0, 9), CreateCuboid(5, 20, 5, 20, 5, 20), intersectingCuboid);
	pLambda(intersects, intersectingCuboid);
	intersects = GetIntersection(CreateCuboid(0, 10, 0, 10, 0, 10), CreateCuboid(15, 20, 15, 20, 15, 20), intersectingCuboid);
	pLambda(intersects, intersectingCuboid);
	intersects = GetIntersection(CreateCuboid(0, 10, 0, 10, 0, 10), CreateCuboid(10, 20, 10, 20, 10, 20), intersectingCuboid);
	pLambda(intersects, intersectingCuboid);
	
	intersects = GetIntersection(CreateCuboid(0, 9, 0, 9, 0, 9), CreateCuboid(9, 18, 0, 9, 0, 9), intersectingCuboid);
	pLambda(intersects, intersectingCuboid);



	
}

}	// P22
