#pragma once
#include "DistanceBase.h"

class CEuclideanDistance: public CDistanceBase
{
public:
	CEuclideanDistance(void);
	virtual ~CEuclideanDistance(void);

	float distanceV(std::vector<int> vVec1, std::vector<int> vVec2) override;
	float distanceV(std::vector<float> vVec1, std::vector<float> vVec2) override;
};