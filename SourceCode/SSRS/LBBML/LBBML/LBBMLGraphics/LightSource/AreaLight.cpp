#include "AreaLight.h"
#include <GLM/gtc/matrix_transform.hpp>

CLBBMLGraphicsAreaLight::CLBBMLGraphicsAreaLight(void):m_LightColor(1, 1, 1)
{

}

CLBBMLGraphicsAreaLight::~CLBBMLGraphicsAreaLight(void)
{

}

//************************************************************
//Function:
void CLBBMLGraphicsAreaLight::generatePointLightSamples(const glm::vec2& vLeftBottom, const glm::vec2& vRightTop, int vSize)
{
	_ASSERT(vSize > 0, "failed generate area light");

	m_PointLightsPos = LBBMLGraphicsSoftShadowLighting::generateLightSamples3DBySize(vLeftBottom, vRightTop, vSize);
}

//************************************************************
//Function:
void CLBBMLGraphicsAreaLight::translate(const glm::vec3& vTranslate)
{
	for (unsigned int i = 0; i < m_PointLightsPos.size(); ++i)
		m_PointLightsPos[i] += vTranslate;
}

//************************************************************
//Function:
void CLBBMLGraphicsAreaLight::scale(const glm::vec3& vScale)
{
	for (auto Itr : m_PointLightsPos)
	{
		Itr.x *= vScale.x;
		Itr.y *= vScale.y;
		Itr.z *= vScale.z;
	}
	
}

//************************************************************
//Function:
void CLBBMLGraphicsAreaLight::rotate(const glm::vec3& vRotAxis, float vRotAngle)
{
	glm::mat4 RotMat = glm::mat4(1.0);
	RotMat = glm::rotate(RotMat, vRotAngle, vRotAxis);

	for (auto Itr : m_PointLightsPos)
	{
		glm::vec4 Temp = glm::vec4(Itr, 1.0);
		Temp = RotMat * Temp;
		Itr  = glm::vec3(Temp.x, Temp.y, Temp.z);
	}
}

//************************************************************
//Function:
const glm::vec3& CLBBMLGraphicsAreaLight::getPointLightByIndex(int vIndex) const
{
	_ASSERT(vIndex >= 0 && vIndex < m_PointLightsPos.size(), "index out of range");

	return m_PointLightsPos[vIndex];
}

//************************************************************
//Function:
const glm::vec3 CLBBMLGraphicsAreaLight::getAreaLightCenter()
{
	_ASSERT(m_PointLightsPos.size() > 0, "area light not initialized");
	
	glm::vec3 Center = glm::vec3(0.0, 0.0, 0.0);
	for (auto Itr : m_PointLightsPos)
		Center += Itr;
	return Center / (float)m_PointLightsPos.size();
}