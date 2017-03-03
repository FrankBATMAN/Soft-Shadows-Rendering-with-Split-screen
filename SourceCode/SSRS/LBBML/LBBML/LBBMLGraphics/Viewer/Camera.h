#pragma once

#include <GLM/glm.hpp>
#include "../../LBBML/MathWorker/LBBMLMathWorker.h"
#include "../GraphicsCommon/LBBML_GRAPHICS_INTERFACE_EXPORTS.h"

class LBBML_GRAPHICS_DLL_EXPORTS CLBBMLGraphicsCamera
{
public:
	CLBBMLGraphicsCamera(void);
	virtual ~CLBBMLGraphicsCamera(void);

	void initPositions(int vSize, float vXMin, float vXMax, float vYMin, float vYMax, float vZMin, float vZMax);
	void initPositions(int vSize, const glm::vec2& vRangeX, const glm::vec2& vRangeY, const glm::vec2& vRangeZ);
	void setDirection(const glm::vec3& vLookAt) {m_LookAt = vLookAt;}
	void setCameraUp(const glm::vec3& vUp) {m_Up = vUp;}
	
	int getCameraPositionsSetSize()	const {return m_Positions.size();}

	const glm::vec3& getPositionByIndex(int vIndex)  const;
	const glm::vec3& getLookAt() const {return m_LookAt;}
	const glm::vec3& getUp() const {return m_Up;}

private:
	void __initialize();

	std::vector<glm::vec3> m_Positions;
	glm::vec3 m_LookAt;
	glm::vec3 m_Up;	
};