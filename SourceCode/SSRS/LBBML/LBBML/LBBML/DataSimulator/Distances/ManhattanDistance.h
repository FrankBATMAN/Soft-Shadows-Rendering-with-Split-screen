#pragma once
#include "DistanceBase.h"

class CManhattanDistance: public CLBBMLDistanceBase
{
public:
	CManhattanDistance(void){}
	virtual ~CManhattanDistance(void){};

	float distanceV(const std::vector<float>& vVec1, const std::vector<float>& vVec2) override;
};