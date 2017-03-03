#include "GenerateSamples.h"

//******************************************************************
//FUNCTION:
std::vector<glm::vec2> LBBMLGraphicsSoftShadowLighting::splitRectangle(const glm::vec2& vLeftBottom, const glm::vec2& vRightTop, const glm::vec2& vSampleInterval, const glm::vec2& vSplitNumber)
{
	std::vector<glm::vec2> SplitResult;
	for (unsigned int i = 0; i < vSplitNumber.y; ++i)
	{
		for (unsigned int k = 0; k < vSplitNumber.x; ++k)
		{
			glm::vec2 SubLeftBottom;
			glm::vec2 SubRightTop;
			SubLeftBottom.x = vLeftBottom.x + k * vSampleInterval.x;
			SubLeftBottom.y = vLeftBottom.y + i * vSampleInterval.y;

			SubRightTop.x = SubLeftBottom.x + vSampleInterval.x;
			SubRightTop.y = SubLeftBottom.y + vSampleInterval.y;

			SplitResult.push_back(SubLeftBottom);
			SplitResult.push_back(SubRightTop);
		}
	}
	return SplitResult;
}

//************************************************************
//Function:
std::vector<glm::vec2> LBBMLGraphicsSoftShadowLighting::calculateSubLightPositions(const std::vector<glm::vec2>& vSubRectangles)
{
	float vSubRectangleWidth  = vSubRectangles[1].x - vSubRectangles[0].x;
	float vSubRectangleHeight = vSubRectangles[1].y - vSubRectangles[0].y;
	float DisturbParamiter = 0.2;

	std::vector<glm::vec2> SubLightsPositions;
	std::vector<glm::vec2>::const_iterator itr;
	for (itr = vSubRectangles.begin(); itr != vSubRectangles.end(); itr += 2)
	{
		glm::vec2 subLightPosition;
		subLightPosition.x = (itr->x + (itr+1)->x)/2 + vSubRectangleWidth  * LBBMLMathWorker::generateRandomf(-DisturbParamiter, DisturbParamiter);
		subLightPosition.y = (itr->y + (itr+1)->y)/2 + vSubRectangleHeight * LBBMLMathWorker::generateRandomf(-DisturbParamiter, DisturbParamiter);
		SubLightsPositions.push_back(subLightPosition);
	}
	return SubLightsPositions;
}

//************************************************************
//Function:
std::vector<glm::vec2> LBBMLGraphicsSoftShadowLighting::generateLightSamples2D(const glm::vec2& vLeftBottom, const glm::vec2& vRightTop, float vInterval)
{
	float RectangleWidth  = abs(vRightTop.x - vLeftBottom.x);
	float RectangleHeight = abs(vRightTop.y - vLeftBottom.y);
	int SplitNumberX = RectangleWidth / vInterval;
	int SplitNumberY = RectangleHeight / vInterval;

	float SampleIntervalX = RectangleWidth  / SplitNumberX;
	float SampleIntervalY = RectangleHeight / SplitNumberY;

	std::vector<glm::vec2> SubRectangles = splitRectangle(vLeftBottom, vRightTop, glm::vec2(SampleIntervalX, SampleIntervalY), glm::vec2(SplitNumberX, SplitNumberY));

	return calculateSubLightPositions(SubRectangles);
}

//************************************************************
//Function:
std::vector<glm::vec3> LBBMLGraphicsSoftShadowLighting::generateLightSamples3D(const glm::vec2& vLeftBottom, const glm::vec2& vRightTop, float vInterval)
{
	_ASSERT(vInterval > 0.0 ,"The sample interval must be greater than zero!");

	std::vector<glm::vec2> Coordinates2D;
	Coordinates2D = generateLightSamples2D(vLeftBottom, vRightTop, vInterval);

	std::vector<glm::vec3> Coordinates3D;
	std::vector<glm::vec2>::iterator itr;
	for (itr = Coordinates2D.begin(); itr != Coordinates2D.end(); ++itr)
	{
		glm::vec3 Coordinate(itr->x, itr->y, 0.0f);
		Coordinates3D.push_back(Coordinate);
	} 
	return Coordinates3D;
}

//******************************************************************
//FUNCTION:
std::vector<glm::vec3> LBBMLGraphicsSoftShadowLighting::generateLightSamples3DBySize(const glm::vec2& vLeftBottom, const glm::vec2& vRightTop, int vSize)
{
	float RectangleWidth  = abs(vRightTop.x - vLeftBottom.x);
	float RectangleHeight = abs(vRightTop.y - vLeftBottom.y);
	float EdgeRatio = RectangleWidth / RectangleHeight;

	int SplitNumberY = sqrt(vSize / EdgeRatio);
	int SplitNumberX = vSize / SplitNumberY; 

	if (SplitNumberY * SplitNumberX < vSize)
	{
		if (SplitNumberY < SplitNumberX)
		{
			++SplitNumberY;
		}
		else
			++SplitNumberX;
	}

	float SampleIntervalX = RectangleWidth  / SplitNumberX;
	float SampleIntervalY = RectangleHeight / SplitNumberY;

	std::vector<glm::vec2> SubRectangles = splitRectangle(vLeftBottom, vRightTop, glm::vec2(SampleIntervalX, SampleIntervalY), glm::vec2(SplitNumberX, SplitNumberY));

	std::vector<glm::vec2> SubLightPositions = calculateSubLightPositions(SubRectangles);
	std::random_shuffle(SubLightPositions.begin(),SubLightPositions.end());

	std::vector<glm::vec3> SubLightPositions3D;
	for (unsigned int i = 0; i < vSize; ++i)
	{
		SubLightPositions3D.push_back(glm::vec3(SubLightPositions[i].x, SubLightPositions[i].y, 0.0f));
	}
	return SubLightPositions3D;
}