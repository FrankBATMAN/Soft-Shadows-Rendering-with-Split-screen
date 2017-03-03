#include "SoftShadowMapping.h"
#include <time.h>
#include <boost/foreach.hpp>
#include <Soil/include/SOIL.h>

#define ONE_RADIAN 57.2956

CLBBMLGraphicsSoftShadowMapping::CLBBMLGraphicsSoftShadowMapping(void)
{

}

CLBBMLGraphicsSoftShadowMapping::~CLBBMLGraphicsSoftShadowMapping(void)
{
	__releaseVariables();
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::initFromConfig(const std::string& vConfig)
{
	_ASSERT(!vConfig.empty(), "config path can not be empty");
	try
	{
		boost::property_tree::xml_parser::read_xml(vConfig, m_ConfigTree);
	}
	catch (...)
	{
		_ASSERT(false, "can not open file, check file path");
	}

	__initWindow();
	__initTextureResolution();
	__initCameras();
	__initLightSource();
	__initFrustum();
	__initOpenGL();
	__initShaderPrograms();
	__initFBOs();
	__initScene();
	__initStats();
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::run(EDisplayMapMode vDisplayMapMode, EShadowMapType vShadowMapType, unsigned int vMapSequenceNumber, unsigned int vNumPointLights)
{
	__updateStats(vNumPointLights);

	if (__isCameraValid())
	{
		__startRenderLoop(vDisplayMapMode, vShadowMapType, vMapSequenceNumber, vNumPointLights);

		if (m_FrameNumber > 1)
			__collectStats();
	}
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__updateStats(int vNumPointLights)
{
	m_CameraPositionIndex = (m_CameraPositionIndex + 1) % m_pCamera->getCameraPositionsSetSize();
	if(m_IsCameraMoving) m_CameraPosition = m_pCamera->getPositionByIndex(m_CameraPositionIndex);
	LBBMLGraphics::createFrustumBB(m_pFrustrumBB, *m_pFrustum, m_CameraPosition, m_pCamera->getLookAt(), m_pCamera->getUp());

	if(m_IsLightMoving)	__updateLightPositions();

	m_TriangleNumber = 0;
	m_BatchNumber = 0;
	m_NumPointLights = vNumPointLights;
	m_FrameNumber += 1;	
	clock_t CurrentTime = clock();
	m_FrameCostTime = (float)(CurrentTime - m_FormerTime) / CLOCKS_PER_SEC;
	m_FormerTime = CurrentTime;
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__updateLightPositions()
{
	if (m_pLightSource->getAreaLightCenter().x < m_LightMovement.m_RangeX.x || m_pLightSource->getAreaLightCenter().x > m_LightMovement.m_RangeX.y)
		m_LightMovement.m_MoveStep.x = -m_LightMovement.m_MoveStep.x;
	if (m_pLightSource->getAreaLightCenter().y < m_LightMovement.m_RangeY.x || m_pLightSource->getAreaLightCenter().y > m_LightMovement.m_RangeY.y)
		m_LightMovement.m_MoveStep.y = -m_LightMovement.m_MoveStep.y;
	if (m_pLightSource->getAreaLightCenter().z < m_LightMovement.m_RangeZ.x || m_pLightSource->getAreaLightCenter().z > m_LightMovement.m_RangeZ.y)
		m_LightMovement.m_MoveStep.z = -m_LightMovement.m_MoveStep.z;

	m_pLightSource->translate(m_LightMovement.m_MoveStep);
}

//**************************************************************
//Function:
bool CLBBMLGraphicsSoftShadowMapping::__isCameraValid()
{
	for (auto Itr : m_RenderStudio)
	{
		CLBBMLGraphicsOBB* OBB = Itr->getOrientedBoundingBox();
		SOBBHalfLength HalfLen = OBB->getHalfLength();
		float SphereRadius = std::pow((std::pow(HalfLen.X_HalfLength,2.0) + std::pow(HalfLen.Y_HalfLength, 2.0) + std::pow(HalfLen.Z_HalfLength, 2.0)), 0.5);
		if (glm::distance(OBB->getCenter(), m_CameraPosition) < SphereRadius)
			return false;	
	}
	return true;
}

//*********************************************************************************
//FUNCTION:
GLuint CLBBMLGraphicsSoftShadowMapping::fetchMapFBO(EDisplayMapMode vDispalyMapMode, unsigned int vMapSequenceNumber) const
{
	switch (vDispalyMapMode)
	{
	case DISPLAY_DEPTH_MAP:
		return m_DepthMapsFBO[vMapSequenceNumber].second;
		break;
	case DISPLAY_SINGLE_OCCLUSION_MAP:
		return m_OcclusionMapsFBO[vMapSequenceNumber].second;
		break;
	case DISPLAY_OVERALL_OCCLUSION_MAP:
		return m_SoftShadowMapFBO.second;
		break;
	case DISPLAY_SHADOW_MAP:
	default:
		return 0;
		break;
	}
}

//************************************************************** 
//Function:
void CLBBMLGraphicsSoftShadowMapping::__initWindow()
{
	glm::vec2 Window = extractVectorFromString<glm::vec2>(m_ConfigTree.get<std::string>("softShadowMapping.window"));
	m_WindowSize = std::make_pair(Window.x, Window.y);
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__initTextureResolution()
{
	glm::vec2 TextureResolution = extractVectorFromString<glm::vec2>(m_ConfigTree.get<std::string>("softShadowMapping.textureResolution"));
	m_TextureResolution = std::make_pair(TextureResolution.x, TextureResolution.y);
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__initCameras()
{
	std::string IsCameraMoving = m_ConfigTree.get<std::string>("softShadowMapping.camera.isMoving");
	m_IsCameraMoving = IsCameraMoving == "true" ? true : false;

	int PositionSize    = m_ConfigTree.get<int>("softShadowMapping.camera.positionSize");
	glm::vec3 MinCorner = extractVectorFromString<glm::vec3>(m_ConfigTree.get<std::string>("softShadowMapping.camera.positionRange.minCorner"));
	glm::vec3 MaxCorner = extractVectorFromString<glm::vec3>(m_ConfigTree.get<std::string>("softShadowMapping.camera.positionRange.maxCorner"));
	glm::vec3 LookAt    = extractVectorFromString<glm::vec3>(m_ConfigTree.get<std::string>("softShadowMapping.camera.lookAt"));
	glm::vec3 Up        = extractVectorFromString<glm::vec3>(m_ConfigTree.get<std::string>("softShadowMapping.camera.up"));
	
	m_pCamera = new CLBBMLGraphicsCamera;
	m_pCamera->initPositions(PositionSize, MinCorner.x, MaxCorner.x, MinCorner.y, MaxCorner.y, MinCorner.z, MaxCorner.z);
	m_pCamera->setDirection(LookAt);
	m_pCamera->setCameraUp(Up);

	m_CameraPositionIndex = 0;
	m_CameraPosition = glm::vec3(30,30,30);
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__initLightSource()
{
	std::string IsLightMoving = m_ConfigTree.get<std::string>("softShadowMapping.lightSource.isMoving");
	m_IsLightMoving = IsLightMoving == "true" ? true : false;

	int PointLightSize          = m_ConfigTree.get<int>("softShadowMapping.lightSource.sampleSize");
	glm::vec3 AreaLightPosition = extractVectorFromString<glm::vec3>(m_ConfigTree.get<std::string>("softShadowMapping.lightSource.areaLightPosition"));
	glm::vec3 Scale             = extractVectorFromString<glm::vec3>(m_ConfigTree.get<std::string>("softShadowMapping.lightSource.scale"));
	glm::vec3 RotAxis           = extractVectorFromString<glm::vec3>(m_ConfigTree.get<std::string>("softShadowMapping.lightSource.rotAxis"));
	float RotAngle              = m_ConfigTree.get<float>("softShadowMapping.lightSource.rotAngle");

	m_LightMovement.m_RangeX   = extractVectorFromString<glm::vec2>(m_ConfigTree.get<std::string>("softShadowMapping.lightSource.movement.rangeX"));
	m_LightMovement.m_RangeY   = extractVectorFromString<glm::vec2>(m_ConfigTree.get<std::string>("softShadowMapping.lightSource.movement.rangeY"));
	m_LightMovement.m_RangeZ   = extractVectorFromString<glm::vec2>(m_ConfigTree.get<std::string>("softShadowMapping.lightSource.movement.rangeZ"));
	m_LightMovement.m_MoveStep = extractVectorFromString<glm::vec3>(m_ConfigTree.get<std::string>("softShadowMapping.lightSource.movement.stepsize"));

	m_pLightSource = new CLBBMLGraphicsAreaLight;
	m_pLightSource->generatePointLightSamples(glm::vec2(-1,-1), glm::vec2(1,1), PointLightSize);
	m_pLightSource->translate(AreaLightPosition);
	m_pLightSource->scale(Scale);
	m_pLightSource->rotate(RotAxis, RotAngle);
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__initOpenGL()
{
	GLenum Result = glewInit();
	if (Result != GLEW_OK)
	{
		fprintf(stderr, "Error : %s\n", glewGetErrorString(Result));
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D_ARRAY_EXT);
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__initShaderPrograms()
{
	m_ShadowMapping = glCreateProgram();
	LBBMLGraphics::bindShader(m_ShadowMapping, VERTEX_SHADER, "Shaders/shadowMappingVertex.glsl");
	LBBMLGraphics::bindShader(m_ShadowMapping, FRAGMENT_SHADER, "Shaders/shadowMappingFragment.glsl");
	LBBMLGraphics::linkShaderProgram(m_ShadowMapping);

	m_GenerateDepthMap = glCreateProgram();
	LBBMLGraphics::bindShader(m_GenerateDepthMap, VERTEX_SHADER, "Shaders/depthMapVertex.glsl");
	LBBMLGraphics::bindShader(m_GenerateDepthMap, FRAGMENT_SHADER, "Shaders/depthMapFragment.glsl");
	LBBMLGraphics::linkShaderProgram(m_GenerateDepthMap);

	m_RenderTexture2Screen = glCreateProgram();
	LBBMLGraphics::bindShader(m_RenderTexture2Screen, VERTEX_SHADER, "Shaders/renderDepthMapVertex.glsl");
	LBBMLGraphics::bindShader(m_RenderTexture2Screen, FRAGMENT_SHADER, "Shaders/renderDepthMapFragment.glsl");
	LBBMLGraphics::linkShaderProgram(m_RenderTexture2Screen);

	m_GenerateOcclusionMap = glCreateProgram();
	LBBMLGraphics::bindShader(m_GenerateOcclusionMap, VERTEX_SHADER, "Shaders/generateOcclusionMapVertex.glsl");
	LBBMLGraphics::bindShader(m_GenerateOcclusionMap, FRAGMENT_SHADER, "Shaders/generateOcclusionMapFragment.glsl");
	LBBMLGraphics::linkShaderProgram(m_GenerateOcclusionMap);
	
	m_AccumulateSoftShadowMap = glCreateProgram();
	LBBMLGraphics::bindShader(m_AccumulateSoftShadowMap, VERTEX_SHADER, "Shaders/accumulateOcclusionMapVertex.glsl");
	LBBMLGraphics::bindShader(m_AccumulateSoftShadowMap, FRAGMENT_SHADER, "Shaders/accumulateOcclusionMapFragment.glsl");
	LBBMLGraphics::linkShaderProgram(m_AccumulateSoftShadowMap);
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__initFBOs()
{
	int LightsSize = m_pLightSource->getPointLightSamplesSize();

	m_DepthMapsFBO.resize(LightsSize, std::make_pair(0, 0));
	m_OcclusionMapsFBO.resize(LightsSize, std::make_pair(0, 0));
	m_SoftShadowMapFBO       = std::make_pair(0, 0);
	m_LightCenterDepthMapFBO = std::make_pair(0, 0);

	for(unsigned int i = 0; i < m_DepthMapsFBO.size(); ++i)
		LBBMLGraphics::createFBO(m_DepthMapsFBO[i].first, m_DepthMapsFBO[i].second, m_TextureResolution.first, m_TextureResolution.second, DEPTH_FBO);
	for(unsigned int i = 0; i < m_OcclusionMapsFBO.size(); ++i)
		LBBMLGraphics::createFBO(m_OcclusionMapsFBO[i].first, m_OcclusionMapsFBO[i].second, m_TextureResolution.first, m_TextureResolution.second, RED_FBO);
	LBBMLGraphics::createFBO(m_SoftShadowMapFBO.first, m_SoftShadowMapFBO.second, m_TextureResolution.first, m_TextureResolution.second, RGBA_FBO);
	LBBMLGraphics::createFBO(m_LightCenterDepthMapFBO.first, m_LightCenterDepthMapFBO.second, m_TextureResolution.first, m_TextureResolution.second, DEPTH_FBO);

	m_TextureArray = 0;
	glGenTextures(1, &m_TextureArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, m_TextureArray);
	glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D_ARRAY_EXT, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);

	glTexImage3D(GL_TEXTURE_2D_ARRAY_EXT, 0, GL_RED, m_TextureResolution.first, m_TextureResolution.second, LightsSize, 0, GL_RED,  GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, 0);
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__initScene()
{
	m_pTextureDrawable = new CLBBMLGraphicsTextureDrawable;

	boost::property_tree::ptree Scene = m_ConfigTree.get_child("softShadowMapping.scene");
	BOOST_FOREACH(boost::property_tree::ptree::value_type &Attr, Scene)
	{
		if (Attr.first == "drawable")
		{
			if (Attr.second.get<std::string>("type") == "Quad")
			{
				LBBMLGraphicsStructure::STransform QuadTransform;
				QuadTransform.m_Position = extractVectorFromString<glm::vec3>(Attr.second.get<std::string>("position"));
				QuadTransform.m_Scale    = extractVectorFromString<glm::vec3>(Attr.second.get<std::string>("scale"));
				QuadTransform.m_RotAxis  = extractVectorFromString<glm::vec3>(Attr.second.get<std::string>("rotAxis"));
				QuadTransform.m_RotAngle = Attr.second.get<float>("rotAngle");

				CSoftShadowMappingRenderStudio* QuadRenderStudio = new CSoftShadowMappingRenderStudio;
				QuadRenderStudio->setRenderObj(new CLBBMLGraphicsQuadDrawable);
				QuadRenderStudio->setOrientedBoundingBox(new CLBBMLGraphicsOBB);
				QuadRenderStudio->setTransform(QuadTransform);
				
				m_RenderStudio.push_back(QuadRenderStudio);
			}
			else if (Attr.second.get<std::string>("type") == "Cube")
			{
				LBBMLGraphicsStructure::STransform CubeTransform;
				CubeTransform.m_Position = extractVectorFromString<glm::vec3>(Attr.second.get<std::string>("position"));
				CubeTransform.m_Scale    = extractVectorFromString<glm::vec3>(Attr.second.get<std::string>("scale"));
				CubeTransform.m_RotAxis  = extractVectorFromString<glm::vec3>(Attr.second.get<std::string>("rotAxis"));
				CubeTransform.m_RotAngle = Attr.second.get<float>("rotAngle");

				CSoftShadowMappingRenderStudio* CubeRenderStudio = new CSoftShadowMappingRenderStudio;
				CubeRenderStudio->setRenderObj(new CLBBMLGraphicsCubeDrawable);
				CubeRenderStudio->setOrientedBoundingBox(new CLBBMLGraphicsOBB);
				CubeRenderStudio->setTransform(CubeTransform);

				m_RenderStudio.push_back(CubeRenderStudio);
;
			}
			else if (Attr.second.get<std::string>("type") == "Model")
			{
				LBBMLGraphicsStructure::STransform ModelTransform;
				ModelTransform.m_Position = extractVectorFromString<glm::vec3>(Attr.second.get<std::string>("position"));
				ModelTransform.m_Scale    = extractVectorFromString<glm::vec3>(Attr.second.get<std::string>("scale"));
				ModelTransform.m_RotAxis  = extractVectorFromString<glm::vec3>(Attr.second.get<std::string>("rotAxis"));
				ModelTransform.m_RotAngle = Attr.second.get<float>("rotAngle");

				CSoftShadowMappingRenderStudio* ModelRenderStudio = new CSoftShadowMappingRenderStudio;
				ModelRenderStudio->setRenderObj(new CLBBMLGraphicsModelDrawable(Attr.second.get<std::string>("ModelFilePath")));
				ModelRenderStudio->setOrientedBoundingBox(new CLBBMLGraphicsOBB);
				ModelRenderStudio->setTransform(ModelTransform);

				m_RenderStudio.push_back(ModelRenderStudio);
			}
		}
	}
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__initFrustum()
{
	m_pFrustum = new CLBBMLGraphicsPerspectiveFrustum;
	float Fovy = m_ConfigTree.get<float>("softShadowMapping.frustum.fovy");
	glm::vec2 Aspect = extractVectorFromString<glm::vec2>(m_ConfigTree.get<std::string>("softShadowMapping.frustum.aspect"));
	float ZNear = m_ConfigTree.get<float>("softShadowMapping.frustum.nearPlane");
	float ZFar = m_ConfigTree.get<float>("softShadowMapping.frustum.farPlane");
	m_pFrustum->setPerspective(Fovy, (float)Aspect.x / (float)Aspect.y, ZNear, ZFar);

	m_pFrustrumBB = new CLBBMLGraphicsFrustumBB;
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__initStats()
{
	m_pStats = new CLBBMLGraphicsStats;

	m_NumPointLights = 0;
	m_TriangleNumber = 0;
	m_BatchNumber    = 0;
	m_FrameNumber    = 0;
	m_FrameCostTime  = 0.0;
	m_FormerTime     = 0.0;
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__releaseVariables()
{
	delete(m_pFrustum);
	delete(m_pFrustrumBB);
	m_pStats->setStatsFilePath("data.csv");
	delete(m_pStats);

	for (auto Itr : m_RenderStudio)
	{
		delete(Itr);
	}
	m_RenderStudio.clear();

	delete(m_pTextureDrawable);
	delete(m_pCamera);
	delete(m_pLightSource);
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__collectStats()
{
	std::vector<boost::any> Stat;
	Stat.push_back(m_FrameNumber);
	Stat.push_back(glm::vec2(m_TextureResolution.first, m_TextureResolution.second));
	Stat.push_back(m_CameraPosition);
	Stat.push_back(m_pCamera->getLookAt());
	Stat.push_back(m_pCamera->getUp());
	Stat.push_back(m_NumPointLights);
	Stat.push_back(m_TriangleNumber);
	Stat.push_back(m_BatchNumber);
	Stat.push_back(m_FrameCostTime);

	m_pStats->addStat(Stat);
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__startRenderLoop(EDisplayMapMode vDisplayMapMode, EShadowMapType vShadowMapType, unsigned int vTextureMapIndex, unsigned int vNumPointLights)
{
	_ASSERT(vTextureMapIndex >= 0 && vTextureMapIndex < m_pLightSource->getPointLightSamplesSize(), "texture map index out of range");
	_ASSERT(vNumPointLights > 0 && vNumPointLights <= m_pLightSource->getPointLightSamplesSize(), "occlusion map size error");
	_ASSERT(vTextureMapIndex <= vNumPointLights - 1, "texture id must be less than point lights size");

	__generateDepthMapsPass(vNumPointLights);

	if (vDisplayMapMode == DISPLAY_DEPTH_MAP)
	{
		__renderTexture2Screen(m_DepthMapsFBO[vTextureMapIndex].first);
		return;
	}

	__generateOcclusionMapsPass(vNumPointLights);

	if (vDisplayMapMode == DISPLAY_SINGLE_OCCLUSION_MAP)
	{
		__renderTexture2Screen(m_OcclusionMapsFBO[vTextureMapIndex].first);
		return;
	}	

	__accumulateOverallOcclusionMap(vNumPointLights);
	m_TriangleNumber += m_pTextureDrawable->getTriangles();
	m_BatchNumber    += m_pTextureDrawable->getBatches();

	if (vDisplayMapMode == DISPLAY_OVERALL_OCCLUSION_MAP)
	{	
		__renderTexture2Screen(m_SoftShadowMapFBO.first);
		return;
	}

	__generateDepthMapAreaLightCenter();
	__generateShadowMapPass(vShadowMapType);
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__renderScene(GLuint& vProgramID)
{
	unsigned int Flag = 0;
	static float Step = 0.0f;
	for (auto Itr : m_RenderStudio)
	{
		
		if (LBBMLGraphics::isOBBInFrustum(*(Itr->getOrientedBoundingBox()), *m_pFrustrumBB))
		{
// 			glm::mat4 ModelMat = glm::mat4(1.0);
// 			ModelMat = glm::translate(ModelMat, Itr->getTransform().m_Position);
// 			ModelMat = glm::scale(ModelMat, Itr->getTransform().m_Scale);
// 			ModelMat = glm::rotate(ModelMat, Itr->getTransform().m_RotAngle, Itr->getTransform().m_RotAxis);
// 			glUniformMatrix4fv(glGetUniformLocation(vProgramID, "uM"), 1, GL_FALSE, value_ptr(ModelMat));

			glm::mat4 ModelMat = glm::mat4(1.0);
			if (Flag % 3 == 0)
			{
				ModelMat = glm::translate(ModelMat, Itr->getTransform().m_Position);
				ModelMat = glm::scale(ModelMat, Itr->getTransform().m_Scale);
				ModelMat = glm::rotate(ModelMat, Itr->getTransform().m_RotAngle, Itr->getTransform().m_RotAxis);
			}
			else if (Flag % 3 == 1)
			{
				ModelMat = glm::translate(ModelMat, Itr->getTransform().m_Position + glm::vec3(0.0, Step, 0.0));
				ModelMat = glm::scale(ModelMat, Itr->getTransform().m_Scale);
				ModelMat = glm::rotate(ModelMat, Itr->getTransform().m_RotAngle, Itr->getTransform().m_RotAxis);
			}
			else
			{
				ModelMat = glm::translate(ModelMat, Itr->getTransform().m_Position + glm::vec3(Step, 0.0, 0.0));
				ModelMat = glm::scale(ModelMat, Itr->getTransform().m_Scale);
				ModelMat = glm::rotate(ModelMat, Itr->getTransform().m_RotAngle, Itr->getTransform().m_RotAxis);
			}
			glUniformMatrix4fv(glGetUniformLocation(vProgramID, "uM"), 1, GL_FALSE, value_ptr(ModelMat));
			Itr->render();

			m_TriangleNumber += Itr->fetchTriangleNumber();
			m_BatchNumber    += Itr->fetchBatchNumber();

			Step+=0.001;
			Flag++;
		}
	}
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__renderTexture2Screen(GLuint vTextureID)
{
	glUseProgram(m_RenderTexture2Screen);

	glUniform1f(glGetUniformLocation(m_RenderTexture2Screen, "near_plane"),m_pFrustum->getPerspective().ZNear);
	glUniform1f(glGetUniformLocation(m_RenderTexture2Screen, "far_plane"),m_pFrustum->getPerspective().ZFar);
	glUniform1i(glGetUniformLocation(m_RenderTexture2Screen, "shadowMap"), 0);

	glViewport(0,0,m_WindowSize.first,m_WindowSize.second);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, vTextureID);
	m_pTextureDrawable->drawV();

	glUseProgram(0);
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__generateDepthMapsPass(int vNumPointLights)
{
	for (unsigned int i = 0; i < vNumPointLights; ++i)
	{
		glUseProgram(m_GenerateDepthMap);

		LBBMLGraphics::createFrustumBB(m_pFrustrumBB, *m_pFrustum, m_pLightSource->getPointLightByIndex(i), m_pCamera->getLookAt(), m_pCamera->getUp());

		glm::mat4 LightView       = glm::lookAt(m_pLightSource->getPointLightByIndex(i), m_pCamera->getLookAt(), m_pCamera->getUp());
		glm::mat4 LightProjection = glm::perspective<float>(m_pFrustum->getPerspective().Fovy, m_pFrustum->getPerspective().Aspect, m_pFrustum->getPerspective().ZNear, m_pFrustum->getPerspective().ZFar);
		glm::mat4 LightVP         = LightProjection * LightView;
		glUniformMatrix4fv(glGetUniformLocation(m_GenerateDepthMap, "uVP"), 1, GL_FALSE, value_ptr(LightVP));

		glViewport(0,0,m_TextureResolution.first,m_TextureResolution.second);
		glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapsFBO[i].second);
		glClear(GL_DEPTH_BUFFER_BIT);
		__renderScene(m_GenerateDepthMap);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glUseProgram(0);
	}
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__generateDepthMapAreaLightCenter()
{
	LBBMLGraphics::createFrustumBB(m_pFrustrumBB, *m_pFrustum, m_pLightSource->getAreaLightCenter(), m_pCamera->getLookAt(), m_pCamera->getUp());

	glUseProgram(m_GenerateDepthMap);

	glm::mat4 LightCenterView       = glm::lookAt(m_pLightSource->getAreaLightCenter(), m_pCamera->getLookAt(), m_pCamera->getUp());
	glm::mat4 LightCenterProjection = glm::perspective<float>(m_pFrustum->getPerspective().Fovy, m_pFrustum->getPerspective().Aspect, m_pFrustum->getPerspective().ZNear, m_pFrustum->getPerspective().ZFar);
	glm::mat4 LightCenterVP         = LightCenterProjection * LightCenterView;
	glUniformMatrix4fv(glGetUniformLocation(m_GenerateDepthMap, "uVP"), 1, GL_FALSE, value_ptr(LightCenterVP));

	glViewport(0,0,m_TextureResolution.first,m_TextureResolution.second);
	glBindFramebuffer(GL_FRAMEBUFFER, m_LightCenterDepthMapFBO.second);
	glClear(GL_DEPTH_BUFFER_BIT);
	__renderScene(m_GenerateDepthMap);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(0);
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__generateOcclusionMapsPass(int vNumDepthMaps)
{
	LBBMLGraphics::createFrustumBB(m_pFrustrumBB, *m_pFrustum, m_CameraPosition, m_pCamera->getLookAt(), m_pCamera->getUp());

	for (unsigned int i = 0; i < vNumDepthMaps; ++i)
	{
		glUseProgram(m_GenerateOcclusionMap);

		glm::mat4 ViewMat       = glm::lookAt(m_CameraPosition, m_pCamera->getLookAt(), m_pCamera->getUp());
		glm::mat4 ProjectionMat = glm::perspective(m_pFrustum->getPerspective().Fovy, m_pFrustum->getPerspective().Aspect, m_pFrustum->getPerspective().ZNear, m_pFrustum->getPerspective().ZFar);
		glm::mat4 VPMat         = ProjectionMat * ViewMat;
		glUniformMatrix4fv(glGetUniformLocation(m_GenerateOcclusionMap, "uVP"), 1, GL_FALSE, value_ptr(VPMat));

		glm::mat4 LightView     = glm::lookAt(m_pLightSource->getPointLightByIndex(i), m_pCamera->getLookAt(), m_pCamera->getUp());
		glm::mat4 LightSpaceMat = ProjectionMat * LightView;
		glUniformMatrix4fv(glGetUniformLocation(m_GenerateOcclusionMap, "uLightSpaceVP"), 1, GL_FALSE, value_ptr(LightSpaceMat));
		glUniform1i(glGetUniformLocation(m_GenerateOcclusionMap, "uDepthMap"), 0);

		glViewport(0,0,m_TextureResolution.first,m_TextureResolution.second);
		glBindFramebuffer(GL_FRAMEBUFFER, m_OcclusionMapsFBO[i].second);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_DepthMapsFBO[i].first);
		__renderScene(m_GenerateOcclusionMap);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glUseProgram(0);
	}
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__accumulateOverallOcclusionMap(unsigned int vNumOcclusionMaps)
{
	for (unsigned int i = 0; i < vNumOcclusionMaps; i++)
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, m_TextureArray);
		glBindFramebuffer(GL_FRAMEBUFFER, m_OcclusionMapsFBO[i].second);
		glCopyTexSubImage3D(GL_TEXTURE_2D_ARRAY_EXT, 0, 0, 0, i, 0, 0, m_TextureResolution.first, m_TextureResolution.second);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, 0);
	}

	glUseProgram(m_AccumulateSoftShadowMap);

	glUniform1f(glGetUniformLocation(m_AccumulateSoftShadowMap, "uSize"), vNumOcclusionMaps);
	glUniform1i(glGetUniformLocation(m_AccumulateSoftShadowMap, "uSubOcclusionMaps"), 0);

	glViewport(0,0,m_TextureResolution.first,m_TextureResolution.second);
	glBindFramebuffer(GL_FRAMEBUFFER, m_SoftShadowMapFBO.second);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, m_TextureArray);
	m_pTextureDrawable->drawV();
	glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(0);
}

//**************************************************************
//Function:
void CLBBMLGraphicsSoftShadowMapping::__generateShadowMapPass(EShadowMapType vShadowMapType)
{
	glUseProgram(m_ShadowMapping);
	LBBMLGraphics::createFrustumBB(m_pFrustrumBB, *m_pFrustum, m_CameraPosition, m_pCamera->getLookAt(), m_pCamera->getUp());

	glm::mat4 ViewMat		 = glm::lookAt(m_CameraPosition, m_pCamera->getLookAt(), m_pCamera->getUp());
	glm::mat4 ProjectionMat = glm::perspective(m_pFrustum->getPerspective().Fovy, m_pFrustum->getPerspective().Aspect, m_pFrustum->getPerspective().ZNear, m_pFrustum->getPerspective().ZFar);
	glm::mat4 VPMat         = ProjectionMat * ViewMat;
	glUniformMatrix4fv(glGetUniformLocation(m_ShadowMapping, "uVP"), 1, GL_FALSE, value_ptr(VPMat));

	glm::mat4 LightView    = glm::lookAt(m_pLightSource->getAreaLightCenter(), m_pCamera->getLookAt(), m_pCamera->getUp());
	glm::mat4 LightSpaceVP = ProjectionMat * LightView;
	glUniformMatrix4fv(glGetUniformLocation(m_ShadowMapping, "uLightSpaceVP"), 1, GL_FALSE, value_ptr(LightSpaceVP));
	glUniform1f(glGetUniformLocation(m_ShadowMapping, "uWidth"), m_WindowSize.first);
	glUniform1f(glGetUniformLocation(m_ShadowMapping, "uHeight"), m_WindowSize.second);
	glUniform3fv(glGetUniformLocation(m_ShadowMapping, "uLightPosition"), 1, &m_pLightSource->getAreaLightCenter()[0]);
	glUniform3fv(glGetUniformLocation(m_ShadowMapping, "uCameraPosition"), 1, &m_CameraPosition[0]);
	glUniform1i(glGetUniformLocation(m_ShadowMapping, "uShadowMap"), 0);
	glUniform1i(glGetUniformLocation(m_ShadowMapping, "uLightCenterDepthMap"), 1);

	glViewport(0,0,m_WindowSize.first,m_WindowSize.second);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	if (vShadowMapType == HARD_SHADOW_MAP)
	{
		glBindTexture(GL_TEXTURE_2D, m_OcclusionMapsFBO[0].first);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, m_SoftShadowMapFBO.first);
	}
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_LightCenterDepthMapFBO.first);
	__renderScene(m_ShadowMapping);

	glReadBuffer(GL_FRONT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	unsigned char* Data = new unsigned char[m_WindowSize.first * m_WindowSize.second * 3];
	glReadPixels(0, 0, m_WindowSize.first, m_WindowSize.second, GL_RGB, GL_UNSIGNED_BYTE, Data);
	static unsigned int FrameNumber = 0;
	std::string FileName = "./Capture/SotfShaowMapping_";
	FileName = FileName + std::to_string(FrameNumber) + ".bmp";
	SOIL_save_image(FileName.c_str(), SOIL_SAVE_TYPE_BMP, m_WindowSize.first, m_WindowSize.second, 3, Data);
	FrameNumber++;

	glUseProgram(0);
}