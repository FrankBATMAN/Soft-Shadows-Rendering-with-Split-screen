#pragma once
#include "boundingboxbase.h"
#include "../GraphicsCommon/LBBML_GRAPHICS_INTERFACE_EXPORTS.h"

class LBBML_GRAPHICS_DLL_EXPORTS CLBBMLGraphicsAABB : public CLBBMLGraphicsBoundingBoxBase
{
public:
	CLBBMLGraphicsAABB(void);
	virtual ~CLBBMLGraphicsAABB(void);

	void createV(const std::vector<glm::vec3>& vVertices) override;

	const glm::vec3& getMinCorner() const {return m_MinCorner;}
	const glm::vec3& getMaxCorner() const {return m_MaxCorner;}
	const std::vector<glm::vec3> getAABBVertices();

private:
	void __initialize();

	glm::vec3 m_MinCorner;
	glm::vec3 m_MaxCorner;
};