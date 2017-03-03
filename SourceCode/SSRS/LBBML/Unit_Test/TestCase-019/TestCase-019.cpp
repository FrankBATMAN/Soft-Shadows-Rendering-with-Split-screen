/*******************************************************
 ** Author : YX
 ** Data : 2016-6-20
 ** Test Purpose: generate light samples by size
*******************************************************/

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <GLM/matrix.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <OpenGL/include/openGL/glew.h>
#include <OpenGL/include/openGL/glfw3.h>
#include <LBBML_ROOT/include/LBBMLGraphics/LightSource/AreaLight.h>
#include <LBBML_ROOT/include/LBBMLGraphics/GraphicsCommon/LBBMLGraphicsInterface.h>

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

GLuint gVBO1, gVBO2, gVBO3;
GLuint gMVPLocation;
GLuint gLightShaderProgram, gCoordinateShaderProgram;

std::vector<glm::vec3> gLightSamples;
CLBBMLGraphicsAreaLight* gGraphicsAreaLight;

//******************************************************************
//FUNCTION:
void renderScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

	static float Angle = 0.0f;
    Angle += 0.01f;

	glm::mat4 WorldMat = glm::mat4(1.0);
	WorldMat = glm::rotate(WorldMat, Angle, glm::vec3(1,0,0));
	glm::mat4 ViewMat = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 0.0),glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 PerspectiveMat = glm::perspective<float>(45.0f, 1024.0 / 768.0, 0.1f, 20.0f);

	glUseProgram(gLightShaderProgram);
	glUniformMatrix4fv(gMVPLocation, 1, GL_TRUE, glm::value_ptr(PerspectiveMat * ViewMat * WorldMat));

	gMVPLocation = glGetUniformLocation(gLightShaderProgram, "gMVP");
    assert(gMVPLocation != 0xFFFFFFFF);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDisableVertexAttribArray(0);

	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glPointSize(2.0);
	glDrawArrays(GL_POINTS, 0, 1000);
	glDisableVertexAttribArray(0);
	glUseProgram(0);

	glUseProgram(gCoordinateShaderProgram);
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	WorldMat = glm::mat4(1.0f);
	glUniformMatrix4fv(gMVPLocation, 1, GL_FALSE, glm::value_ptr(PerspectiveMat * ViewMat * WorldMat));
	gMVPLocation = glGetUniformLocation(gLightShaderProgram, "gMVP");
    assert(gMVPLocation != 0xFFFFFFFF);
	glDrawArrays(GL_LINES, 0, 6);
    glDisableVertexAttribArray(0);
}

//******************************************************************
//FUNCTION:
void initShaderProgram()
{
	gLightShaderProgram = glCreateProgram();
	LBBMLGraphics::bindShader(gLightShaderProgram, VERTEX_SHADER, "LightVert.glsl");
	LBBMLGraphics::bindShader(gLightShaderProgram, FRAGMENT_SHADER, "LightFrag.glsl");
	LBBMLGraphics::linkShaderProgram(gLightShaderProgram);

	gCoordinateShaderProgram = glCreateProgram();
	LBBMLGraphics::bindShader(gCoordinateShaderProgram, VERTEX_SHADER, "CoordinateVert.glsl");
	LBBMLGraphics::bindShader(gCoordinateShaderProgram, FRAGMENT_SHADER, "CoordinateFrag.glsl");
	LBBMLGraphics::linkShaderProgram(gCoordinateShaderProgram);
}

//******************************************************************
//FUNCTION:
std::vector<glm::vec3> generateTestData(glm::vec3 vLeftBottomPositon, glm::vec3 vRightTopPositon)
{
	int Size = 1000;
	gGraphicsAreaLight = new CLBBMLGraphicsAreaLight; 
	gGraphicsAreaLight->generatePointLightSamples(glm::vec2(vLeftBottomPositon.x, vLeftBottomPositon.y), glm::vec2(vRightTopPositon.x, vRightTopPositon.y), Size);
	std::vector<glm::vec3> LightSamples;
	for (int i = 0; i < Size; i++)
	{
		LightSamples.push_back(gGraphicsAreaLight->getPointLightByIndex(i));
	}

	return LightSamples;   
}

//******************************************************************
//FUNCTION:
void createVertexBuffer()
{
	GLfloat Vertice[][3] =
	{
		-0.6f, -0.5f, 0.0f,
		0.6f, -0.5f, 0.0f,
		0.6f, 0.5f, 0.0f,
		-0.6f, 0.5f, 0.0f
	};
 	glGenBuffers(1, &gVBO1);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice), Vertice, GL_STATIC_DRAW);

	gLightSamples = generateTestData(glm::vec3(Vertice[0][0],Vertice[0][1],Vertice[0][2]) , glm::vec3(Vertice[2][0],Vertice[2][1],Vertice[2][2]));

 	glGenBuffers(1, &gVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO2);
	glBufferData(GL_ARRAY_BUFFER, gLightSamples.size() * 12, &gLightSamples[0], GL_STATIC_DRAW);

	glm::vec3 Coordinate[6];
    Coordinate[0] = glm::vec3(0.0f, 0.0f, 0.0f);
    Coordinate[1] = glm::vec3(0.5f, 0.0f, 0.0f);
    Coordinate[2] = glm::vec3(0.0f, 0.0f, 0.0f);
    Coordinate[3] = glm::vec3(0.0f, 0.5f, 0.0f);
	Coordinate[4] = glm::vec3(0.0f, 0.0f, 0.0f);
    Coordinate[5] = glm::vec3(0.0f, 0.0f, 0.5f);
	glGenBuffers(1, &gVBO3);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Coordinate), Coordinate, GL_STATIC_DRAW);
}

//******************************************************
//Function:
void startRenderLoop()
{
	if(!glfwInit()) return;
	GLFWwindow* pWindow = glfwCreateWindow(1024, 768, "Generate Light Samples", nullptr, nullptr);
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

	createVertexBuffer();
	initShaderProgram();

	while(!glfwWindowShouldClose(pWindow))
	{
		renderScene();
		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}
	glfwTerminate();
}

//******************************************************
//Function:
void releaseVariables()
{
	delete(gGraphicsAreaLight);
}

int main(void)
{
	installMemoryLeakDetector();

	startRenderLoop();
	releaseVariables();

	system("pause");
	return 0;
}