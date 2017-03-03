/*******************************************************
 ** Author : YX
 ** Data : 2016-6-14
 ** Test Purpose: test if the softshadowmapping is correct. NSFC-41
*******************************************************/

#include <OpenGL/include/openGL/glew.h>
#include <OpenGL/include/openGL/glfw3.h>
#include <LBBML_ROOT/include/LBBMLGraphicsApp/SoftShadowMapping/SoftShadowMapping.h>

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

CLBBMLGraphicsSoftShadowMapping* gSoftShadowMapping;

unsigned int gLightNumber;

//*********************************************************************************
//FUNCTION:
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		gLightNumber++;
		gLightNumber = gLightNumber  >= gSoftShadowMapping->getLightSourceSize() ? gSoftShadowMapping->getLightSourceSize() : gLightNumber;
	}
}

//************************************************************
//Function:
void initializeVariables()
{
	gLightNumber = 10;
	gSoftShadowMapping = new CLBBMLGraphicsSoftShadowMapping;
}

//************************************************************
//Function:
void releaseVariables()
{
	delete(gSoftShadowMapping);
}

//************************************************************
//Function:
void startRenderLoop()
{
	if(!glfwInit()) return;

	GLFWwindow* pWindow = glfwCreateWindow(1024, 768, "Soft Shadow Mapping with Split-screen", nullptr, nullptr);
	if(!pWindow)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(pWindow);
	glfwSetKeyCallback(pWindow, keyboardHandler);

	gSoftShadowMapping->initFromConfig("config.xml");

	while(!glfwWindowShouldClose(pWindow))
	{
		glClearColor(0.4f,0.6f,0.8f,1.0f);
		gSoftShadowMapping->run(DISPLAY_SHADOW_MAP, SOFT_SHADOW_MAP, 0, gLightNumber);
		
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