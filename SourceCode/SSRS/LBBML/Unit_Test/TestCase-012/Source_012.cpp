/*******************************************************
 ** Author : YX
 ** Data : 2016-6-8
 ** Test Purpose: test create OBB and display it: NSFC-39
*******************************************************/

#include <OpenGL/include/openGL/glfw3.h>
#include <LBBML_ROOT/include/LBBML/Common/LBBMLInterface.h>
#include <LBBML_ROOT/include/LBBMLGraphics/BoundingBox/OBB.h>

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

CLBBMLOBB* gOBB;
std::vector<glm::vec3> gVertices;

//*********************************************************************************
//FUNCTION:
void drawCubeLine(const glm::vec3& vBegin, const glm::vec3& vEnd)
{
	glVertex3f(vBegin[0], vBegin[1], vBegin[2]);
	glVertex3f(vEnd[0], vEnd[1], vEnd[2]);
}

//*********************************************************************************
//FUNCTION:
void drawMesh()
{
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0,1.0,1.0);
	for(auto Itr : gVertices)
		glVertex3f(Itr[0],Itr[1],Itr[2]);
	glEnd();
}

//*********************************************************************************
//FUNCTION:
glm::vec3 stepVector(SOBBAxis vAxis, SOBBHalfLength vHalfLength, float vX, float vY, float vZ)
{
	glm::vec3 X = vX * (vAxis.X_Axis) * (vHalfLength.X_HalfLength);
	glm::vec3 Y = vY * (vAxis.Y_Axis) * (vHalfLength.Y_HalfLength);
	glm::vec3 Z = vZ * (vAxis.Z_Axis) * (vHalfLength.Z_HalfLength);
	
	return X + Y + Z;
}

//*********************************************************************************
//FUNCTION:
void render()
{
	glEnable(GL_DEPTH_TEST);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClearColor(0.4, 0.6, 0.8, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1024.0 / 768.0, 0.1, 50);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(20,20,20, 0,0,0, 0,1,0);

	glLineWidth(5.0f);

	//x,y,z coord
	glBegin(GL_LINES);
	glColor3f(0,0,0);
	glVertex3f(0,0,0);
	glVertex3f(0,10,0);
	glVertex3f(0,0,0);
	glVertex3f(10,0,0);
	glVertex3f(0,0,0);
	glVertex3f(0,0,10);
	glEnd();

	//draw obb
	//1:get obb
	glm::vec3 Center = gOBB->getCenter();
	SOBBAxis Axis = gOBB->getAxis();
	SOBBHalfLength HalfLength = gOBB->getHalfLength();

	//2:draw center + axis
	glBegin(GL_LINES);
	glColor3f(1,0,0);
	glVertex3f(Center[0], Center[1], Center[2]);
	glVertex3f(Center[0] + Axis.X_Axis[0], Center[1] + Axis.X_Axis[1], Center[2] + Axis.X_Axis[2]);

	glColor3f(0,1,0);
	glVertex3f(Center[0], Center[1], Center[2]);
	glVertex3f(Center[0] + Axis.Y_Axis[0], Center[1] + Axis.Y_Axis[1], Center[2] + Axis.Y_Axis[2]);

	glColor3f(0,0,1);
	glVertex3f(Center[0], Center[1], Center[2]);
	glVertex3f(Center[0] + Axis.Z_Axis[0], Center[1] + Axis.Z_Axis[1], Center[2] + Axis.Z_Axis[2]);
	glEnd();

	//3:draw obb cube
	glm::vec3 v[8];
	v[0] = Center + stepVector(Axis, HalfLength, -1, -1, -1);
	v[1] = Center + stepVector(Axis, HalfLength, -1, -1, 1);
	v[2] = Center + stepVector(Axis, HalfLength, 1, -1, 1);
	v[3] = Center + stepVector(Axis, HalfLength, 1, -1, -1);
	v[4] = Center + stepVector(Axis, HalfLength, 1, 1, -1);
	v[5] = Center + stepVector(Axis, HalfLength, 1, 1, 1);
	v[6] = Center + stepVector(Axis, HalfLength, -1, 1, 1);
	v[7] = Center + stepVector(Axis, HalfLength, -1, 1, -1);

	glLineWidth(3.0f);
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_LINES);
	drawCubeLine(v[0], v[1]);
	drawCubeLine(v[0], v[3]);
	drawCubeLine(v[0], v[7]);

	drawCubeLine(v[1], v[2]);
	drawCubeLine(v[1], v[6]);
	
	drawCubeLine(v[2], v[3]);
	drawCubeLine(v[2], v[5]);

	drawCubeLine(v[3], v[4]);

	drawCubeLine(v[4], v[5]);
	drawCubeLine(v[4], v[7]);

	drawCubeLine(v[6], v[5]);
	drawCubeLine(v[6], v[7]);

	glEnd();

	drawMesh();
}

//*********************************************************************************
//FUNCTION:
void initializeVariables()
{
	for(unsigned int i = 0; i < 100; ++i)
	{
		glm::vec3 Vertex;
		Vertex.x = LBBMLMathWorker::generateRandomf(-2, 2);
		Vertex.y = LBBMLMathWorker::generateRandomf(-2, 2);
		Vertex.z = LBBMLMathWorker::generateRandomf(-2, 2);
		gVertices.push_back(Vertex);
	}

	gOBB = new CLBBMLOBB;
	gOBB->createV(gVertices);
}

//*********************************************************************************
//FUNCTION:
void releaseVariables()
{
	delete(gOBB);
}

//************************************************************
//Function:
void startRenderLoop()
{
	if(!glfwInit()) return;

	GLFWwindow* pWindow = glfwCreateWindow(1024, 768, "OBB Test", nullptr, nullptr);
	if(!pWindow)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(pWindow);

	while(!glfwWindowShouldClose(pWindow))
	{
		render();
		gOBB->rotate(glm::vec3(0,1,0), 0.02);
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