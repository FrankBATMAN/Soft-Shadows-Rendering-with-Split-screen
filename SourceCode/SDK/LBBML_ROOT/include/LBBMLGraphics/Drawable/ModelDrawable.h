#pragma once
#include "DrawableBase.h"
#include <string>
#include <vector>
#include <GLM/glm.hpp>
#include <OpenGL/include/openGL/glew.h>
#include <OpenGL/include/openGL/freeglut.h>
#include "../GraphicsCommon/LBBML_GRAPHICS_INTERFACE_EXPORTS.h"

struct SVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class LBBML_GRAPHICS_DLL_EXPORTS CLBBMLGraphicsModelDrawable: public CLBBMLGraphicsDrawableBase
{
public:
	CLBBMLGraphicsModelDrawable(const std::string& vFilePath);
	virtual ~CLBBMLGraphicsModelDrawable(void);

	virtual void createOBBV(CLBBMLGraphicsOBB* vOBB) override;
	virtual void drawV() override;

private:
	void __LoadModelFromFile(const std::string& vFilePath);
	void __initFBO();

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	std::vector<SVertex> m_Vertices;
	std::vector<GLuint> m_Indices;
};