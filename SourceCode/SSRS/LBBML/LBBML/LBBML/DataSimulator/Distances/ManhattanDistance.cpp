#include "ManhattanDistance.h"

//*************************************************************
//Function:
float CManhattanDistance::distanceV(const std::vector<float>& vVec1, const std::vector<float>& vVec2)
{
	_ASSERT(!vVec1.empty() && !vVec2.empty(), "empty vector");
	_ASSERT(vVec1.size() == vVec2.size(), "vectors size not match");

	float ManhattanDistance = 0.0;
	for (unsigned int index = 0; index < vVec1.size(); ++index)
		ManhattanDistance += std::abs(vVec1[index] - vVec2[index]);
	return ManhattanDistance;
}