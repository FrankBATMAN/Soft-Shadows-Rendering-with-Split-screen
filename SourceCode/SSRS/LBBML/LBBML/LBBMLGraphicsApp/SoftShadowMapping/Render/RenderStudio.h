#pragma once
#include "../../../LBBMLGraphics/GraphicsCommon/LBBMLGraphicsInterface.h"
#include "../../../LBBMLGraphics/GraphicsCommon/LBBMLGraphicsStructure.h"
#include "../../../LBBMLGraphics/Drawable/DrawableBase.h"

class CSoftShadowMappingRenderStudio
{
public:
	CSoftShadowMappingRenderStudio(void);
	virtual ~CSoftShadowMappingRenderStudio(void);

	void setRenderObj(CLBBMLGraphicsDrawableBase* vDrawedObject);
	void setOrientedBoundingBox(CLBBMLGraphicsOBB* vOBB);
	void setTransform(LBBMLGraphicsStructure::STransform vTransform);
	
	void render();
	void translate(const glm::vec3& vTranslate);
	void scale(const glm::vec3& vScale);
	void rotate(const glm::vec3& vRotAxis, float vRotAngle);

	int fetchTriangleNumber() const {return m_pRenderObj->getTriangles();}
	int fetchBatchNumber() const {return m_pRenderObj->getBatches();}

	CLBBMLGraphicsOBB* getOrientedBoundingBox() const {return m_pOBB;}
	const LBBMLGraphicsStructure::STransform& getTransform() const {return m_Transform;}

private:
	void __releaseVariables();

	CLBBMLGraphicsDrawableBase* m_pRenderObj;
	CLBBMLGraphicsOBB* m_pOBB;
	LBBMLGraphicsStructure::STransform m_Transform;
};

