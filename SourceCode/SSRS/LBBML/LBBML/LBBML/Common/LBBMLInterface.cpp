#include "LBBMLInterface.h"

#pragma region EXPORTED_FUNCTION

//************************************************************************************
//Function: 
void LBBML::writeData(const std::string& vFilename, const std::vector<boost::any>& vAnyVec)
{
	LBBMLStatsCollector::writeData(vFilename, vAnyVec);
}

//************************************************************************************
//Function:
void LBBML::generateSimulationData(int vDim, int vSize, const std::string& vFilename)
{
	LBBMLDataSimulator::generateSimulationDatasetWithoutDistance(vDim, vSize, vFilename);
}

//**********************************************************************************
//Function:
void LBBML::generateHighDimSimulationDataset(const std::string& vTrainingSet, const std::string& vTestingSet, int vTrainSize, int vTestSize, int vDimension, const EDistanceType& vDistanceType)
{
	LBBMLDataSimulator::generateHighDimSimulationDatasetWithDistance(vTrainingSet, vTestingSet, vTrainSize, vTestSize, vDimension, vDistanceType);
}

//******************************************************************
//FUNCTION:
std::vector<glm::vec4> LBBML::splitRectangle(int vSplitNumberX, int vSplitNumberY, int vWinWidth, int vWinHeight)
{
	return LBBMLMathWorker::splitRectangle(vSplitNumberX, vSplitNumberY, vWinWidth, vWinHeight);
}
#pragma endregion EXPORTED_FUNCTION