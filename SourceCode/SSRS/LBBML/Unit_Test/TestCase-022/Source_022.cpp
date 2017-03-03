/*******************************************************
 ** Author : YX
 ** Data : 2016-07-20
 ** Test Purpose: test use SOIL to load a image file as a texture
*******************************************************/

#include <memory>
#include <LBBML_ROOT/include/LBBMLGraphics/Viewer/Window.h>
#include <LBBML_ROOT/include/LBBMLGraphics/Drawable/QuadDrawable.h>
#include <LBBML_ROOT/include/LBBMLGraphics/GraphicsCommon/LBBMLGraphicsInterface.h>
#include <OpenGL/include/openGL/freeglut.h>
#include <GLM/gtc/matrix_transform.hpp>

//FUNCTION: detect the memory leak in DEBU G mode
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

CLBBMLGraphicsQuadDrawable* gQuad;
GLuint gImage;
GLuint gShaderProgram;

void render()
{
	glClearColor(0.4, 0.6, 0.8, 1.0);

	glUseProgram(gShaderProgram);

	glm::mat4 ModelMat = glm::mat4(1.0);
	ModelMat = glm::scale(ModelMat, glm::vec3(15, 1, 15));
	ModelMat = glm::translate(ModelMat, glm::vec3(0, 0, 0));

	glm::mat4 ViewMat = glm::lookAt(glm::vec3(30, 30, 30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 ProjMat = glm::perspective<float>(45.0, 1024.0 / 768.0, 0.1, 200.0);
	glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "uMVP"), 1, GL_FALSE, value_ptr(ProjMat * ViewMat * ModelMat));

	glUniform1i(glGetUniformLocation(gShaderProgram, "uImage"), 0);

	glViewport(0,0,1024,768);
	glClear(GL_COLOR_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gImage);
	STransform Tranform;
	Tranform.m_Position = glm::vec3(0,0,0);
	Tranform.m_Scale = glm::vec3(15, 1, 15);
	Tranform.m_RotAxis = glm::vec3(0,1,0);
	Tranform.m_RotAngle = 0.0;
	gQuad->drawV(gShaderProgram, Tranform);

	glUseProgram(0);
}

void initShaderProgram()
{
	LBBMLGraphics::createShaderProgram(gShaderProgram);
	LBBMLGraphics::bindShader(gShaderProgram, VERTEX_SHADER, "VertexShader.glsl");
	LBBMLGraphics::bindShader(gShaderProgram, FRAGMENT_SHADER, "FragmentShader.glsl");
	LBBMLGraphics::linkShaderProgram(gShaderProgram);
}

int main(void)
{
	installMemoryLeakDetector();

	CLBBMLGraphicsWindow* pWindow = new CLBBMLGraphicsWindow;
	if(pWindow->isWindowCreated(1024, 768, "test SOIL load Image"))
	{
		initShaderProgram();
		gQuad = new CLBBMLGraphicsQuadDrawable;
		gImage = LBBMLGraphics::loadImageAsTexture(GL_TEXTURE_2D, LOAD_AS_GREY, "wall.jpg");

		pWindow->runDisplayFunc(render);

		delete(gQuad);
	}

	delete(pWindow);

	return 0;
}