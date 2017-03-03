#pragma once
#include <OpenGL/include/openGL/glew.h>
#include <OpenGL/include/openGL/freeglut.h>
#include <Soil/include/SOIL.h>
#include <string>

enum EImageChannel
{
	LOAD_AUTO = 0,
	LOAD_AS_GREY,
	LOAD_AS_RGB,
	LOAD_AS_RGBA
};

namespace TextureLoader
{
	void loadImageAsTexture(GLuint& vTexture, const EImageChannel& vType, const std::string& vImageFilePath);
	void soilLoadGLTexture(GLuint& vTexture, unsigned int vChannel, const std::string& vFilePath);
}