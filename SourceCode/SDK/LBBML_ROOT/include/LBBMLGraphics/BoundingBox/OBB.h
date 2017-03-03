#pragma once
#include "BoundingBoxBase.h"
#include "../GraphicsCommon/LBBML_GRAPHICS_INTERFACE_EXPORTS.h"

struct SOBBAxis
{
	glm::vec3 X_Axis;
	glm::vec3 Y_Axis;
	glm::vec3 Z_Axis;
};

struct SOBBHalfLength
{
	float X_HalfLength;
	float Y_HalfLength;
	float Z_HalfLength;
};

class LBBML_GRAPHICS_DLL_EXPORTS CLBBMLGraphicsOBB: public CLBBMLGraphicsBoundingBoxBase
{
public:
	CLBBMLGraphicsOBB(void);
	virtual ~CLBBMLGraphicsOBB(void);

	void createV(const std::vector<glm::vec3>& vVertices) override;

	void translate(const glm::vec3& vTrans);
	void rotate(const glm::vec3& vAxis, float vAngle);
	void scale(const glm::vec3& vScale);

	const glm::vec3& getCenter() const {return m_Center;}
	const SOBBAxis& getAxis() const {return m_Axis;}
	const SOBBHalfLength& getHalfLength() const {return m_HalfLength;}
	const std::vector<glm::vec3> getOBBVertices() const;

private:
	glm::vec3 __rotateAxis(const glm::mat4& vRotateMat, glm::vec3 vAxis);
	glm::vec3 __calculateOBBCornerPosition(float vX, float vY, float vZ) const;

	glm::vec3 m_Center;
	SOBBAxis m_Axis;
	SOBBHalfLength m_HalfLength;
};