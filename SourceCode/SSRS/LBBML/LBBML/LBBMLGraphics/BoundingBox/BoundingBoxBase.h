#pragma once
#include <vector>
#include <GLM/glm.hpp>

class CLBBMLGraphicsBoundingBoxBase
{
public:
	CLBBMLGraphicsBoundingBoxBase(void){}
	virtual ~CLBBMLGraphicsBoundingBoxBase(void){}

	virtual void createV(const std::vector<glm::vec3>& vVertices) = 0;
};