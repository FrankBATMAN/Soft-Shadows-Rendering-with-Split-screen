/*******************************************************
 ** Author : YX
 ** Data : 2016-6-14
 ** Test Purpose: test if the hardShadowMapping is correct. NSFC-41
*******************************************************/

#include <LBBML_ROOT/include/LBBMLGraphicsApp/SoftShadowMapping/SoftShadowMapping.h>
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
	//_crtBreakAlloc = 1023;
#endif
}

int gMapSequenceNumber;
CLBBMLGraphicsSoftShadowMapping* gSoftShadowMapping;

//********************************************************
//Function:
void initializeVariables()
{
	gMapSequenceNumber = 0;
	gSoftShadowMapping = new CLBBMLGraphicsSoftShadowMapping;
}

//********************************************************
//Function:
void releaseVariables()
{
	delete(gSoftShadowMapping);
}

//********************************************************
//Function:
void callbackKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		gMapSequenceNumber = gMapSequenceNumber >= gSoftShadowMapping->getLightSourceSize() ? 0 : ++gMapSequenceNumber;
	}
}

//********************************************************
//Function:
void startRenderLoop()
{
	if (!glfwInit()) return;

	GLFWwindow* pWindow = glfwCreateWindow(1024, 768, "Test Hard Shadow Mapping", nullptr, nullptr);
	if (!pWindow)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(pWindow);
	glfwSetKeyCallback(pWindow, callbackKey);

	gSoftShadowMapping->initFromConfig("config.xml");
	while (!glfwWindowShouldClose(pWindow))
	{
		glClearColor(0.4f,0.6f,0.8f,1.0f);
		gSoftShadowMapping->run(DISPLAY_DEPTH_MAP, HARD_SHADOW_MAP, gMapSequenceNumber, gSoftShadowMapping->getLightSourceSize());

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