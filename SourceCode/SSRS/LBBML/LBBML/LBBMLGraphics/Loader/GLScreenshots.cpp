#include <stdlib.h>
#include <atlimage.h>
#include "GLScreenshots.h"
#include <math.h>
#include <string>

CGLScreenshots::CGLScreenshots(void)
{

}

CGLScreenshots::~CGLScreenshots(void)
{

}

//********************************************************
//Function:
void CGLScreenshots::saveGLDisplay2Image(const GLuint& vFBOID, GLuint vX, GLuint vY, GLsizei vWidth, GLsizei vHeight, const EDisplayMode& vMode, bool vLinearTransfer)
{
	_ASSERT(vFBOID >= 0, "frame buffer error");
	_ASSERT(vX >= 0 && vY >= 0 && vWidth > 0 && vHeight > 0, "invalid parameters");
	
	if (vMode == DEPTH_DISPLAY)	__saveDepthMap2Image(vFBOID, vX, vY, vWidth, vHeight, vLinearTransfer);
	else if (vMode == RGB_DISPLAY) __saveColorMap2Image(vFBOID, vX, vY, vWidth, vHeight);
	else
		_ASSERT(false, "save image error");
}

//********************************************************
//Function:
void CGLScreenshots::__saveDepthMap2Image(const GLuint& vFBOID, GLuint vX, GLuint vY, GLsizei vWidth, GLsizei vHeight, bool vLinearTransfer)
{
	float* PixelData;
	PixelData = (float*)malloc(vWidth * vHeight * sizeof(float));

	glBindFramebuffer(GL_FRAMEBUFFER, vFBOID);
	glReadPixels(vX, vY, vWidth, vHeight, GL_DEPTH_COMPONENT, GL_FLOAT, PixelData);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float Transfer = 200.0;
	CImage GrayDepthImage;
	GrayDepthImage.Create(vWidth, vHeight, 24, 0);
	for (unsigned int i = 0; i < vHeight; ++i)
		for (unsigned int k = 0; k < vWidth; ++k)
			if (vLinearTransfer)
				GrayDepthImage.SetPixelRGB(k, vHeight -1 -i, pow(PixelData[i * vWidth + k], Transfer) * 255.0, pow(PixelData[i * vWidth + k], Transfer) * 255.0, pow(PixelData[i * vWidth + k], Transfer) * 255.0);
			else
				GrayDepthImage.SetPixelRGB(k, vHeight -1 -i, PixelData[i * vWidth + k] * 255.0, PixelData[i * vWidth + k]* 255.0, PixelData[i * vWidth + k] * 255.0);
	GrayDepthImage.Save("GrayDepthImage.png");

	free(PixelData);
}

//********************************************************
//Function:
void CGLScreenshots::__saveColorMap2Image(const GLuint& vFBOID, GLuint vX, GLuint vY, GLsizei vWidth, GLsizei vHeight)
{
	GLfloat* PixelData;
	PixelData = (GLfloat*)malloc(3 * vWidth * vHeight * sizeof(GLfloat));

	glBindFramebuffer(GL_FRAMEBUFFER, vFBOID);
	glReadPixels(vX, vY, vWidth, vHeight, GL_RGB, GL_FLOAT, PixelData);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CImage ColorImageTarget;
	ColorImageTarget.Create(vWidth, vHeight, 24, 0);
	for (unsigned int i = 0; i < vHeight; ++i)
		for (unsigned int k = 0; k < vWidth; ++k)
			ColorImageTarget.SetPixelRGB(k, vHeight -1 -i, PixelData[(i * vWidth + k) * 3] * 255.0, PixelData[(i * vWidth + k) * 3 + 1] * 255.0, PixelData[(i * vWidth + k) * 3 + 2] * 255.0);
	ColorImageTarget.Save("ColorImage.png");

	free(PixelData);
}