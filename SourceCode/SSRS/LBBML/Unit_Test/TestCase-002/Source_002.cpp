/*******************************************************
 ** Author : YX
 ** Data : 2016-4-27
 ** Test Purpose: read CSV from TestCase-001 to see if 
 *****************matches the original
*******************************************************/

#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <GLM/glm.hpp>

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

	std::string FilePath = "..\\TestCase-001\\csv.csv";
	int ColumnNumber = 0, NumLines = 0;

	std::ifstream CSV(FilePath);
	_ASSERT(CSV);

	std::string line;
	while(getline(CSV,line))
	{
		NumLines++;
		std::cout << line << std::endl;
		std::vector<std::string> Elements;
		boost::split(Elements, line, boost::is_any_of(","));
		ColumnNumber = Elements.size();

		int a = atoi(Elements[0].c_str());
		bool b = atoi(Elements[1].c_str());
		char c = Elements[2].c_str()[0];
		float d = atof(Elements[3].c_str());
		std::string e = Elements[4];
		glm::vec3 f(atoi(Elements[5].c_str()),atoi(Elements[6].c_str()),atoi(Elements[7].c_str()));
	}

	system("pause");
	return 0;
}