#pragma once
#include "../GraphicsCommon/LBBML_GRAPHICS_INTERFACE_EXPORTS.h"

struct SPerspectiveFrustum
{
	float Fovy;
	float Aspect;
	float ZNear;
	float ZFar;
};

class LBBML_GRAPHICS_DLL_EXPORTS CLBBMLGraphicsPerspectiveFrustum
{
public:
	CLBBMLGraphicsPerspectiveFrustum(void);
	virtual ~CLBBMLGraphicsPerspectiveFrustum(void);

	void setPerspective(float vFovy, float vAspect, float vZNear, float vZFar);
	
	const SPerspectiveFrustum& getPerspective() const {return m_Perspective;}

private:
	SPerspectiveFrustum m_Perspective;
};