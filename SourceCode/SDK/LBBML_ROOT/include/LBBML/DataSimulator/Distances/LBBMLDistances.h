#pragma once
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

	float calculateDistance(std::vector<float> v1, std::vector<float> v2, const EDistanceType& vType);
	float calculateDistance(std::vector<int> v1, std::vector<int> v2, const EDistanceType& vType);

private:

	CEuclideanDistance* m_pEuclideanDis;
	CMahalanobisDistance* m_pMahalanobisDis;
	CManhattanDistance* m_pManhattanDis;
};