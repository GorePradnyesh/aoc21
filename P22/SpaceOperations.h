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
struct IntersectPoint
{
	std::int32_t mMin;
	std::int32_t mMax;
	
	operator bool() const {return (!!mMin && !!mMax);}
};
constexpr IntersectPoint kZero_Interserct {0, 0};


/**/
struct Cuboid
{
	Coord minCorner;
	std::int32_t mXDim; 	// x
	std::int32_t mYDim; 	// y
	std::int32_t mZDim;	// z
	
	std::int32_t GetVolume();
}; //

/**/
inline std::int32_t Cuboid::GetVolume()
{
	return mXDim * mYDim * mZDim;
}



Cuboid CreateCuboid(
		std::int32_t inXLow,
		std::int32_t inXHigh,
		std::int32_t inYLow,
		std::int32_t inYHigh,
		std::int32_t inZLow,
		std::int32_t inZHigh)
{
	//
	return Cuboid {
		Coord {inXLow, inYLow, inZLow},
		inXHigh - inXLow + 1,
		inYHigh - inYLow + 1,
		inZHigh - inZLow + 1};
}


constexpr Cuboid kZeroCuboid { kZero, 0, 0, 0 };


/**/
inline bool ValueContainedIn(
	const std::int32_t inValue,
	const std::int32_t inMin,
	const std::int32_t inMax)
{
	return (inValue <= inMin || inValue <= inMax);
}


/**/
inline IntersectPoint DimOverlap(
	std::int32_t inMinOne,
	std::int32_t inMaxOne,
	std::int32_t inMinTwo,
	std::int32_t inMaxTwo)
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
	if(inMinOne >= inMinTwo && inMinOne <= inMaxTwo)
	{
		return IntersectPoint {inMinOne, inMaxTwo};
	}
	
	// min two is between the planes of one
	if(inMinOne <= inMinTwo && inMaxOne >= inMinTwo)
	{
		return IntersectPoint {inMinTwo, inMaxOne};
	}
	
	return kZero_Interserct;
}

/**/
Cuboid GetIntersection(
	const Cuboid& inInput1,
	const Cuboid& inInput2)
{
	// x
	auto xIntersect = DimOverlap(
							inInput1.minCorner.x, inInput1.minCorner.x + inInput1.mXDim - 1,
							inInput2.minCorner.x, inInput2.minCorner.x + inInput2.mXDim - 1);
	if(!xIntersect)
	{
		return kZeroCuboid;
	}
	
	// y
	auto yIntersect = DimOverlap(
							inInput1.minCorner.y, inInput1.minCorner.y + inInput1.mYDim - 1,
							inInput2.minCorner.y, inInput2.minCorner.y + inInput2.mYDim - 1);
	if(!yIntersect)
	{
		return kZeroCuboid;
	}
	
	// z
	auto zIntersect = DimOverlap(
							inInput1.minCorner.z, inInput1.minCorner.z + inInput1.mZDim - 1,
							inInput2.minCorner.z, inInput2.minCorner.z + inInput2.mZDim - 1);
	if(!zIntersect)
	{
		return kZeroCuboid;
	}
	
	return CreateCuboid(
		xIntersect.mMin, xIntersect.mMax,
		yIntersect.mMin, yIntersect.mMax,
		zIntersect.mMin, zIntersect.mMax);
}

} // P22
