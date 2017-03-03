#include "LBBMLMathWorker.h"
#include <time.h>
#include <boost/random.hpp>

//************************************************************
//Function:
int	LBBMLMathWorker::generateRandomi(int vMin, int vMax)
{
	static time_t seed = time(0);
	boost::random::mt19937 RandomNumberGenerator(seed++);
	boost::random::uniform_int_distribution<> Range;
	if (vMax > vMin)
		Range = boost::random::uniform_int_distribution<>(vMin, vMax);
	else
		Range = boost::random::uniform_int_distribution<>(vMax, vMin);

	return Range(RandomNumberGenerator);
}

//************************************************************
//Function:
float LBBMLMathWorker::generateRandomf(float vMin, float vMax)
{
	static time_t seed = time(0);
	boost::random::mt19937 RandomNumberGenerator(seed++);
	boost::random::uniform_real_distribution<> Range;
	if (vMax > vMin)
		Range = boost::random::uniform_real_distribution<>(vMin, vMax);
	else
		Range = boost::random::uniform_real_distribution<>(vMax, vMin);

	return Range(RandomNumberGenerator);
}

//************************************************************
//Function:
std::vector<int> LBBMLMathWorker::generateRandomVectori(unsigned int vSize, int vMin, int vMax)
{
	std::vector<int> RandomIntegerList;
	for (unsigned int i = 0; i < vSize; ++i)
		RandomIntegerList.push_back(generateRandomi(vMin, vMax));
	return RandomIntegerList;
}

//************************************************************
//Function:
std::vector<float> LBBMLMathWorker::generateRandomVectorf(unsigned int vSize, float vMin, float vMax)
{
	std::vector<float> RandomFloatList;
	for (unsigned int i = 0; i < vSize; ++i)
		RandomFloatList.push_back(generateRandomf(vMin, vMax));
	return RandomFloatList;
}

//************************************************************
//Function:
std::vector<glm::vec4> LBBMLMathWorker::splitRectangle(int vSplitNumberX, int vSplitNumberY, int vWinWidth, int vWinHeight)
{
	assert(vSplitNumberX > 0 && vSplitNumberY > 0 && vWinWidth > 0 && vWinHeight > 0, "invalid parameters");

	std::vector<glm::vec4> SubWindows;

	int SubWinWidth = vWinWidth / vSplitNumberX, SubWinHeight = vWinHeight / vSplitNumberY;
	for (int StepX = 0; StepX < vSplitNumberX; ++StepX)
	{
		for (int StepY = 0; StepY < vSplitNumberY; ++StepY)
		{
			float LeftBottomX = 0.0, LeftBottomY = 0.0, RightTopX = 0.0, RightTopY = 0.0;
			LeftBottomX = StepX * SubWinWidth;
			LeftBottomY = StepY * SubWinHeight;
			RightTopX   = LeftBottomX + SubWinWidth;
			RightTopY   = LeftBottomY + SubWinHeight;

			glm::vec4 SubWindow(LeftBottomX, LeftBottomY, RightTopX, RightTopY);
			SubWindows.push_back(SubWindow);
		}
	}

	return SubWindows;
}