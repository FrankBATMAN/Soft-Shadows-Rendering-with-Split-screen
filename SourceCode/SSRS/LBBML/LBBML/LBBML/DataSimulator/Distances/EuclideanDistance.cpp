#include "EuclideanDistance.h"

float CEuclideanDistance::distanceV(const std::vector<float>& v1, const std::vector<float>& v2)
{
	_ASSERT(!v1.empty() && !v2.empty(), "empty vector");
	_ASSERT(v1.size() == v2.size(), "vectors size not match");

	float EulicdeanDistance = 0.0;
	for (unsigned int index = 0; index < v1.size(); ++index)
		EulicdeanDistance += std::pow(v1[index] - v2[index], 2.0);

	return std::sqrt(EulicdeanDistance);
}