#pragma once
#include "DistanceBase.h"
#include <eigen3/Eigen/LU>

class CMahalanobisDistance: public CDistanceBase
{
public:
	CMahalanobisDistance(void);
	virtual ~CMahalanobisDistance(void);

	float distanceV(std::vector<int> vVec1, std::vector<int> vVec2) override;
	float distanceV(std::vector<float> vVec1, std::vector<float> vVec2) override;

private:
	Eigen::MatrixXf __calculateCovarianceMatrix(std::vector<int> vVec1, std::vector<int> vVec2);
	Eigen::MatrixXf __calculateCovarianceMatrix(std::vector<float> vVec1, std::vector<float> vVec2);
	void __inverseMatrix(Eigen::MatrixXf& vMatrix);
};