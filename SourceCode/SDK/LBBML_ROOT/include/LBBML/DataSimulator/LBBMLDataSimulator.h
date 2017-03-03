#pragma once
#include "Functions/LBBMLFunctions.h"
#include "Distances/LBBMLDistances.h"
#include "../StatsCollector/LBBMLStatsCollector.h"
#include "../MathWorker/LBBMLMathWorker.h"

namespace LBBMLDataSimulator
{
	void generateSimulationDatasetWithoutDistance(int vDim, int vSize, const std::string& vFilename);
	void generateHighDimSimulationDatasetWithDistance(const std::string& vTrainingSet, const std::string& vTestingSet, int vTrainSize, int vTestSize, int vDimension, const EDistanceType& vDistanceType);	
	void printFormula(const std::vector<float>& vWeights, const std::vector<CFunctionBase*>& vFunctions);
	void writeTrainOrTestSet(const std::string& vFilename, const std::vector<std::vector<float>>& vVariables, const std::vector<float>& vWeights, const std::vector<CFunctionBase*>& vFunctions);

	float calculateFormulaValue(const std::vector<float>& vWeights, const std::vector<CFunctionBase*>& vFunctions, const std::vector<float>& vX);
	
	std::vector<std::vector<float>> generatePointsCloudAroundOnePoint(int vSize, int vDim, const std::vector<float>& vPoint, float vRadius, const EDistanceType& vDistanceType);
};