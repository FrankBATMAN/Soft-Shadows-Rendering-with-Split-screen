/*******************************************************
 ** Author : YX
 ** Data : 2016-6-15
 ** Test Purpose: test different camera positions
*******************************************************/

#include <memory.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <LBBML_ROOT/include/LBBMLGraphics/Viewer/Camera.h>
#include <LBBML_ROOT/include/LBBMLGraphics/Drawable/CubeDrawable.h>
#include <LBBML_ROOT/include/LBBMLGraphics/GraphicsCommon/LBBMLGraphicsInterface.h>
#include <OpenGL/include/openGL/glfw3.h>

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

GLuint gVBO;
GLuint gProgram, gCoordinateProgram;
GLuint gPositionIndex, gPositionSize;

CLBBMLGraphicsCamera* gCamera;
CLBBMLGraphicsCubeDrawable* gCube;

//******************************************************************
//FUNCTION:
void initShaderProgram()
{
	gProgram = glCreateProgram();
	LBBMLGraphics::bindShader(gProgram, VERTEX_SHADER, "vert.glsl");
	LBBMLGraphics::bindShader(gProgram, FRAGMENT_SHADER, "frag.glsl");
	LBBMLGraphics::linkShaderProgram(gProgram);

	gCoordinateProgram = glCreateProgram();
	LBBMLGraphics::bindShader(gCoordinateProgram, VERTEX_SHADER, "CoordinateVert.glsl");
	LBBMLGraphics::bindShader(gCoordinateProgram, FRAGMENT_SHADER, "CoordinateFrag.glsl");
	LBBMLGraphics::linkShaderProgram(gCoordinateProgram);
}

//******************************************************************
//FUNCTION:
void initVariables()
{
	gPositionIndex = 0; 
	gPositionSize  = 100;
	gCamera = new CLBBMLGraphicsCamera;
	gCamera->initPositions(gPositionSize, -20, 20, 5, 20, -20, 20);
	gCube   = new CLBBMLGraphicsCubeDrawable;
}

//******************************************************************
//FUNCTION:
void releaseVariables()
{
	delete(gCamera);
	delete(gCube);
}

//******************************************************************
//FUNCTION:
void render()
{
	glClearColor(0.4, 0.6, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,1024,768);

	glUseProgram(gProgram);

	glm::mat4 ViewMat = glm::lookAt(gCamera->getPositionByIndex(gPositionIndex), glm::vec3(0,0,0), glm::vec3(0,1,0));
	glm::mat4 ProjMat = glm::perspective<float>(45.0f, 1024.0/768.0, 0.1f, 50.0f);
	glUniformMatrix4fv(glGetUniformLocation(gProgram, "uVP"), 1, GL_FALSE, value_ptr(ProjMat * ViewMat));

	gPositionIndex = gPositionIndex >= gCamera->getCameraPositionsSetSize() ? 0 : ++gPositionIndex;

	STransform Transform;
	Transform.m_Position = glm::vec3(0,2,2);
	Transform.m_Scale = glm::vec3(1,1,1);
	Transform.m_RotAxis = glm::vec3(1,1,1);
	Transform.m_RotAngle = 0.0;
	gCube->drawV(gProgram, Transform);

	glUseProgram(0);

	glUseProgram(gCoordinateProgram);
	glUniformMatrix4fv(glGetUniformLocation(gCoordinateProgram, "uVP"), 1, GL_FALSE, value_ptr(ProjMat * ViewMat));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, 6);

	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

//******************************************************************
//FUNCTION:
static void createVertexBuffer()
{
	glm::vec3 Coordinate[6];
	Coordinate[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	Coordinate[1] = glm::vec3(5.0f, 0.0f, 0.0f);
	Coordinate[2] = glm::vec3(0.0f, 0.0f, 0.0f);
	Coordinate[3] = glm::vec3(0.0f, 5.0f, 0.0f);
	Coordinate[4] = glm::vec3(0.0f, 0.0f, 0.0f);
	Coordinate[5] = glm::vec3(0.0f, 0.0f, 5.0f);
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Coordinate), Coordinate, GL_STATIC_DRAW);
}

void startRenderLoop()
{
	if (!glfwInit()) return;

	GLFWwindow* pWindow = glfwCreateWindow(1024, 768, "Camera Test", nullptr, nullptr);
	if (!pWindow)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(pWindow);
	glfwSwapInterval(30);

	glewInit();
	createVertexBuffer();
	initShaderProgram();
	initVariables();

	while(!glfwWindowShouldClose(pWindow))
	{
		render();

		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}
	glfwTerminate();
}

int main()
{
	installMemoryLeakDetector();

	startRenderLoop();
	releaseVariables();

	system("pause");
	return 0;
}