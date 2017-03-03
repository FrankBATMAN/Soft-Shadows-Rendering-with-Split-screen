#include "TextureLoader.h"

//******************************************************************************************************
//Function:
void TextureLoader::loadImageAsTexture(GLuint& vTexture, const EImageChannel& vType, const std::string& vImageFilePath)
{
	_ASSERTE(!vImageFilePath.empty(), "file path can not be empty");

	switch (vType)
	{
	case LOAD_AUTO:
		soilLoadGLTexture(vTexture, SOIL_LOAD_AUTO, vImageFilePath);
		break;
	case LOAD_AS_GREY:
		soilLoadGLTexture(vTexture, SOIL_LOAD_L, vImageFilePath);
		break;
	case LOAD_AS_RGB:
		soilLoadGLTexture(vTexture, SOIL_LOAD_RGB, vImageFilePath);
		break;
	case LOAD_AS_RGBA:
		soilLoadGLTexture(vTexture, SOIL_LOAD_RGBA, vImageFilePath);
		break;
	default:
		_ASSERT(false, "invalid image channels");
	}
}

//******************************************************************************************************
//Function:
void TextureLoader::soilLoadGLTexture(GLuint& vTexture, unsigned int vChannel, const std::string& vFilePath)
{
	vTexture = SOIL_load_OGL_texture(vFilePath.c_str(), vChannel, vTexture, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	
	if(vTexture == 0)
	{
		_ASSERT(false, "failed load image as texture");
	}
}