/*******************************************************
 ** Author : YX
 ** Data : 2016-6-8
 ** Test Purpose: test create OBB and culling: NSFC-39
*******************************************************/

#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <OpenGL/include/openGL/glew.h>
#include <OpenGL/include/openGL/glfw3.h>
#include <LBBML_ROOT/include/LBBMLGraphics/GraphicsCommon/LBBMLGraphicsInterface.h>
#include <LBBML_ROOT/include/LBBMLGraphics/Drawable/CubeDrawable.h>
#include <LBBML_ROOT/include/LBBMLGraphics/Viewer/PerspectiveFrustum.h>

//FUNCTION: detect the memory leak in DEBUG mode
void installMemoryLeakDetector()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	//_CRTDBG_LEAK_CHECK_DF: Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks and generate an error 
	//report if the application failed to free all the memory it allocated. OFF: Do not automatically perform leak checking at program exit.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//the following statement is used to trigger a breakpoint when memory leak happens
	//comment it out if there is no memory leak report;
	//_crtBreakAlloc = 289;
#endif
}

GLuint gVAO, gVBO;
GLuint gProgram;
CLBBMLGraphicsCubeDrawable* gCube;
CLBBMLGraphicsPerspectiveFrustum* gFrustum;

//*********************************************************************************
//FUNCTION:
void initShaderProgram()
{
	gProgram = glCreateProgram();
	LBBMLGraphics::bindShader(gProgram, VERTEX_SHADER, "vert.glsl");
	LBBMLGraphics::bindShader(gProgram, FRAGMENT_SHADER, "frag.glsl");
	LBBMLGraphics::linkShaderProgram(gProgram);
}

//*********************************************************************************
//FUNCTION:
void render()
{
	glClearColor(0.4, 0.6, 0.8, 1.0);
	
	glUseProgram(gProgram);

	glm::mat4 ViewMat = glm::lookAt(glm::vec3(30, 30, 30), glm::vec3(0,0,0), glm::vec3(0,1,0));
	glm::mat4 ProMat  = glm::perspective<float>(45.0f, 1024.0/768.0, 0.1f, 200.0f);

	glUniformMatrix4fv(glGetUniformLocation(gProgram, "uVP"), 1, GL_FALSE, value_ptr(ProMat * ViewMat));
	
	glViewport(0,0,1024,768);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static float Pos = 2.0;
	Pos += 0.02;

	STransform Transform;
	Transform.m_Position = glm::vec3(0, Pos, 0);
	Transform.m_Scale    = glm::vec3(3, 3, 3);
	Transform.m_RotAxis  = glm::vec3(1, 0, 1);
	Transform.m_RotAngle = 0;

	if(gCube->isVisible(gFrustum->getFrustumBBPlanes()))
	{
		gCube->drawV(gProgram, Transform);
	}
	else
	{
		std::cout << gCube->getOBBCenter().y <<  "	invisible !" << std::endl;
		std::cout << "batches:	" <<  gCube->getBatchesV() << std::endl;
		std::cout << "triangles:	" <<  gCube->getTrianglesV() << std::endl;
	}

	glUseProgram(0);
}

//*********************************************************************************
//FUNCTION:
void initializeVariables()
{
	glEnable(GL_DEPTH_TEST);

	gVAO     = 0;
	gVBO     = 0;
	gProgram = 0;

	gFrustum = new CLBBMLGraphicsPerspectiveFrustum;
	gFrustum->setPerspective(45.0, 1024.0/768.0, 0.1, 200.0f);
	gFrustum->initFrustumSBB(glm::vec3(30,30,30), glm::vec3(0,0,0), glm::vec3(0,1,0));
}

//*********************************************************************************
//FUNCTION:
void releaseVariables()
{
	delete(gFrustum);
	delete(gCube);
}

//************************************************************
//Function:
void startRenderLoop()
{
	if(!glfwInit()) return;

	GLFWwindow* pWindow = glfwCreateWindow(1024, 768, "OBB Test", nullptr, nullptr);
	if(!pWindow)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(pWindow);

	GLenum Ret = glewInit();
	if(Ret != GLEW_OK)
	{
		fprintf(stderr, "Error : %s\n", glewGetErrorString(Ret));
	}

	gCube = new CLBBMLGraphicsCubeDrawable;
	initShaderProgram();
	
	while(!glfwWindowShouldClose(pWindow))
	{
		render();
		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}
	glfwTerminate();
}

int main(int argc, char** argv)
{
	installMemoryLeakDetector();

	initializeVariables();
	startRenderLoop();
	releaseVariables();

	system("pause");
	return 0;
}