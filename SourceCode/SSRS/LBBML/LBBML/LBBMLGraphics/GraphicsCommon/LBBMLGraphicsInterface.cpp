#include "LBBMLGraphicsInterface.h"
#include <iostream>

//*********************************************************************************************
//Function:
void LBBMLGraphics::createShaderProgram(GLuint& vShaderProgram)
{
	vShaderProgram = glCreateProgram();
	if(vShaderProgram == 0)
	{
		_ASSERT(false, "error create shader program");
	}
}

//*********************************************************************************************
//Function:
void LBBMLGraphics::bindShader(GLuint& vProgramID, const EShaderType& vShaderType, const std::string& vShaderFile)
{
	CLBBMLShaderLoader* pShaderLoader = new CLBBMLShaderLoader;
	pShaderLoader->bindShader(vProgramID, vShaderType, vShaderFile);
	delete(pShaderLoader);
}

//*********************************************************************************************
//Function:
void LBBMLGraphics::linkShaderProgram(GLuint& vShaderProgram)
{
	GLint Result = GL_FALSE;

	glLinkProgram(vShaderProgram);
	glGetProgramiv(vShaderProgram, GL_LINK_STATUS, &Result);
	if (!Result)
	{
		GLchar ErrorLog[1024];
		glGetProgramInfoLog(vShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error Linking program : '%s'\n", ErrorLog);
	}
}

//*********************************************************************************************
//Function:
void LBBMLGraphics::createFBO(GLuint& vTextureObject, GLuint& vFBO, GLuint vWidth, GLuint vHeight, const EFBOMode& vMode)
{
	vTextureObject = 0, vFBO = 0;
	glGenTextures(1, &vTextureObject);
	glBindTexture(GL_TEXTURE_2D, vTextureObject);

	switch (vMode)
	{
	case RGBA_FBO:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, vWidth, vHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		break;
	case DEPTH_FBO:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, vWidth, vHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		break;
	case RED_FBO:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, vWidth, vHeight, 0, GL_RED, GL_FLOAT, NULL);
		break;
	case RG_FBO:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, vWidth, vHeight, 0, GL_RG, GL_FLOAT, NULL);
		break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &vFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, vFBO);
	
	if (vMode == DEPTH_FBO)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, vTextureObject, 0);
	}
	else
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vTextureObject, 0);
	}
	
	GLenum Test = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(Test != GL_FRAMEBUFFER_COMPLETE)
		_ASSERT(false, "Fail to initialize frame buffer!\n");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//*********************************************************************************************
//Function:
void LBBMLGraphics::deleteFBO(GLuint& vTextureObject, GLuint& vFBO)
{
	glDeleteTextures(1, &vTextureObject);
	glDeleteFramebuffers(1, &vFBO);
}

//*********************************************************************************************
//Function:
GLuint LBBMLGraphics::loadImageAsTexture(GLenum vTextureType, const EImageChannel& vChannel, const std::string& vImageFilePath)
{
	if(vTextureType == GL_TEXTURE_2D)
	{
		GLuint TextureID = 0;
		TextureLoader::loadImageAsTexture(TextureID, vChannel, vImageFilePath);
		return TextureID;
	}
	else
	{
		std::cout << "unsupported texture type, use GL_TEXTURE_2D instead" << std::endl;
		_ASSERT(false);
	}
}

//*********************************************************************************************
//Function:
void LBBMLGraphics::saveGLDisplay2Image(const GLuint& vFBOID, GLuint vX, GLuint vY, GLsizei vW, GLsizei vH, const EDisplayMode& vMode, bool vLinearTransfer)
{
	CGLScreenshots* pGLScreenshots = new CGLScreenshots;

	pGLScreenshots->saveGLDisplay2Image(vFBOID, vX, vY, vW, vH, vMode, vLinearTransfer);

	delete(pGLScreenshots);
}

//*********************************************************************************************
//Function:
void LBBMLGraphics::createFrustumBB(CLBBMLGraphicsFrustumBB* vFBB, const CLBBMLGraphicsPerspectiveFrustum& vPF, const glm::vec3& vPos, const glm::vec3& vDir, const glm::vec3& vUp)
{
	if(vFBB == nullptr) vFBB = new CLBBMLGraphicsFrustumBB;

	glm::vec3 Forward = glm::normalize(vDir - vPos);
	glm::vec3 Up = glm::normalize(vUp);
	glm::vec3 Right = glm::normalize(glm::cross(Forward, Up));

	glm::vec3 NearPlaneCenter = vPos + vPF.getPerspective().ZNear * Forward;
	float NearPlaneHalfHeight = tan(vPF.getPerspective().Fovy / 2) * vPF.getPerspective().ZNear;
	float NearPlaceHalfWidth  = NearPlaneHalfHeight * vPF.getPerspective().Aspect;

	glm::vec3 FarPlaneCenter = vPos + vPF.getPerspective().ZFar * Forward;
	float FarPlaneHalfHeight = tan(vPF.getPerspective().Fovy / 2) * vPF.getPerspective().ZFar;
	float FarPlaneHalfWidth  = FarPlaneHalfHeight * vPF.getPerspective().Aspect;

	std::vector<glm::vec3> FrustumVertices;

	FrustumVertices.push_back(NearPlaneCenter - NearPlaceHalfWidth * Right - NearPlaneHalfHeight * Up);
	FrustumVertices.push_back(NearPlaneCenter - NearPlaceHalfWidth * Right + NearPlaneHalfHeight * Up);
	FrustumVertices.push_back(NearPlaneCenter + NearPlaceHalfWidth * Right + NearPlaneHalfHeight * Up);
	FrustumVertices.push_back(NearPlaneCenter + NearPlaceHalfWidth * Right - NearPlaneHalfHeight * Up);
	FrustumVertices.push_back(FarPlaneCenter - FarPlaneHalfWidth * Right - FarPlaneHalfHeight * Up);
	FrustumVertices.push_back(FarPlaneCenter - FarPlaneHalfWidth * Right + FarPlaneHalfHeight * Up);
	FrustumVertices.push_back(FarPlaneCenter + FarPlaneHalfWidth * Right + FarPlaneHalfHeight * Up);
	FrustumVertices.push_back(FarPlaneCenter + FarPlaneHalfWidth * Right - FarPlaneHalfHeight * Up);

	vFBB->createV(FrustumVertices);
}

//****************************************************************
//Function:
bool LBBMLGraphics::isOBBInFrustum(const CLBBMLGraphicsOBB& vOBB, const CLBBMLGraphicsFrustumBB& vFBB)
{
	unsigned int OBBInvisibleVertex = 0;
	std::vector<glm::vec3> OBBVertices = vOBB.getOBBVertices();
	for (auto OBBVerticesItr : OBBVertices)
	{
		for (auto PlaneItr : vFBB.getFrustumPlanes())
		{
			if (glm::dot(OBBVerticesItr - PlaneItr.Center, PlaneItr.Normal) < 0.0)
			{
				OBBInvisibleVertex++;
				break;
			}
		}
	}

	return OBBInvisibleVertex != OBBVertices.size();
}