#pragma once
#include "DistanceBase.h"
#include <eigen3/Eigen/LU>

class CMahalanobisDistance: public CLBBMLDistanceBase
{
public:
	CMahalanobisDistance(void){}
	virtual ~CMahalanobisDistance(void){}

	float distanceV(const std::vector<float>& vVec1, const std::vector<float>& vVec2) override;

private:

	Eigen::MatrixXf __calculateCovarianceMatrix(const std::vector<float>& vVec1, const std::vector<float>& vVec2);
};