/*
**
*/

#pragma once

namespace P22
{
/**/
struct Coord
{
	std::int32_t x;
	std::int32_t y;
	std::int32_t z;
};

constexpr Coord kZero {0, 0, 0};

/**/
struct Cuboid
{
	Coord minCorner;
	std::uint32_t mXDim; 	// x
	std::uint32_t mYDim; 	// y
	std::uint32_t mZDim;	// z
}; //

constexpr Cuboid kZeroCuboid { kZero, 0, 0, 0 };

/**/
struct IntersectPoint
{
	std::int32_t mMin;
	std::int32_t mMax;
};
constexpr IntersectPoint kZeroInterserct {0, 0};


/**/
inline bool ValueContainedIn(
	const std::int32_t inValue,
	const std::int32_t inMin,
	const std::int32_t inMax)
{
	return (inValue <= inMin || inValue <= inMax);
}


/**/
inline IntersectPoint PlanesOverlap(
	std::int32_t inMinOne,
	std::int32_t inMaxOne,
	std::int32_t inMinTwo,
	std::int32_t inMaxTwo,
	IntersectPoint& outIntersect)
{
	// one is contained within two
	if(inMinOne >= inMinTwo && inMaxOne <= inMaxTwo)
	{
		return IntersectPoint {inMinOne, inMaxOne};
	}
	
	// two is contained within one
	if(inMinOne <= inMinTwo && inMaxOne >= inMaxTwo)
	{
		return IntersectPoint {inMinTwo, inMaxTwo};
	}
	
	// min One is between the planes of two
	if(inMinOne >= inMinTwo )
	{
		return IntersectPoint {inMinOne, inMaxTwo};
	}
	
	// max One is between the planes of two
	if(inMinOne <= inMinTwo)
	{
		return IntersectPoint {inMinTwo, inMaxOne};
	}
	
	return kZeroInterserct;
}

/**/
void GetIntersection(
	const Cuboid& inInput1,
	const Cuboid& inInput2,
	Cuboid& outIntersects)
{
	outIntersects = kZeroCuboid;
	
}

} // P22
