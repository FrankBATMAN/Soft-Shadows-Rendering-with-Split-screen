#include "OBB.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <eigen3/Eigen/eigen>

CLBBMLGraphicsOBB::CLBBMLGraphicsOBB(void)
{
	m_Center = glm::vec3(0,0,0);

	m_Axis.X_Axis = glm::vec3(0,0,0);
	m_Axis.Y_Axis = glm::vec3(0,0,0);
	m_Axis.Z_Axis = glm::vec3(0,0,0);

	m_HalfLength.X_HalfLength = 0;
	m_HalfLength.Y_HalfLength = 0;
	m_HalfLength.Z_HalfLength = 0;
}

CLBBMLGraphicsOBB::~CLBBMLGraphicsOBB(void)
{

}

//*****************************************************************
//Function:
void CLBBMLGraphicsOBB::createV(const std::vector<glm::vec3>& vVertices)
{
	_ASSERT(vVertices.size() > 0, "error building OBB");

	m_Center = glm::vec3(0.0, 0.0, 0.0);
	for (auto Itr : vVertices)
		m_Center += Itr;
	m_Center /= vVertices.size();

	Eigen::Matrix3f Mat;
	for (unsigned int i = 0; i < 3; ++i)
		for (unsigned int k = 0; k < 3; ++k)
		{
			Mat(i,k) = 0.0;
			for (auto Itr : vVertices)
				Mat(i,k) += (Itr[i] - m_Center[i]) * (Itr[k] - m_Center[k]);
		}
	Mat = Mat / (vVertices.size() - 1);

	Eigen::EigenSolver<Eigen::Matrix3f> EigenSolver(Mat);
	Eigen::Vector3f X = EigenSolver.eigenvectors().col(0).real();
	Eigen::Vector3f Y = EigenSolver.eigenvectors().col(1).real();
	Eigen::Vector3f Z = EigenSolver.eigenvectors().col(2).real();

	m_Axis.X_Axis = glm::normalize(glm::vec3(X[0], X[1], X[2]));
	m_Axis.Y_Axis = glm::normalize(glm::vec3(Y[0], Y[1], Y[2]));
	m_Axis.Z_Axis = glm::normalize(glm::vec3(Z[0], Z[1], Z[2]));

	m_HalfLength.X_HalfLength = -FLT_MAX;
	m_HalfLength.Y_HalfLength = -FLT_MAX;
	m_HalfLength.Z_HalfLength = -FLT_MAX;

	for (auto Itr : vVertices)
	{
		float LenX = 0.0, LenY = 0.0, LenZ = 0.0;
		LenX = abs(glm::dot(Itr, m_Axis.X_Axis));
		LenY = abs(glm::dot(Itr, m_Axis.Y_Axis));
		LenZ = abs(glm::dot(Itr, m_Axis.Z_Axis));

		m_HalfLength.X_HalfLength = m_HalfLength.X_HalfLength < LenX ? LenX : m_HalfLength.X_HalfLength;
		m_HalfLength.Y_HalfLength = m_HalfLength.Y_HalfLength < LenY ? LenY : m_HalfLength.Y_HalfLength;
		m_HalfLength.Z_HalfLength = m_HalfLength.Z_HalfLength < LenZ ? LenZ : m_HalfLength.Z_HalfLength;
	}
}

//*****************************************************************
//Function:
void CLBBMLGraphicsOBB::translate(const glm::vec3& vTrans)
{
	m_Center += vTrans;
}

//*****************************************************************
//Function:
void CLBBMLGraphicsOBB::rotate(const glm::vec3& vAxis, float vAngle)
{
	glm::mat4 RotateMat = glm::mat4(1.0);
	RotateMat = glm::rotate(RotateMat, vAngle, vAxis);

	m_Axis.X_Axis = glm::normalize(__rotateAxis(RotateMat, m_Axis.X_Axis));
	m_Axis.Y_Axis = glm::normalize(__rotateAxis(RotateMat, m_Axis.Y_Axis));
	m_Axis.Z_Axis = glm::normalize(__rotateAxis(RotateMat, m_Axis.Z_Axis));
}

//*****************************************************************
//Function:
void CLBBMLGraphicsOBB::scale(const glm::vec3& vScale)
{
	_ASSERT(vScale.x > 0 && vScale.y > 0 && vScale.z > 0, "wrong scale operation");

	m_HalfLength.X_HalfLength *= vScale.x;
	m_HalfLength.Y_HalfLength *= vScale.y;
	m_HalfLength.Z_HalfLength *= vScale.z;
}

//*****************************************************************
//Function:
const std::vector<glm::vec3> CLBBMLGraphicsOBB::getOBBVertices() const
{
	std::vector<glm::vec3> OBBVertices;
	
	OBBVertices.push_back(__calculateOBBCornerPosition(-1, -1, -1));
	OBBVertices.push_back(__calculateOBBCornerPosition(-1, -1, 1));
	OBBVertices.push_back(__calculateOBBCornerPosition(1, -1, 1));
	OBBVertices.push_back(__calculateOBBCornerPosition(1, -1, -1));

	OBBVertices.push_back(__calculateOBBCornerPosition(-1, 1, -1));
	OBBVertices.push_back(__calculateOBBCornerPosition(-1, 1, 1));
	OBBVertices.push_back(__calculateOBBCornerPosition(1, 1, 1));
	OBBVertices.push_back(__calculateOBBCornerPosition(1, 1, -1));

	return OBBVertices;
}

//*****************************************************************
//Function:
glm::vec3 CLBBMLGraphicsOBB::__rotateAxis(const glm::mat4& vRotateMat, glm::vec3 vAxis)
{
	glm::vec4 RotResult = glm::vec4(vAxis, 1.0);
	RotResult = RotResult * vRotateMat;
	return glm::vec3(RotResult.x, RotResult.y, RotResult.z);
}

//*****************************************************************
//Function:
glm::vec3 CLBBMLGraphicsOBB::__calculateOBBCornerPosition(float vX, float vY, float vZ) const
{
	glm::vec3 X = vX * (m_Axis.X_Axis) * (m_HalfLength.X_HalfLength);
	glm::vec3 Y = vY * (m_Axis.Y_Axis) * (m_HalfLength.Y_HalfLength);
	glm::vec3 Z = vZ * (m_Axis.Z_Axis) * (m_HalfLength.Z_HalfLength);

	return m_Center + X + Y + Z;
}