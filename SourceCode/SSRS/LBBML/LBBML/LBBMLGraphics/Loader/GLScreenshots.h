#pragma once
#include <OpenGL/include/openGL/glew.h>
#include <OpenGL/include/openGL/freeglut.h>

enum EDisplayMode
{
	RGB_DISPLAY = 0,
	DEPTH_DISPLAY
};

class CGLScreenshots
{
public:
	CGLScreenshots(void);
	virtual ~CGLScreenshots(void);

	void saveGLDisplay2Image(const GLuint& vFBOID, GLuint vX, GLuint vY, GLsizei vWidth, GLsizei vHeight, const EDisplayMode& vMode, bool vLinearTransfer);

private:
	void __saveDepthMap2Image(const GLuint& vFBOID, GLuint vX, GLuint vY, GLsizei vWidth, GLsizei vHeight, bool vLinearTransfer);
	void __saveColorMap2Image(const GLuint& vFBOID, GLuint vX, GLuint vY, GLsizei vWidth, GLsizei vHeight);
};