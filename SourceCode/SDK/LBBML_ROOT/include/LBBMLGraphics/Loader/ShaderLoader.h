#pragma once
#include <string>
#include <OpenGL/include/openGL/glew.h>
#include <OpenGL/include/openGL/freeglut.h>

enum EShaderType
{
	VERTEX_SHADER = 0,
	TESS_CONTROL_SHADER,
	TESS_EVALUATION_SHADER,
	GEOMETRY_SHADER,
	FRAGMENT_SHADER
};

class CLBBMLShaderLoader
{
public:
	CLBBMLShaderLoader(void);
	virtual ~CLBBMLShaderLoader(void);

	void bindShader(GLuint& vProgramID, const EShaderType& vShaderType, const std::string& vShaderFile);

private:
	void __createShader(GLuint& vShaderID, const std::string& vShaderFile);
	void __attachShader(GLuint& vProgramID, GLuint& vShaderID);
};