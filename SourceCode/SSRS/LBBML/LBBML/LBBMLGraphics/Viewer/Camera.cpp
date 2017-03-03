#include "Camera.h"

CLBBMLGraphicsCamera::CLBBMLGraphicsCamera(void)
{
	__initialize();
}

CLBBMLGraphicsCamera::~CLBBMLGraphicsCamera(void)
{

}

//******************************************************************
//FUNCTION:
void CLBBMLGraphicsCamera::initPositions(int vSize, float vXMin, float vXMax, float vYMin, float vYMax, float vZMin, float vZMax)
{
	_ASSERT(vSize > 0, "size must be greater than zero");

	for (unsigned int i = 0; i < vSize; ++i)
	{
		glm::vec3 Position;

		Position.x = LBBMLMathWorker::generateRandomf(vXMin, vXMax);
		Position.y = LBBMLMathWorker::generateRandomf(vYMin, vYMax);
		Position.z = LBBMLMathWorker::generateRandomf(vZMin, vZMax);

		m_Positions.push_back(Position);
	}
}

//******************************************************************
//FUNCTION:
void CLBBMLGraphicsCamera::initPositions(int vSize, const glm::vec2& vRangeX, const glm::vec2& vRangeY, const glm::vec2& vRangeZ)
{
	initPositions(vSize, vRangeX[0], vRangeX[1],vRangeY[0], vRangeY[1],vRangeZ[0], vRangeZ[1]);
}

//******************************************************************
//FUNCTION:
const glm::vec3& CLBBMLGraphicsCamera::getPositionByIndex(int vIndex) const
{
	_ASSERT(m_Positions.size() > 0, "camera positions can not be empty");

	return m_Positions[vIndex % m_Positions.size()];
}

//******************************************************************
//FUNCTION:
void CLBBMLGraphicsCamera::__initialize()
{
	m_Positions.resize(0);
	m_LookAt = glm::vec3(0, 0, 0);
	m_Up = glm::vec3(0, 1, 0);
}