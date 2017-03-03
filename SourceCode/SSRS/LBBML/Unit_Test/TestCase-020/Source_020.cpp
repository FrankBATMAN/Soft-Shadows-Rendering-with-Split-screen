/*******************************************************
 ** Author : YX
 ** Data : 2016-6-23
 ** Test Purpose: Test GL_R8 Texture	
*******************************************************/

#include <memory>
#include <string>
#include <iostream>
#include <OpenGL/include/openGL/glew.h>
#include <OpenGL/include/openGL/glfw3.h>
#include <LBBML_ROOT/include/LBBMLGraphics/GraphicsCommon/LBBMLGraphicsInterface.h>
#include <LBBML_ROOT/include/LBBMLGraphics/Drawable/CubeDrawable.h>
#include <LBBML_ROOT/include/LBBMLGraphics/Drawable/TextureDrawable.h>

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
	//_crtBreakAlloc = 30337;
#endif
}

GLuint gProgram = 0, gTexture = 0;
GLuint gVAO = 0, gVBO = 0;
CLBBMLGraphicsCubeDrawable* gCube;
CLBBMLGraphicsTextureDrawable* gQuad;

//******************************************************
//Function:
void initShaderProgram()
{
	gProgram = glCreateProgram();
	LBBMLGraphics::bindShader(gProgram, VERTEX_SHADER, "vert.glsl");
	LBBMLGraphics::bindShader(gProgram, FRAGMENT_SHADER, "frag.glsl");
	LBBMLGraphics::linkShaderProgram(gProgram);

	gTexture = glCreateProgram();
	LBBMLGraphics::bindShader(gTexture, VERTEX_SHADER, "vertTexture.glsl");
	LBBMLGraphics::bindShader(gTexture, FRAGMENT_SHADER, "fragTexture.glsl");
	LBBMLGraphics::linkShaderProgram(gTexture);
}

//******************************************************
//Function:
void render()
{
	glClearColor(0.4, 0.6, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,1024,768);

	glUseProgram(gProgram);

	glm::mat4 P = glm::perspective<float>(45.0, 1024.0/768.0, 0.1, 100.0);
	glm::mat4 V = glm::lookAt(glm::vec3(30,30,30), glm::vec3(0,0,0), glm::vec3(0,1,0));
	glm::mat4 VP = P * V;
	glUniformMatrix4fv(glGetUniformLocation(gProgram, "uVP"), 1, GL_FALSE, value_ptr(VP));

	static float Angle = 0.0;
	Angle += 0.02;
	STransform Transform;
	Transform.m_Position = glm::vec3(0,0,0);
	Transform.m_Scale    = glm::vec3(3,3,3);
	Transform.m_RotAxis  = glm::vec3(0,1,0);
	Transform.m_RotAngle = Angle;

	glBindFramebuffer(GL_FRAMEBUFFER, gVBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gCube->drawV(gProgram, Transform);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(0);

	glUseProgram(gTexture);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE0, gVAO);
	gQuad->draw();
	glUseProgram(0);
}

//******************************************************
//Function:
void init()
{
	initShaderProgram();
	LBBMLGraphics::createFBO(gVAO, gVBO, 1024, 768, RED_FBO);
	gCube = new CLBBMLGraphicsCubeDrawable;
	gQuad = new CLBBMLGraphicsTextureDrawable;
}

//******************************************************
//Function:
void startRenderLoop()
{
	if(!glfwInit()) return;
	GLFWwindow* pWindow = glfwCreateWindow(1024, 768, "Test GL_R8 Texture", nullptr, nullptr);
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

	init();

	while(!glfwWindowShouldClose(pWindow))
	{
		render();
		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}
	glfwTerminate();
}

//***************************************************
//Function
void releaseVariables()
{
	delete(gQuad);
	delete(gCube);
}

int main(void)
{
	installMemoryLeakDetector();

	startRenderLoop();
	releaseVariables();

	system("pause");
	return 0;
}