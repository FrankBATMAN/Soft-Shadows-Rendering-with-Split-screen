/*******************************************************
 ** Author : YX
 ** Data : 2016-05-06
 ** Test Purpose : Test loading common models using OSG and analysis the number of triangle mesh based on different situation.
*******************************************************/

/*******************************************************
 ** Author : YX
 ** Data : 2016-07-16
 ** Attention : Abandoned. Because we don't rely on OSG any more.
 *******************************************************/

#include <iostream>
#include "osgViewer/ViewerBase"

#include "Common/NSFCInterface.h"

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
	_crtBreakAlloc = 161;
#endif
}

int main()
{
	installMemoryLeakDetector();

	if (!NSFC::renderScene("CommonModelConfig.xml"))
	{
		std::cout << "Render scene with common model fail!" << std::endl;
		return false;
	}
	
	return true;
}