/*******************************************************
 ** Author : YX
 ** Data : 2016-5-18
 ** Test Purpose: test split rendering a triangle
*******************************************************/

/*******************************************************
 ** Author : LQQ
 ** Data : 2016-06-19, 2016-07-16, 2016-07-18
 ** Test Purpose: test split rendering a model through shaderbased way using glfw.
*******************************************************/

#include <LBBML_ROOT/include/LBBMLGraphicsApp/SplitScreenRendering/ShaderBasedSplitScreenRendering.h>
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

CLBBMLGraphicsShaderBasedSplitScreenRendering* gShaderBasedSplitRendering;

//********************************************************
//Function:
void releaseVariables()
{
	delete(gShaderBasedSplitRendering);
}

//********************************************************
//Function:
void startRenderLoop()
{
	if (!glfwInit()) return;

	GLFWwindow* pWindow = glfwCreateWindow(1024, 768, "Split Screen", nullptr, nullptr);
	if (!pWindow)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(pWindow);

	gShaderBasedSplitRendering = new CLBBMLGraphicsShaderBasedSplitScreenRendering;
	gShaderBasedSplitRendering->initFromConfig("SplitScreenRenderingConfig.xml");

	while (!glfwWindowShouldClose(pWindow))
	{
		glClearColor(0.4f,0.6f,0.8f,1.0f);
		gShaderBasedSplitRendering->renderScene();
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