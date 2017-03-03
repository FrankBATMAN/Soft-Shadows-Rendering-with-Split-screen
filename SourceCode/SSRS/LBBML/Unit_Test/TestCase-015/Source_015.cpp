/*******************************************************
 ** Author : YX
 ** Data : 2016-6-?
 ** Test Purpose: test collision avoidance using AABB
*******************************************************/

#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <OpenGL/include/openGL/glew.h>
#include <OpenGL/include/openGL/glfw3.h>
#include <LBBML_ROOT/include/LBBMLGraphics/GraphicsCommon/LBBMLGraphicsInterface.h>
#include <LBBML_ROOT/include/LBBMLGraphics/Drawable/CubeDrawable.h>
#include <LBBML_ROOT/include/LBBMLGraphics/BoundingBox/AABB.h>

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

GLfloat gCubePos[3];
GLuint gVAO, gVBO, gIBO;
GLuint gProgram, gAABBProgram;
GLboolean gCubePosFlag[3];

CLBBMLGraphicsCubeDrawable* gCube;
std::vector<glm::vec3> gVertices;
CLBBMLAABB* gAABB;

//******************************************************************
//FUNCTION:
void computeCubePos()
{
	glm::vec3 MinCorner = gAABB->getMin();
	glm::vec3 MaxCorner = gAABB->getMax();
	
	SOBBHalfLength OBBHalfLength;
	OBBHalfLength = gCube->getOBBHalfLength();
	
	float MaxLength = sqrt(pow(OBBHalfLength.X_HalfLength,2) + pow(OBBHalfLength.Y_HalfLength,2) + pow(OBBHalfLength.Z_HalfLength,2));

	gCubePosFlag[0] = gCubePos[0]-MaxLength < MinCorner[0] ? true  : gCubePosFlag[0];
	gCubePosFlag[0] = gCubePos[0]+MaxLength > MaxCorner[0] ? false : gCubePosFlag[0];
	gCubePosFlag[1] = gCubePos[1]-MaxLength < MinCorner[1] ? true  : gCubePosFlag[1];
	gCubePosFlag[1] = gCubePos[1]+MaxLength > MaxCorner[1] ? false : gCubePosFlag[1];
	gCubePosFlag[2] = gCubePos[2]-MaxLength < MinCorner[2] ? true  : gCubePosFlag[2];
	gCubePosFlag[2] = gCubePos[2]+MaxLength > MaxCorner[2] ? false : gCubePosFlag[2];

	gCubePos[0] = gCubePosFlag[0] == true ? (gCubePos[0] + 0.1) : (gCubePos[0] -= 0.1);
	gCubePos[1] = gCubePosFlag[1] == true ? (gCubePos[1] + 0.2) : (gCubePos[1] -= 0.2);
	gCubePos[2] = gCubePosFlag[2] == true ? (gCubePos[2] + 0.5) : (gCubePos[2] -= 0.5);
}

//******************************************************************
//FUNCTION:
void initTestData()
{
	glm::vec3 Vertex0 = glm::vec3(-5, -5, -5);
	gVertices.push_back(Vertex0);
	glm::vec3 Vertex1 = glm::vec3(5, 5, 5);
	gVertices.push_back(Vertex1);
	glm::vec3 Vertex2 = glm::vec3(-5, 2, 2);
	gVertices.push_back(Vertex2);
}

//******************************************************************
//FUNCTION:
void initShaderProgram()
{
	gProgram = glCreateProgram();
	LBBMLGraphics::bindShader(gProgram, VERTEX_SHADER, "vert.glsl");
	LBBMLGraphics::bindShader(gProgram, FRAGMENT_SHADER, "frag.glsl");
	LBBMLGraphics::linkShaderProgram(gProgram);

	gAABBProgram = glCreateProgram();
	LBBMLGraphics::bindShader(gAABBProgram, VERTEX_SHADER, "AABBvert.glsl");
	LBBMLGraphics::bindShader(gAABBProgram, FRAGMENT_SHADER, "AABBfrag.glsl");
	LBBMLGraphics::linkShaderProgram(gAABBProgram);
}

//******************************************************************
//FUNCTION:
static void createVertexBuffer()
{
	std::vector<glm::vec3> Vertices;
	Vertices = gAABB->getBoxVertices();
	
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size()*12, &Vertices[0], GL_STATIC_DRAW);
}

//******************************************************************
//FUNCTION:
static void createIndexBuffer()
{
	unsigned int Indices[] = {0,1,1,2,2,3,0,3,3,5,5,6,6,0,6,7,7,4,4,5,2,4,1,7};

	glGenBuffers(1, &gIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
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

	glm::mat4 ViewMat = glm::lookAt(glm::vec3(20, 15, 10), glm::vec3(0,0,0), glm::vec3(0,1,0));
	glm::mat4 ProMat  = glm::perspective<float>(45.0f, 1024.0 / 768.0, 0.1f, 40.0f);
	glUniformMatrix4fv(glGetUniformLocation(gProgram, "uVP"), 1, GL_FALSE, value_ptr(ProMat * ViewMat));

	computeCubePos();
	STransform Transform;
	Transform.m_Position = glm::vec3(gCubePos[0],gCubePos[1],gCubePos[2]);
	Transform.m_Scale    = glm::vec3(1,1,1);
	Transform.m_RotAxis  = glm::vec3(1,1,1);
	Transform.m_RotAngle = 0;
  	gCube->drawV(gProgram, Transform);

	glUseProgram(0);

	glUseProgram(gAABBProgram);

	glUniformMatrix4fv(glGetUniformLocation(gAABBProgram, "uVP"), 1, GL_FALSE, value_ptr(ProMat * ViewMat));

	glPointSize(3.0f);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);

	glUseProgram(0);
}

//************************************************************
//Function:
void initializeVariables()
{
	gVAO = 0;
	gVBO = 0;
	gIBO = 0;

	gProgram     = 0;
	gAABBProgram = 0;

	gCubePos[0] = -3.0;
	gCubePos[1] = -3.0;
	gCubePos[2] = -3.0;

	gCubePosFlag[0] = true;
	gCubePosFlag[1] = true;
	gCubePosFlag[2] = true;

	initTestData();
	gAABB = new CLBBMLAABB;
	gAABB -> createV(gVertices);
}

//************************************************************
//Function:
void releaseVariables()
{
	delete(gCube);
	delete(gAABB);
}

//************************************************************
//Function:
void startRenderLoop()
{
	if(!glfwInit()) return;

	GLFWwindow* pWindow = glfwCreateWindow(1024, 768, "AABB Test", nullptr, nullptr);
	if(!pWindow)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(pWindow);

	GLenum Result = glewInit();
	if (Result != GLEW_OK)
	{
		fprintf(stderr, "Error : %s\n", glewGetErrorString(Result));
	}

	gCube = new CLBBMLGraphicsCubeDrawable;
	initShaderProgram();
	createVertexBuffer();
	createIndexBuffer();

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

	initializeVariables();
	startRenderLoop();
	releaseVariables();

	system("pause");
	return 0;
}