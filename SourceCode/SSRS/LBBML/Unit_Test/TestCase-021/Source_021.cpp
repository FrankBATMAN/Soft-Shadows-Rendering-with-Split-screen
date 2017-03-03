/*******************************************************
 ** Author : YX
 ** Data : 2016-6-29
 ** Test Purpose: test use Glfw, handle memory leak
*******************************************************/

#include <LBBML_ROOT/include/LBBMLGraphics/Viewer/Window.h>
#include <LBBML_ROOT/include/LBBMLGraphicsApp/SoftShadowMapping/SoftShadowMapping.h>
#include <OpenGL/include/openGL/glfw3.h>
#include <OpenGL/include/openGL/freeglut.h>

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
CLBBMLGraphicsWindow* gWindow;

//************************************************************
//Function:
void initializeVariables()
{
	gSoftShadowMapping = new CLBBMLGraphicsSoftShadowMapping;
	gWindow = new CLBBMLGraphicsWindow;
}

//************************************************************
//Function:
void releaseVariables()
{
	delete(gSoftShadowMapping);
	delete(gWindow);
}

//***********************************************************
//Function:
void render()
{
	glClearColor(0.4f,0.6f,0.8f,1.0f);
	gSoftShadowMapping->run(DISPLAY_SHADOW_MAP, SOFT_SHADOW_MAP, 0, 100);
}

//************************************************************
//Function:
void startRenderLoop()
{
	boost::property_tree::ptree PTree;
	boost::property_tree::xml_parser::read_xml("config.xml", PTree);

	glm::vec2 WindowSize = extractVectorFromString<glm::vec2>(PTree.get<std::string>("softShadowMapping.window"));
	std::string WindowTitle = PTree.get<std::string>("softShadowMapping.windowTitle");

	if(gWindow->isWindowCreated(WindowSize.x, WindowSize.y, WindowTitle));
	{
		gSoftShadowMapping->initFromConfig("config.xml");
		gWindow->runDisplayFunc(render);
	}
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