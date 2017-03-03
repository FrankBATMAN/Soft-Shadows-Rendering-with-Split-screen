#pragma once
#include "DistanceBase.h"
#include "EuclideanDistance.h"
#include "MahalanobisDistance.h"
#include "ManhattanDistance.h"

enum EDistanceType
{
	EUCLIDEAN_DISTANCE = 0,
	MANHATTAN_DISTANCE,
	MAHANALOBIS_DISTANCE
};

class CLBBMLDistances
{
public:
	CLBBMLDistances(void);
	virtual ~CLBBMLDistances(void);

	float calculateDistance(const std::vector<float>& v1, const std::vector<float>& v2, const EDistanceType& vType);

private:
	CLBBMLDistanceBase* m_Distance;
};