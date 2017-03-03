/*******************************************************
 ** Author : YX
 ** Data : 2016-6-13
 ** Test Purpose: test Save texture to an image. NSFC-42
*******************************************************/

#include <GLM/glm.hpp>
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

GLuint gVAO;
GLuint gVBO;
GLuint gShaderProgramID;

//********************************************************
//Function:
void initializeOpenGL()
{
	glClearColor(0.0, 1.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
}

//********************************************************
//Function:
void initializeVAO()
{
	glm::vec3 Triangle[3];
	Triangle[0] = glm::vec3(-1.0f, -1.0f, -1.0f);
	Triangle[1] = glm::vec3(1.0f, -1.0f, -0.5f);
	Triangle[2] = glm::vec3(0.0f, 1.0f, -0.2f);

	glGenBuffers(1, &gVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle, GL_STATIC_DRAW);
}

//********************************************************
//Function:
void initializeFBO()
{
	GLuint VAO;
	LBBMLGraphics::createFBO(VAO, gVBO, 1024, 768, RGBA_FBO);
}

//********************************************************
//Function:
void initializeShaderProgram()
{
	gShaderProgramID = glCreateProgram();
	LBBMLGraphics::bindShader(gShaderProgramID, VERTEX_SHADER, "vert.glsl");
	LBBMLGraphics::bindShader(gShaderProgramID, FRAGMENT_SHADER, "frag.glsl");
	LBBMLGraphics::linkShaderProgram(gShaderProgramID);
}

//********************************************************
//Function:
void initializeVariables()
{
	initializeOpenGL();
	initializeVAO();
	initializeFBO();
	initializeShaderProgram();
}

//********************************************************
//Function:
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gShaderProgramID);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, gVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, gVBO);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);

	LBBMLGraphics::saveGLDisplay2Image(gVBO, 0, 0, 1024, 768, RGB_DISPLAY, true);

	glUseProgram(gShaderProgramID);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, gVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

//************************************************************
//Function:
void startRenderLoop()
{
	if (!glfwInit()) return;

	GLFWwindow* pWindow = glfwCreateWindow(1024, 768, "Save Texture to Image", nullptr, nullptr);
	if (!pWindow)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(pWindow);

	GLenum Ret = glewInit();
	if (Ret != GLEW_OK)
	{
		fprintf(stderr, "Error : %s\n", glewGetErrorString(Ret));
	}

	initializeVariables();

	while (!glfwWindowShouldClose(pWindow))
	{
		glClearColor(0.4f,0.6f,0.8f,1.0f);
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

	system("pause");
	return 0;
}