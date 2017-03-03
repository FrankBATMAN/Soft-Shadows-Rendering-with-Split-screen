#include "ShaderBasedSplitScreenRendering.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "../../LBBML/Common/LBBMLInterface.h"
#include "../../LBBMLGraphics/GraphicsCommon/LBBMLGraphicsInterface.h"

CLBBMLGraphicsShaderBasedSplitScreenRendering::CLBBMLGraphicsShaderBasedSplitScreenRendering()
{
	__initialize();
}

CLBBMLGraphicsShaderBasedSplitScreenRendering::~CLBBMLGraphicsShaderBasedSplitScreenRendering()
{
	__finalize();
}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsShaderBasedSplitScreenRendering::initFromConfig(const std::string& vConfigFilePath)
{
	_ASSERT(!vConfigFilePath.empty(), "config path can not be empty");
	try
	{
		boost::property_tree::xml_parser::read_xml(vConfigFilePath, m_ConfigTree);
	}
	catch (...)
	{
		_ASSERT(false, "can not open file, check file path");
	}

	__initializeWindow();
	__initializeCamera();
	__initializeFrustum();
	__initializeScene();
	__splitRectangle();
}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsShaderBasedSplitScreenRendering::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (unsigned int i = 0; i < m_SubViewPorts.size(); i++)
	{
		glUseProgram(m_ShaderProgramID);

		glm::mat4 CameraMat     = glm::lookAt(m_pCamera->getPositionByIndex(0), m_pCamera->getLookAt(), m_pCamera->getUp());
		glm::mat4 ProjectionMat = glm::perspective(m_Frustum.Fovy, m_Frustum.Aspect, m_Frustum.ZNear, m_Frustum.ZFar);

		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID,"uViewMat"), 1, GL_FALSE, value_ptr(CameraMat));
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID,"uProjectionMat"), 1, GL_FALSE, value_ptr(ProjectionMat));
		glUniform4fv(glGetUniformLocation(m_ShaderProgramID,"uSubViewports"), 1, value_ptr(m_SubViewPorts[i]));
		glm::vec2 ScreenResolution = glm::vec2(m_Window.x, m_Window.y);
		glUniform2fv(glGetUniformLocation(m_ShaderProgramID,"uScreenResolution"), 1, value_ptr(ScreenResolution));

		__renderScene();

		glUseProgram(0);
	}

	glLineWidth(5.0f);
	glColor3f(0.0f, 0.0f, 0.0f);	
	for (int i = 0; i < m_SubViewPorts.size(); ++i)
	{
		glBegin(GL_LINE_LOOP);
		glm::vec4 SubWindowPosition = m_SubViewPorts[i];
		SubWindowPosition.x = (SubWindowPosition.x - m_Window.x/2.0f) / (m_Window.x/2.0f);
		SubWindowPosition.y = (SubWindowPosition.y - m_Window.y/2.0f) / (m_Window.y/2.0f);
		SubWindowPosition.z = (SubWindowPosition.z - m_Window.x/2.0f) / (m_Window.x/2.0f);
		SubWindowPosition.w = (SubWindowPosition.w - m_Window.y/2.0f) / (m_Window.y/2.0f);
		glVertex3f(SubWindowPosition.x, SubWindowPosition.y, 0.0f);
		glVertex3f(SubWindowPosition.z, SubWindowPosition.y, 0.0f);
		glVertex3f(SubWindowPosition.z, SubWindowPosition.w, 0.0f);
		glVertex3f(SubWindowPosition.x, SubWindowPosition.w, 0.0f);
		glEnd();
	}

}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsShaderBasedSplitScreenRendering::__initialize()
{
	__initializeGLEW();
	__initializeShader();
}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsShaderBasedSplitScreenRendering::__initializeGLEW()
{
	GLenum res = glewInit();

	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error : %s\n", glewGetErrorString(res));
	}
}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsShaderBasedSplitScreenRendering::__initializeShader()
{
	m_ShaderProgramID = glCreateProgram();
	LBBMLGraphics::bindShader(m_ShaderProgramID, VERTEX_SHADER, "SplitScreenVert.glsl");
	LBBMLGraphics::bindShader(m_ShaderProgramID, FRAGMENT_SHADER, "SplitScreenFrag.glsl");
	LBBMLGraphics::linkShaderProgram(m_ShaderProgramID);
}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsShaderBasedSplitScreenRendering::__initializeWindow()
{
// 	glm::vec2 Window = extractVectorFromString<glm::vec2>(m_ConfigTree.get<std::string>("shaderBasedSplitRendering.window"));
// 	m_pWindow = new CLBBMLGraphicsWindow;
// 	m_pWindow->initWindow(Window.x, Window.y);
	m_Window = extractVectorFromString<glm::vec2>(m_ConfigTree.get<std::string>("shaderBasedSplitRendering.window"));
}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsShaderBasedSplitScreenRendering::__initializeCamera()
{
	int PositionSize    = m_ConfigTree.get<int>("shaderBasedSplitRendering.camera.positionSize");
	glm::vec3 MinCorner = extractVectorFromString<glm::vec3>(m_ConfigTree.get<std::string>("shaderBasedSplitRendering.camera.positionRange.minCorner"));
	glm::vec3 MaxCorner = extractVectorFromString<glm::vec3>(m_ConfigTree.get<std::string>("shaderBasedSplitRendering.camera.positionRange.maxCorner"));
	glm::vec3 LookAt    = extractVectorFromString<glm::vec3>(m_ConfigTree.get<std::string>("shaderBasedSplitRendering.camera.lookAt"));
	glm::vec3 Up        = extractVectorFromString<glm::vec3>(m_ConfigTree.get<std::string>("shaderBasedSplitRendering.camera.up"));

	m_pCamera = new CLBBMLGraphicsCamera;
	m_pCamera->initPositions(PositionSize, MinCorner.x, MaxCorner.x, MinCorner.y, MaxCorner.y, MinCorner.z, MaxCorner.z);
	m_pCamera->setDirection(LookAt);
	m_pCamera->setCameraUp(Up);
}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsShaderBasedSplitScreenRendering::__initializeFrustum()
{
	glm::vec2 Aspect = extractVectorFromString<glm::vec2>(m_ConfigTree.get<std::string>("shaderBasedSplitRendering.frustum.aspect"));

	m_Frustum.Fovy   = m_ConfigTree.get<float>("shaderBasedSplitRendering.frustum.fovy");
	m_Frustum.Aspect = (float)Aspect.x / (float)Aspect.y;
	m_Frustum.ZNear  = m_ConfigTree.get<float>("shaderBasedSplitRendering.frustum.nearPlane");
	m_Frustum.ZFar   = m_ConfigTree.get<float>("shaderBasedSplitRendering.frustum.farPlane");
}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsShaderBasedSplitScreenRendering::__initializeScene()
{
	boost::property_tree::ptree Scene = m_ConfigTree.get_child("shaderBasedSplitRendering.scene");
	BOOST_FOREACH(boost::property_tree::ptree::value_type &Attr, Scene)
	{
		if (Attr.first == "drawable")
		{
			CLBBMLGraphicsModelDrawable* Model = new CLBBMLGraphicsModelDrawable(Attr.second.get<std::string>("ModelFilePath"));
			LBBMLGraphicsStructure::STransform ModelTransform;
			ModelTransform.m_Position = extractVectorFromString<glm::vec3>(Attr.second.get<std::string>("position"));
			ModelTransform.m_Scale    = extractVectorFromString<glm::vec3>(Attr.second.get<std::string>("scale"));
			ModelTransform.m_RotAxis  = extractVectorFromString<glm::vec3>(Attr.second.get<std::string>("rotAxis"));
			ModelTransform.m_RotAngle = Attr.second.get<float>("rotAngle");
			m_Models.push_back(std::make_pair(Model, ModelTransform));
		}
	}
}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsShaderBasedSplitScreenRendering::__splitRectangle()
{
	if (m_ConfigTree.get<bool>("shaderBasedSplitRendering.splitWindow.isDivideEqually"))
	{
		m_SplitedRatio = extractVectorFromString<glm::vec2>(m_ConfigTree.get<std::string>("shaderBasedSplitRendering.splitWindow.splitedRatio"));

		m_SubViewPorts = LBBML::splitRectangle(m_SplitedRatio.x, m_SplitedRatio.y, m_Window.x, m_Window.y);
	}
	else
	{
		int WindowNum = m_ConfigTree.get<int>("shaderBasedSplitRendering.splitWindow.subWindows.subWindowNum");
		for(int i = 0; i < WindowNum; ++i)
 		{
			m_SubViewPorts.push_back(extractVectorFromString<glm::vec4>(m_ConfigTree.get<std::string>("shaderBasedSplitRendering.splitWindow.subWindows.subWindow" + boost::lexical_cast<std::string>(i))));
 		}
 	}
}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsShaderBasedSplitScreenRendering::__renderScene()
{
	if (m_Models.size() > 0)
	{	
		for (auto Itr : m_Models)
		{
			glm::mat4 ModelMat = glm::mat4(1.0);
			ModelMat = glm::translate(ModelMat, Itr.second.m_Position);
			ModelMat = glm::scale(ModelMat, Itr.second.m_Scale);
			ModelMat = glm::rotate(ModelMat, Itr.second.m_RotAngle, Itr.second.m_RotAxis);

			glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, "uM"), 1, GL_FALSE, value_ptr(ModelMat));

			(*Itr.first).drawV();
		}
	}
}

//*********************************************************************************
//FUNCTION:
void CLBBMLGraphicsShaderBasedSplitScreenRendering::__finalize()
{
//	delete(m_pWindow);
	delete(m_pCamera);
	
	std::vector<std::pair<CLBBMLGraphicsModelDrawable*, LBBMLGraphicsStructure::STransform>>::iterator ModelItr;
	for (ModelItr = m_Models.begin(); ModelItr != m_Models.end(); ++ModelItr)
		delete(ModelItr->first);
	m_Models.clear();
}