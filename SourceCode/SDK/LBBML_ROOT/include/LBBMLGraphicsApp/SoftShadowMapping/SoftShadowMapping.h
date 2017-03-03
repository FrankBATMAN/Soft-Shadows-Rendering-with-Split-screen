#pragma once
#include <string>
#include <math.h>
#include <vector>
#include <iostream>
#include <atlimage.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <OpenGL/include/openGL/glew.h>
#include <OpenGL/include/openGL/glut.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "../../LBBML/Common/LBBMLInterface.h"
#include "../../LBBMLGraphics/GraphicsCommon/LBBMLGraphicsInterface.h"
#include "../../LBBMLGraphics/GraphicsCommon/LBBMLGraphicsStructure.h"
#include "../../LBBMLGraphics/Drawable/TextureDrawable.h"
#include "../../LBBMLGraphics/Drawable/QuadDrawable.h"
#include "../../LBBMLGraphics/Drawable/CubeDrawable.h"
#include "../../LBBMLGraphics/Drawable/ModelDrawable.h"
#include "../../LBBMLGraphics/LightSource/AreaLight.h"
#include "../../LBBMLGraphics/Viewer/Camera.h"
#include "../../LBBMLGraphics/Stats/StatsSet.h"
#include "../../LBBMLGraphics/Viewer/PerspectiveFrustum.h"
#include "../LBBML_GRAPHICS_APP_INTERFACE_EXPORTS.h"
#include "Render/RenderStudio.h"

enum EDepthType
{
	All_Point_Lights = 0,
	Area_Light_Center
};

enum EDisplayMapMode
{
	DISPLAY_DEPTH_MAP = 0,
	DISPLAY_SINGLE_OCCLUSION_MAP,
	DISPLAY_OVERALL_OCCLUSION_MAP,
	DISPLAY_SHADOW_MAP
};

enum EShadowMapType
{
	HARD_SHADOW_MAP =0,
	SOFT_SHADOW_MAP
};

struct SLightMovement
{
	glm::vec2 m_RangeX;
	glm::vec2 m_RangeY;
	glm::vec2 m_RangeZ;
	glm::vec3 m_MoveStep;
};

class LBBML_GRAPHICS_APP_EXPORTS CLBBMLGraphicsSoftShadowMapping
{
public:
	CLBBMLGraphicsSoftShadowMapping(void);
	virtual ~CLBBMLGraphicsSoftShadowMapping(void);

	void initFromConfig(const std::string& vConfig);
	void run(EDisplayMapMode vDisplayMapMode, EShadowMapType vShadowMapType, unsigned int vMapSequenceNumber, unsigned int vNumPointLights);

 	GLuint fetchMapFBO(EDisplayMapMode vDispalyMapMode, unsigned int vMapSequenceNumber) const;
 	unsigned int getLightSourceSize() const {return m_pLightSource->getPointLightSamplesSize();}
 	unsigned int getWindowWidth() const {return m_WindowSize.first;}
 	unsigned int getWindowHeight() const {return m_WindowSize.second;}

private:
	void __initWindow();
	void __initTextureResolution();
	void __initCameras();
	void __initLightSource();
	void __initOpenGL();
	void __initShaderPrograms();
	void __initFBOs();
	void __initScene();
	void __initFrustum();
	void __initStats();
	
	void __updateStats(int vNumPointLights);
	void __updateLightPositions();
	void __collectStats();

	void __startRenderLoop(EDisplayMapMode vDisplayMode, EShadowMapType vShadowMapType, unsigned int vMapSequenceNumber, unsigned int vNumberOfAccumulateOcclusionMap);
	void __renderScene(GLuint& vProgramID);
	void __renderTexture2Screen(GLuint vTextureID);
	bool __isCameraValid();
	void __generateDepthMapsPass(int vNumPointLights);
	void __generateOcclusionMapsPass(int vNumDepthMaps);
	void __accumulateOverallOcclusionMap(unsigned int vNumOcclusionMaps);
	void __generateDepthMapAreaLightCenter();
	void __generateShadowMapPass(EShadowMapType vShadowMapyType);

	void __releaseVariables();

	boost::property_tree::ptree m_ConfigTree;

	std::pair<unsigned int, unsigned int> m_WindowSize;
	std::pair<unsigned int, unsigned int> m_TextureResolution;

	CLBBMLGraphicsPerspectiveFrustum* m_pFrustum;
	CLBBMLGraphicsFrustumBB* m_pFrustrumBB;
	CLBBMLGraphicsCamera* m_pCamera;
	bool m_IsCameraMoving;
	int m_CameraPositionIndex;
	glm::vec3 m_CameraPosition;

	CLBBMLGraphicsAreaLight* m_pLightSource;
	bool m_IsLightMoving;
	SLightMovement m_LightMovement;

	GLuint m_ShadowMapping;
	GLuint m_GenerateDepthMap;
	GLuint m_RenderTexture2Screen;
	GLuint m_GenerateOcclusionMap; 
	GLuint m_AccumulateSoftShadowMap; 
	std::vector<std::pair<GLuint, GLuint>> m_DepthMapsFBO;
	std::vector<std::pair<GLuint, GLuint>> m_OcclusionMapsFBO;
	std::pair<GLuint, GLuint> m_SoftShadowMapFBO;
	std::pair<GLuint, GLuint> m_LightCenterDepthMapFBO;
	GLuint m_TextureArray;

	CLBBMLGraphicsTextureDrawable* m_pTextureDrawable;
	std::vector<CSoftShadowMappingRenderStudio*> m_RenderStudio;

	CLBBMLGraphicsStats*  m_pStats;
	int m_NumPointLights;
	int m_TriangleNumber;
	int m_BatchNumber;
	int m_FrameNumber;
	float m_FrameCostTime;
	float m_FormerTime;
};

//**************************************************************
//Function:extract value from config string
template<class T>
T extractVectorFromString(const std::string& vStr)
{
	std::vector<std::string> SubStr;
	boost::split(SubStr, vStr, boost::is_any_of(","));
	if (SubStr.size() == 2)
	{
		float x = atof(SubStr[0].c_str()), y = atof(SubStr[1].c_str());
		return boost::any_cast<T>(boost::any(glm::vec2(x, y)));
	}
	else if (SubStr.size() == 3)
	{
		float x = atof(SubStr[0].c_str()), y = atof(SubStr[1].c_str()), z = atof(SubStr[2].c_str());
		return boost::any_cast<T>(boost::any(glm::vec3(x, y, z)));
	}
	else if (SubStr.size() == 4)
	{
		float x = atof(SubStr[0].c_str()), y = atof(SubStr[1].c_str()), z = atof(SubStr[2].c_str()), w = atof(SubStr[3].c_str());
		return boost::any_cast<T>(boost::any(glm::vec4(x, y, z, w)));
	}
}