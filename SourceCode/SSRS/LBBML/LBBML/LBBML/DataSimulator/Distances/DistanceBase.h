#pragma once
#include <vector>

class CLBBMLDistanceBase
{
public:
	CLBBMLDistanceBase(void){}
	virtual ~CLBBMLDistanceBase(void){}

	virtual float distanceV(const std::vector<float>& v1, const std::vector<float>& v2) = 0;
};