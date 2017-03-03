#pragma once
#include <string>
#include <OpenGL/include/openGL/glew.h>
#include <OpenGL/include/openGL/glfw3.h>
#include "../GraphicsCommon/LBBML_GRAPHICS_INTERFACE_EXPORTS.h"

class LBBML_GRAPHICS_DLL_EXPORTS CLBBMLGraphicsWindow
{
public:
	CLBBMLGraphicsWindow(void);
	virtual ~CLBBMLGraphicsWindow(void);

	bool isWindowCreated(int vWidth, int vHeight, const std::string& vWindowTitle);
	void runDisplayFunc(void (*CALLBACK)(void));

private:
	bool __initGlew();

	GLFWwindow* m_pWindow;
};