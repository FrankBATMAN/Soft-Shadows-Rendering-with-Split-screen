#include "RenderStudio.h"

CSoftShadowMappingRenderStudio::CSoftShadowMappingRenderStudio(void)
{
}


CSoftShadowMappingRenderStudio::~CSoftShadowMappingRenderStudio(void)
{
	__releaseVariables();
}

//*********************************************************************************
//FUNCTION:
void CSoftShadowMappingRenderStudio::setRenderObj(CLBBMLGraphicsDrawableBase* vDrawedObject)
{
	_ASSERTE(vDrawedObject);
	m_pRenderObj = vDrawedObject;
}

//*********************************************************************************
//FUNCTION:
void CSoftShadowMappingRenderStudio::setOrientedBoundingBox(CLBBMLGraphicsOBB* vOBB)
{
	_ASSERTE(vOBB);
	m_pOBB = vOBB;

	if(m_pRenderObj)
		m_pRenderObj->createOBBV(m_pOBB);
}

//*********************************************************************************
//FUNCTION:
void CSoftShadowMappingRenderStudio::setTransform(LBBMLGraphicsStructure::STransform vTransform)
{
	m_Transform = vTransform;

	m_pOBB->translate(m_Transform.m_Position);
	m_pOBB->scale(m_Transform.m_Scale);
	m_pOBB->rotate(m_Transform.m_RotAxis, m_Transform.m_RotAngle);
}

//*********************************************************************************
//FUNCTION:
void CSoftShadowMappingRenderStudio::render()
{
	m_pRenderObj->drawV();
}

//***********************************************************************************
//Function:
void CSoftShadowMappingRenderStudio::translate(const glm::vec3& vTranslate)
{
	m_pOBB->translate(vTranslate);

	m_Transform.m_Position += vTranslate;
}

//***********************************************************************************
//Function:
void CSoftShadowMappingRenderStudio::scale(const glm::vec3& vScale)
{
	_ASSERTE(vScale.x > 0 && vScale.y > 0 && vScale.z > 0);
	
	m_pOBB->scale(vScale);

	m_Transform.m_Scale *= m_Transform.m_Scale;
}

//*************************************************************************************
//Function:
void CSoftShadowMappingRenderStudio::rotate(const glm::vec3& vRotAxis, float vRotAngle)
{
	//TO DO:calculate global rotate after each step of rotate
	//m_Transform.rotAxis && rotAngle

	m_pOBB->rotate(vRotAxis, vRotAngle);
}

//*********************************************************************************
//FUNCTION:
void CSoftShadowMappingRenderStudio::__releaseVariables()
{
	if(m_pOBB) delete(m_pOBB);
	if(m_pRenderObj) delete(m_pRenderObj);
}