#pragma once
#include "LBBML_INTERFACE_EXPORTS.h"
#include "../DataSimulator/LBBMLDataSimulator.h"
#include "../MathWorker/LBBMLMathWorker.h"
#include "../StatsCollector/LBBMLStatsCollector.h"

namespace LBBML
{
	LBBML_DLL_EXPORTS void writeData(const std::string& vFilename, const std::vector<boost::any>& vAnyVec);
	LBBML_DLL_EXPORTS void generateSimulationData(int vDim, int vSize, const std::string& vFilename);
	LBBML_DLL_EXPORTS void generateHighDimSimulationDataset(const std::string& vTrainingSet, const std::string& vTestingSet, int vTrainSize, int vTestSize, int vDimension, const EDistanceType& vDistanceType);
	LBBML_DLL_EXPORTS std::vector<glm::vec4> splitRectangle(int vSplitNumberX, int cSplitNumberY, int vWinWidth, int vWinHeight);	
}