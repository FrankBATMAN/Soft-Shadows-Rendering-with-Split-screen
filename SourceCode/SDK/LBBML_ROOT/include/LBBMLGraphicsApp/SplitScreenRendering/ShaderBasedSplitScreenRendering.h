#pragma once
#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

#include "../LBBML_GRAPHICS_APP_INTERFACE_EXPORTS.h"
/*#include "../../LBBMLGraphics/Viewer/Window.h"*/
#include "../../LBBMLGraphics/Viewer/Camera.h"
#include "../../LBBMLGraphics/Viewer/PerspectiveFrustum.h"
#include "../../LBBMLGraphics/Drawable/ModelDrawable.h"
#include "../../LBBMLGraphics/GraphicsCommon/LBBMLGraphicsStructure.h"

class LBBML_GRAPHICS_APP_EXPORTS CLBBMLGraphicsShaderBasedSplitScreenRendering
{
public:
	CLBBMLGraphicsShaderBasedSplitScreenRendering();
	~CLBBMLGraphicsShaderBasedSplitScreenRendering();

	void initFromConfig(const std::string& vConfigFilePath);
	void renderScene();

private:
	void __initialize();
	void __initializeGLEW();
	void __initializeShader();
	void __initializeWindow();
	void __initializeCamera();
	void __initializeFrustum();
	void __initializeScene();
	void __splitRectangle();
	void __renderScene();
	void __finalize();

//	CLBBMLGraphicsWindow*        m_pWindow;
	CLBBMLGraphicsCamera*        m_pCamera;

	SPerspectiveFrustum m_Frustum;

	std::vector<std::pair<CLBBMLGraphicsModelDrawable*, LBBMLGraphicsStructure::STransform>> m_Models;
	std::vector<glm::vec4> m_SubViewPorts;
	glm::vec2 m_SplitedRatio;
	glm::vec2 m_Window;

	GLuint m_ShaderProgramID;

	boost::property_tree::ptree m_ConfigTree;
	
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