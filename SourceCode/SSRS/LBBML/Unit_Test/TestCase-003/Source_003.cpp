/*******************************************************
 ** Author : YX
 ** Data : 2016-4-29
 ** Test Purpose: Test Random Generator for High Dimension Simulation
*******************************************************/

#include <vector>
#include <iostream>
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
	//_crtBreakAlloc = 30337;
#endif
}

int main()
{
	installMemoryLeakDetector();

	LBBML::generateSimulationData(20, 100, "testData.csv");

	system("pause");
	return 0;
}