#pragma once
#include "DistanceBase.h"

class CEuclideanDistance: public CLBBMLDistanceBase
{
public:
	CEuclideanDistance(void){}
	virtual ~CEuclideanDistance(void){}

	float distanceV(const std::vector<float>& v1, const std::vector<float>& v2) override;
};