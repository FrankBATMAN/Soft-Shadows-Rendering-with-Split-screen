/*******************************************************
 ** Author : YX
 ** Data : 2016-5-11
 ** Test Purpose: Test generating Training Set , and 
 ** generate testing set, testing set is near training set
 ** in  hyper space
*******************************************************/

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
	//_crtBreakAlloc = 19884;
#endif
}

int main()
{
	installMemoryLeakDetector();

	//Parameters:6 parameters
	//Training Set filename, Testing Set filename, Train Size, test size, dimension, distance type
	LBBML::generateHighDimSimulationDataset("TrainManhattan10.csv", "TestManhattan10.csv", 500, 100, 10, MANHATTAN_DISTANCE);
	LBBML::generateHighDimSimulationDataset("TrainManhattan20.csv", "TestManhattan20.csv", 500, 100, 20, MANHATTAN_DISTANCE);
	LBBML::generateHighDimSimulationDataset("TrainEuclidean10.csv", "TestEuclidean10.csv", 500, 100, 10, EUCLIDEAN_DISTANCE);
	LBBML::generateHighDimSimulationDataset("TrainEuclidean20.csv", "TestEuclidean20.csv", 500, 100, 20, EUCLIDEAN_DISTANCE);
	LBBML::generateHighDimSimulationDataset("TrainMahalanobis10.csv", "TestMahalanobis10.csv", 500, 100, 10, MAHANALOBIS_DISTANCE);
	LBBML::generateHighDimSimulationDataset("TrainMahalanobis20.csv", "TestMahalanobis20.csv", 500, 100, 20, MAHANALOBIS_DISTANCE);

	system("pause");
	return 0;
}