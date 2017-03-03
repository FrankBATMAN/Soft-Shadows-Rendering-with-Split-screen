#pragma once
#include "DrawableBase.h"
#include <OpenGL/include/openGL/glew.h>
#include <OpenGL/include/openGL/freeglut.h>
#include "../GraphicsCommon/LBBML_GRAPHICS_INTERFACE_EXPORTS.h"

class LBBML_GRAPHICS_DLL_EXPORTS CLBBMLGraphicsQuadDrawable: public CLBBMLGraphicsDrawableBase
{
public:
	CLBBMLGraphicsQuadDrawable(void);
	virtual ~CLBBMLGraphicsQuadDrawable(void);

	virtual void createOBBV(CLBBMLGraphicsOBB* vOBB) override;
	virtual void drawV() override;

private:
	void __initFBO();

	GLuint m_VAO;
	GLuint m_VBO;
};