#pragma once
#include "../BoundingBox/OBB.h"
#include "../GraphicsCommon/LBBML_GRAPHICS_INTERFACE_EXPORTS.h"

class LBBML_GRAPHICS_DLL_EXPORTS CLBBMLGraphicsDrawableBase
{
public:
	CLBBMLGraphicsDrawableBase(void);
	virtual ~CLBBMLGraphicsDrawableBase(void);

	virtual void createOBBV(CLBBMLGraphicsOBB* vOBB) = 0;
	virtual void drawV() = 0;
	
	int getBatches() const {return m_Batches;}
	int getTriangles() const {return m_Triangles;}

protected:
	void _setBatches(int vBatches) {m_Batches = vBatches;}
	void _setTriangles(int vTriangles) {m_Triangles = vTriangles;}

private:
	int m_Batches;
	int m_Triangles;
};