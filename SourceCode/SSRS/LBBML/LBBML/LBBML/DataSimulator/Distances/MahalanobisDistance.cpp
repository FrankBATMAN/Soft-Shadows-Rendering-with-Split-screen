#include "MahalanobisDistance.h"
#include <iostream>

//*************************************************************
//Function:
float CMahalanobisDistance::distanceV(const std::vector<float>& vVec1, const std::vector<float>& vVec2)
{
	_ASSERT(!vVec1.empty() && !vVec2.empty(), "empty vector");
	_ASSERT(vVec1.size() == vVec2.size(), "vectors size not match");

	Eigen::VectorXf X_Y(vVec1.size());
	for (unsigned int index = 0; index < vVec1.size(); ++index)
		X_Y[index] = vVec1[index] - vVec2[index];

	Eigen::VectorXf Transpose_of_X_Y = X_Y.transpose();

	Eigen::MatrixXf CovarianceMatrix = __calculateCovarianceMatrix(vVec1, vVec2);
	CovarianceMatrix.inverse();

	Transpose_of_X_Y = (Transpose_of_X_Y.transpose()) * CovarianceMatrix;
	float MahanalibisDistance = 0.0;

	MahanalibisDistance = X_Y.dot(Transpose_of_X_Y);

	return MahanalibisDistance >= 0 ? std::sqrt(MahanalibisDistance) : FLT_MAX;
}

//**************************************************************
//Function:
Eigen::MatrixXf CMahalanobisDistance::__calculateCovarianceMatrix(const std::vector<float>& vVec1, const std::vector<float>& vVec2)
{
	int Row = vVec1.size(), Col = vVec2.size();
	Eigen::MatrixXf CovarianceMatrix = Eigen::MatrixXf(Row, Col);
	for (unsigned int i = 0; i < Row; ++i)
		for (unsigned int k = 0; k < Col; ++k)
			CovarianceMatrix(i,k) = (vVec1[i] - vVec2[i]) * (vVec1[k] - vVec2[k]);

	return CovarianceMatrix;
}