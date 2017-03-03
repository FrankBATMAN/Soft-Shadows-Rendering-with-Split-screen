/*******************************************************
 ** Author : YX
 ** Data : 2016-5-10
 ** Test Purpose: test split window
*******************************************************/

#include <time.h>
#include <GLM/glm.hpp>
#include <OpenGL/include/openGL/glfw3.h>
#include <OpenGL/include/openGL/freeglut.h>
#include <LBBML_ROOT/include/LBBML/Common/LBBMLInterface.h>

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
	//_crtBreakAlloc = 161;
#endif
}

int gWinWidth, gWinHeight;
int gSplitNumberX, gSplitNumberY;
std::vector<glm::vec4> gSplitResult;

//******************************************************************
//FUNCTION:
void initializeOpenGL()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//********************************************************
//Function:
void initializeVariables()
{
	gWinWidth     = 1024;
	gWinHeight    = 768;
	gSplitNumberX = 4;
	gSplitNumberY = 3;
	gSplitResult  = LBBML::splitRectangle(gSplitNumberX, gSplitNumberY, gWinWidth, gWinHeight);

	initializeOpenGL();
}

//******************************************************************
//FUNCTION:
void render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	srand((unsigned)time(NULL));

	for (auto Iter : gSplitResult)
	{
		float Color[3];
		for (int i = 0; i < 3; ++i)
		{
			Color[i] = ((float)rand()) / RAND_MAX;
		}

		glViewport(Iter[0], Iter[1], Iter[2], Iter[3]);
		glColor3f(Color[0],Color[1],Color[2]);
		glBegin(GL_QUADS);

		glVertex2f(-1,-1);
		glVertex2f( 1,-1);
		glVertex2f( 1, 1);
		glVertex2f(-1, 1);
		glEnd();
	}
}

//********************************************************
//Function:
void startRenderLoop()
{
	if (!glfwInit()) return;

	GLFWwindow* pWindow = glfwCreateWindow(1024, 768, "Test Viewport", nullptr, nullptr);
	if (!pWindow)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(pWindow);

	while (!glfwWindowShouldClose(pWindow))
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

	system("pause");
	return 0;
}