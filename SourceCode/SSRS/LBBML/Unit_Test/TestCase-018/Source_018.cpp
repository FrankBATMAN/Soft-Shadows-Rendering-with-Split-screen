/*******************************************************
 ** Author : YX
 ** Data : 2016-6-19
 ** Test Purpose: model load using Assimp lib	
*******************************************************/

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <OpenGL/include/openGL/glew.h>
#include <OpenGL/include/openGL/glfw3.h>
#include <LBBML_ROOT/include/LBBMLGraphics/GraphicsCommon/LBBMLGraphicsInterface.h>
#include <LBBML_ROOT/include/LBBMLGraphics/GraphicsCommon/LBBMLGraphicsStructure.h>
#include <LBBML_ROOT/include/LBBMLGraphics/Drawable/ModelDrawable.h>

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
	//_crtBreakAlloc = 350;
#endif
}

GLuint gProgram = 0;
CLBBMLGraphicsModelDrawable* gModel;
CLBBMLGraphicsFrustumBB* gFrustumBB;
CLBBMLGraphicsOBB* gOBB;

//************************************************************
//Function:
void initShaderProgram()
{
	gProgram = glCreateProgram();
	LBBMLGraphics::bindShader(gProgram, VERTEX_SHADER, "vert.glsl");
	LBBMLGraphics::bindShader(gProgram, FRAGMENT_SHADER, "frag.glsl");
	LBBMLGraphics::linkShaderProgram(gProgram);
}

//************************************************************
//Function:
void initVariables()
{
	initShaderProgram();

	CLBBMLGraphicsPerspectiveFrustum* pFrustum = new CLBBMLGraphicsPerspectiveFrustum;
	pFrustum->setPerspective(45.0, 1024.0/768.0, 0.1, 100.0);
	gFrustumBB = new CLBBMLGraphicsFrustumBB;
	LBBMLGraphics::createFrustumBB(gFrustumBB, *pFrustum, glm::vec3(30,30,30), glm::vec3(0,0,0), glm::vec3(0,1,0));
	delete(pFrustum);

	gOBB = new CLBBMLGraphicsOBB;
	gModel = new CLBBMLGraphicsModelDrawable("../Models/bunny.obj");
	gModel->createOBB(gOBB);
}

//************************************************************
//Function:
void releaseVariables()
{
	delete(gModel);
	delete(gFrustumBB);
	delete(gOBB);
}

//************************************************************
//Function:
void render()
{
	glClearColor(0.4, 0.6, 0.8, 1.0);

	glUseProgram(gProgram);

	glm::mat4 P = glm::perspective<float>(45.0, 1024.0/768.0, 0.1, 100.0);
	glm::mat4 V = glm::lookAt(glm::vec3(30,30,30), glm::vec3(0,0,0), glm::vec3(0,1,0));
	glUniformMatrix4fv(glGetUniformLocation(gProgram, "uVP"), 1, GL_FALSE, value_ptr(P * V));

	static float Angle = 0.0;
	Angle += 0.01;
	LBBMLGraphicsStructure::STransform Transform;
	Transform.m_Position = glm::vec3(0,Angle,0);
	Transform.m_Scale    = glm::vec3(0.5,0.5,0.5);
	Transform.m_RotAxis  = glm::vec3(-1,1,-1);
	Transform.m_RotAngle = Angle;

	glViewport(0,0,1024,768);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (LBBMLGraphics::isOBBInFrustum(*gOBB, *gFrustumBB))
	{
		glm::mat4 ModelMat = glm::mat4(1.0);
		ModelMat = glm::translate(ModelMat, Transform.m_Position);
		ModelMat = glm::scale(ModelMat, Transform.m_Scale);
		ModelMat = glm::rotate(ModelMat, Transform.m_RotAngle, Transform.m_RotAxis);
		glUniformMatrix4fv(glGetUniformLocation(gProgram, "uM"), 1, GL_FALSE, value_ptr(ModelMat));

		gModel->drawV();

		std::cout << "batches:	" <<  gModel->getBatchesV() << std::endl;
		std::cout << "triangles:	" <<  gModel->getTrianglesV() << std::endl;
	}
	else
	{
		std::cout << "batches:	" <<  gModel->getBatchesV() << std::endl;
		std::cout << "triangles:	" <<  gModel->getTrianglesV() << std::endl;
		system("pause");
	}

	glUseProgram(0);
}

//************************************************************
//Function:
void startRenderLoop()
{
	if(!glfwInit()) return;
	GLFWwindow* pWindow = glfwCreateWindow(1024, 768, "Test Model Load", nullptr, nullptr);
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

	initVariables();

	while(!glfwWindowShouldClose(pWindow))
	{
		render();
		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}
	glfwTerminate();
}

int main(void)
{
	installMemoryLeakDetector();

	startRenderLoop();
	releaseVariables();

	system("pause");
	return 0;
}