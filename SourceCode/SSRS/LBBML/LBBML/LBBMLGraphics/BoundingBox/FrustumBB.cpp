#include "FrustumBB.h"

CLBBMLGraphicsFrustumBB::CLBBMLGraphicsFrustumBB(void)
{

}

CLBBMLGraphicsFrustumBB::~CLBBMLGraphicsFrustumBB(void)
{

}

//************************************************************
//Function:
void CLBBMLGraphicsFrustumBB::createV(const std::vector<glm::vec3>& vVertices)
{
	_ASSERT(vVertices.size() == 8, "error building SBB");

	m_Planes.resize(6);
	__calculateFrustumPlane(0, vVertices[0], vVertices[1], vVertices[2], vVertices[3]);
	__calculateFrustumPlane(1, vVertices[3], vVertices[2], vVertices[6], vVertices[7]);
	__calculateFrustumPlane(2, vVertices[0], vVertices[3], vVertices[7], vVertices[4]);
	__calculateFrustumPlane(3, vVertices[0], vVertices[4], vVertices[5], vVertices[1]);
	__calculateFrustumPlane(4, vVertices[2], vVertices[1], vVertices[5], vVertices[6]);
	__calculateFrustumPlane(5, vVertices[5], vVertices[4], vVertices[7], vVertices[6]);
}

//****************************************************************
//Function:
void CLBBMLGraphicsFrustumBB::__calculateFrustumPlane(int vPlaneIndex, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4)
{
	m_Planes[vPlaneIndex].Center = (v1 + v2 + v3 + v4) / (float)4.0;
	m_Planes[vPlaneIndex].Normal = glm::normalize(glm::cross(v2-v1, v4-v1));
}