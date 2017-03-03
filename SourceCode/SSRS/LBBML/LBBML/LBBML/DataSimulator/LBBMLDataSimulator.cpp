#include "LBBMLDataSimulator.h"

//*******************************************************************************************
//Function:
void LBBMLDataSimulator::generateSimulationDatasetWithoutDistance(int vDim, int vSize, const std::string& vFilename)
{
	_ASSERT(!vFilename.empty() && vDim > 0 && vSize > 0, "invalid parameters");

	std::vector<float> Weights = LBBMLMathWorker::generateRandomVectorf(vDim, 1, 10);

	std::vector<CFunctionBase*> Functions;
	CLBBMLFunctions *MyFunc = new CLBBMLFunctions;
	for (unsigned int i = 0; i < vDim; ++i)
		Functions.push_back(MyFunc->getFunctionByIndex(LBBMLMathWorker::generateRandomi(0, 6)));

	printFormula(Weights, Functions);

	for (unsigned int i = 0; i < vSize; ++i)
	{
		std::vector<float> X = LBBMLMathWorker::generateRandomVectorf(vDim, 1, 10);
		std::vector<boost::any> DataVec;
		for (unsigned int dim = 0; dim < X.size(); ++dim)
			DataVec.push_back(X[dim]);

		DataVec.push_back(calculateFormulaValue(Weights, Functions, X));

		LBBMLStatsCollector::writeData(vFilename, DataVec);
	}

	delete(MyFunc);
	Functions.clear();
}

//*******************************************************************************************
//Function:
void LBBMLDataSimulator::generateHighDimSimulationDatasetWithDistance(const std::string& vTrainingSet, const std::string& vTestingSet, int vTrainSize, int vTestSize, int vDimension, const EDistanceType& vDistanceType)
{
	_ASSERT(!vTrainingSet.empty() && !vTestingSet.empty() && ((vTestSize > 0) && (vTrainSize > 0) && (vDimension > 0)), "invalid parameters");

	std::vector<float> Weights = LBBMLMathWorker::generateRandomVectorf(vDimension, 1, 10);

	std::vector<CFunctionBase*> Functions;
	CLBBMLFunctions *MyFunc = new CLBBMLFunctions;
	for (int i = 0; i < vDimension; ++i)
	{
		Functions.push_back(MyFunc->getFunctionByIndex(LBBMLMathWorker::generateRandomi(0, 2)));
	}

	printFormula(Weights, Functions);

	std::vector<float> Mean = LBBMLMathWorker::generateRandomVectorf(vDimension, 1, 5);
	float Radius = 100.0;

	std::vector<std::vector<float>> TrainingSet = generatePointsCloudAroundOnePoint(vTrainSize,vDimension,Mean,Radius,vDistanceType);
	writeTrainOrTestSet(vTrainingSet, TrainingSet, Weights, Functions);

	std::vector<std::vector<float>> TestingSet = generatePointsCloudAroundOnePoint(vTestSize, vDimension, Mean, Radius * 1.5, vDistanceType);
	writeTrainOrTestSet(vTestingSet, TestingSet, Weights, Functions);

	delete(MyFunc);
	Functions.clear();
}

//*******************************************************************************************
//Function:
void LBBMLDataSimulator::printFormula(const std::vector<float>& vWeights, const std::vector<CFunctionBase*>& vFunctions)
{
	_ASSERT(vWeights.size() > 0 && vFunctions.size() == vWeights.size(), "print error!");

	int  Dim = 0;
	auto Itr = vFunctions.begin();

	for (; (Dim < vWeights.size() - 1) && (Itr != vFunctions.end() - 1); ++Dim, ++Itr)
	{
		std::cout << vWeights[Dim] << " * ";
		(*Itr)->printMyFormulaV();
		std::cout << " + ";
	}

	std::cout << vWeights[Dim] << " * ";
	(*Itr)->printMyFormulaV();
	std::cout << std::endl;
}

float LBBMLDataSimulator::calculateFormulaValue(const std::vector<float>& vWeights, const std::vector<CFunctionBase*>& vFunctions, const std::vector<float>& vX)
{
	_ASSERT(vWeights.size() > 0 && vFunctions.size() == vWeights.size() && vX.size() == vWeights.size(), "calculate error!");

	float FormulaValue = 0.0;
	auto Itr = vFunctions.begin();
	for (int Dim = 0; Dim < vWeights.size() && Itr != vFunctions.end(); ++Dim, ++Itr)
		FormulaValue += vWeights[Dim] * (*Itr)->calculateFunctionValueV(vX[Dim]);

	return FormulaValue;
}

//*******************************************************************************
//Function:
std::vector<std::vector<float>> LBBMLDataSimulator::generatePointsCloudAroundOnePoint(int vSize, int vDim, const std::vector<float>& vPoint, float vRadius, const EDistanceType& vDistanceType)
{
	std::vector<std::vector<float>> PointsCloud;

	CLBBMLDistances* Distance = new CLBBMLDistances;
	while (vSize != 0)
	{
		std::vector<float> Point = LBBMLMathWorker::generateRandomVectorf(vDim, 1, 5);
		float Dis = Distance->calculateDistance(vPoint, Point,vDistanceType);
		if (Dis <= vRadius)
		{
			PointsCloud.push_back(Point);
			vSize--;
		}
	}
	delete(Distance);

	return PointsCloud;
}

//**********************************************************************************
//Function:
void LBBMLDataSimulator::writeTrainOrTestSet(const std::string& vFilename, const std::vector<std::vector<float>>& vVariables, const std::vector<float>& vWeights, const std::vector<CFunctionBase*>& vFunctions)
{
	for (unsigned int i = 0; i < vVariables.size(); ++i)
	{
		std::vector<boost::any> SingleData;
		for (unsigned int k = 0; k < vVariables[i].size(); ++k)
			SingleData.push_back(vVariables[i][k]);
		float FormulaValue = calculateFormulaValue(vWeights, vFunctions, vVariables[i]);
		SingleData.push_back(FormulaValue);

		LBBMLStatsCollector::writeData(vFilename, SingleData);
	}
}