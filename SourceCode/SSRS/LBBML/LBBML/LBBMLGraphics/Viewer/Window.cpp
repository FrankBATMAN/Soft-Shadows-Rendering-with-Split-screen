#include "Window.h"

CLBBMLGraphicsWindow::CLBBMLGraphicsWindow(void)
{

}

CLBBMLGraphicsWindow::~CLBBMLGraphicsWindow(void)
{

}

//**************************************************************************************
//Function:
bool CLBBMLGraphicsWindow::isWindowCreated(int vWidth, int vHeight, const std::string& vWindowTitle)
{
	_ASSERT(vWidth > 0 && vHeight > 0, "window size must be positive");

	if(!glfwInit())
	{
		fprintf(stderr, "unable to init Glfw");
		return false;
	}

	m_pWindow = glfwCreateWindow(vWidth, vHeight, vWindowTitle.c_str(), NULL, NULL);
	if(!m_pWindow)
	{
		fprintf(stderr, "unable to create Glfw window");
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_pWindow);

	return __initGlew();
}

//**************************************************************************************
//Function:
void CLBBMLGraphicsWindow::runDisplayFunc(void (*CALLBACK)(void))
{
	while(!glfwWindowShouldClose(m_pWindow))
	{
		CALLBACK();

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

//**************************************************************************************
//Function:
bool CLBBMLGraphicsWindow::__initGlew()
{
	if(glewInit() != GLEW_OK)
	{
		fprintf(stderr, "unable to init Glew");
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
		return false;
	}

	return true;
}