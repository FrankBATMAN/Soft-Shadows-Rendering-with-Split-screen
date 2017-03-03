#include "ShaderLoader.h"
#include <fstream>
#include <iostream>

CLBBMLShaderLoader::CLBBMLShaderLoader(void)
{

}

CLBBMLShaderLoader::~CLBBMLShaderLoader(void)
{

}

//*****************************************************************************************
//Function:
void CLBBMLShaderLoader::bindShader(GLuint& vProgramID, const EShaderType& vShaderType, const std::string& vShaderFile)
{
	_ASSERT(!vShaderFile.empty(), "invalid file path");
	if(vProgramID == 0)
		vProgramID = glCreateProgram();

	GLuint ShaderID = 0;
	switch (vShaderType)
	{
	case VERTEX_SHADER:
		ShaderID = glCreateShader(GL_VERTEX_SHADER);
		break;
	case TESS_CONTROL_SHADER:
		ShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
		break;
	case TESS_EVALUATION_SHADER:
		ShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
		break;
	case GEOMETRY_SHADER:
		ShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case FRAGMENT_SHADER:
		ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	default:
		break;
	}
	__createShader(ShaderID, vShaderFile);
	__attachShader(vProgramID, ShaderID);
}

//******************************************************************************************
//Function:
void CLBBMLShaderLoader::__createShader(GLuint& vShaderID, const std::string& vShaderFile)
{
	//read shader code
	std::string ShaderCode = "";
	std::ifstream ShaderFile(vShaderFile);
	if (ShaderFile.is_open())
	{
		std::string Line = "";
		while(getline(ShaderFile, Line))
		{
			Line = "\n" + Line;
			ShaderCode += Line;
		}
		ShaderFile.close();
	}
	else
	{
		std::cout << "Cannot open shader file:	" << vShaderFile << std::endl;
		return;
	}

	//compile shader code
	const char* ShaderSource = ShaderCode.c_str();
	glShaderSource(vShaderID, 1, &ShaderSource, NULL);
	glCompileShader(vShaderID);

	//check shader
	GLint Result = GL_FALSE;
	glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &Result);
	if (!Result)
	{
		GLchar ErrorLog[1024];
		glGetShaderInfoLog(vShaderID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error compiling shader '%s' : '%s'\n", vShaderFile, ErrorLog);
	}
}

//******************************************************************************************
//Function:
void CLBBMLShaderLoader::__attachShader(GLuint& vProgramID, GLuint& vShaderID)
{
	glAttachShader(vProgramID, vShaderID);
}