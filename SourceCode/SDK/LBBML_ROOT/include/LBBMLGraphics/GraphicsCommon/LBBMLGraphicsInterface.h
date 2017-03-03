#pragma  once

#include <vector>
#include "LBBML_GRAPHICS_INTERFACE_EXPORTS.h"
#include "../Loader/ShaderLoader.h"
#include "../Loader/GLScreenshots.h"
#include "../Loader/TextureLoader.h"
#include "../BoundingBox/FrustumBB.h"
#include "../BoundingBox/OBB.h"
#include "../Viewer/PerspectiveFrustum.h"

enum EFBOMode
{
	RGBA_FBO = 0,
	DEPTH_FBO,
	RED_FBO,
	RG_FBO
};

namespace LBBMLGraphics
{
	LBBML_GRAPHICS_DLL_EXPORTS void createShaderProgram(GLuint& vShaderProgram);
	LBBML_GRAPHICS_DLL_EXPORTS void bindShader(GLuint& vProgramID, const EShaderType& vShaderType, const std::string& vShaderFile);
	LBBML_GRAPHICS_DLL_EXPORTS void linkShaderProgram(GLuint& vShaderProgram);

	LBBML_GRAPHICS_DLL_EXPORTS void createFBO(GLuint& vTextureObject, GLuint& vFBO, GLuint vWidth, GLuint vHeight, const EFBOMode& vMode);
	LBBML_GRAPHICS_DLL_EXPORTS void deleteFBO(GLuint& vTextureObject, GLuint& vFBO);

	LBBML_GRAPHICS_DLL_EXPORTS GLuint loadImageAsTexture(GLenum vTextureType, const EImageChannel& vChannel, const std::string& vImageFilePath);
	LBBML_GRAPHICS_DLL_EXPORTS void saveGLDisplay2Image(const GLuint& vFBOID, GLuint vX, GLuint vY, GLsizei vW, GLsizei vH, const EDisplayMode& vMode, bool vLinearTransfer);

	LBBML_GRAPHICS_DLL_EXPORTS void createFrustumBB(CLBBMLGraphicsFrustumBB* vFBB, const CLBBMLGraphicsPerspectiveFrustum& vPF, const glm::vec3& vPos, const glm::vec3& vDir, const glm::vec3& vUp);
	LBBML_GRAPHICS_DLL_EXPORTS bool isOBBInFrustum(const CLBBMLGraphicsOBB& vOBB, const CLBBMLGraphicsFrustumBB& vFBB);
}