#pragma once
#include <vector>

class CDistanceBase
{
public:
	CDistanceBase(void){};
	virtual ~CDistanceBase(void){};

	virtual float distanceV(std::vector<int> vVec1, std::vector<int> vVec2) = 0;
	virtual float distanceV(std::vector<float> vVec1, std::vector<float> vVec2) = 0;
};