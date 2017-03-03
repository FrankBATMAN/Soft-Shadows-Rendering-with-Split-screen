#include "PerspectiveFrustum.h"

CLBBMLGraphicsPerspectiveFrustum::CLBBMLGraphicsPerspectiveFrustum(void)
{
	
}

CLBBMLGraphicsPerspectiveFrustum::~CLBBMLGraphicsPerspectiveFrustum(void)
{
	
}

//*****************************************************
//Function:
void CLBBMLGraphicsPerspectiveFrustum::setPerspective(float vFovy, float vAspect, float vZNear, float vZFar)
{
	m_Perspective.Fovy   = vFovy;
	m_Perspective.Aspect = vAspect;
	m_Perspective.ZNear  = vZNear;
	m_Perspective.ZFar   = vZFar;
}