/*******************************************************
 ** Author : YX
 ** Data : 2016-4-27
 ** Test Purpose: write multi data types to CSV table file	
*******************************************************/

#include <memory>
#include <string>
#include <vector>
#include <boost/any.hpp>
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

	std::string Filename = "csv.csv";

	int a = 1; 
	bool b = true;
	char c = 'c';
	float d = 1.234;
	std::string e = "colName";
	glm::vec3 f(1,2,3);

	std::vector<boost::any> Factors;
	Factors.push_back(a);
	Factors.push_back(b);
	Factors.push_back(c);
	Factors.push_back(d);
	Factors.push_back(e);
	Factors.push_back(f);

	LBBML::writeData(Filename, Factors);

	system("pause");
	return 0;
}