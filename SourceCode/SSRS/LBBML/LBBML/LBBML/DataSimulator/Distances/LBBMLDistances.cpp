#include "LBBMLDistances.h"

CLBBMLDistances::CLBBMLDistances(void)
{
	
}

CLBBMLDistances::~CLBBMLDistances(void)
{
	if(m_Distance) delete(m_Distance);
}

//**************************************************
//Function:
float CLBBMLDistances::calculateDistance(const std::vector<float>& v1, const std::vector<float>& v2, const EDistanceType& vType)
{
	if (vType == EUCLIDEAN_DISTANCE)
	{
		m_Distance = new CEuclideanDistance;
	}
	else if (vType == MANHATTAN_DISTANCE)
	{
		m_Distance = new CMahalanobisDistance;
	}
	else if (vType == MAHANALOBIS_DISTANCE)
	{
		m_Distance = new CMahalanobisDistance;
	}
	else 
		return -1;

	return m_Distance->distanceV(v1, v2);
}