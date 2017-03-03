#pragma once
#include "DistanceBase.h"

class CManhattanDistance: public CDistanceBase
{
public:
	CManhattanDistance(void);
	virtual ~CManhattanDistance(void);

	float distanceV(std::vector<int> vVec1, std::vector<int> vVec2) override;
	float distanceV(std::vector<float> vVec1, std::vector<float> vVec2) override;
};