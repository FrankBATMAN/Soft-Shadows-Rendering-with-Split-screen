#pragma once

#include "LightBase.h"
#include "GenerateSamples.h"
#include <vector>
#include <GLM/glm.hpp>
#include "../GraphicsCommon/LBBML_GRAPHICS_INTERFACE_EXPORTS.h"

class LBBML_GRAPHICS_DLL_EXPORTS CLBBMLGraphicsAreaLight : public CLBBMLGraphicsLightBase
{
public:
	CLBBMLGraphicsAreaLight(void);
	virtual ~CLBBMLGraphicsAreaLight(void);

	void generatePointLightSamples(const glm::vec2& vLeftBottom, const glm::vec2& vRightTop, int vSize);
	void setLightColor(const glm::vec3& vColor) {m_LightColor = vColor;}

	void translate(const glm::vec3& vTranslate);
	void scale(const glm::vec3& vScale);
	void rotate(const glm::vec3& vRotAxis, float vRotAngle);

	unsigned int getPointLightSamplesSize() const {return m_PointLightsPos.size();}
	const glm::vec3& getPointLightByIndex(int vIndex) const;
	const glm::vec3& getLightColor() const {return m_LightColor;}
	const glm::vec3 getAreaLightCenter();

private:
	std::vector<glm::vec3> m_PointLightsPos;
	glm::vec3 m_LightColor;
};