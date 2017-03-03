#include "AABB.h"

CLBBMLGraphicsAABB::CLBBMLGraphicsAABB()
{
	__initialize();
}

CLBBMLGraphicsAABB::~CLBBMLGraphicsAABB()
{

}

//********************************************************************************
//Function:
void CLBBMLGraphicsAABB::createV(const std::vector<glm::vec3>& vVertices)
{
	_ASSERT(vVertices.size() > 0, "error building AABB");
	
	for (int i = 0; i < vVertices.size(); ++i)
	{
		for (int k = 0; k < vVertices[i].length(); ++k)
		{
			m_MaxCorner[k] = m_MaxCorner[k] > vVertices[i][k] ? m_MaxCorner[k] : vVertices[i][k];
			m_MinCorner[k] = m_MinCorner[k] < vVertices[i][k] ? m_MinCorner[k] : vVertices[i][k];
		}
	}
}

//******************************************************************
//FUNCTION:
const std::vector<glm::vec3> CLBBMLGraphicsAABB::getAABBVertices()
{
	std::vector<glm::vec3> BoxVertices;

	BoxVertices.push_back(m_MinCorner);
	BoxVertices.push_back(glm::vec3(m_MaxCorner.x, m_MinCorner.y, m_MinCorner.z));
	BoxVertices.push_back(glm::vec3(m_MaxCorner.x, m_MaxCorner.y, m_MinCorner.z));
	BoxVertices.push_back(glm::vec3(m_MinCorner.x, m_MaxCorner.y, m_MinCorner.z));
	BoxVertices.push_back(m_MaxCorner);
	BoxVertices.push_back(glm::vec3(m_MinCorner.x, m_MaxCorner.y, m_MaxCorner.z));
	BoxVertices.push_back(glm::vec3(m_MinCorner.x, m_MinCorner.y, m_MaxCorner.z));
	BoxVertices.push_back(glm::vec3(m_MaxCorner.x, m_MinCorner.y, m_MaxCorner.z));

	return BoxVertices;
}

//******************************************************************
//FUNCTION:
void CLBBMLGraphicsAABB::__initialize()
{
	m_MinCorner = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_MaxCorner = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}