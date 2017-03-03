#pragma once

#include <vector>
#include <GLM/vec4.hpp>
#include "../Common/LBBML_INTERFACE_EXPORTS.h"

namespace LBBMLMathWorker
{
	LBBML_DLL_EXPORTS int   generateRandomi(int vMin, int vMax);
	LBBML_DLL_EXPORTS float generateRandomf(float vMin, float vMax);

	LBBML_DLL_EXPORTS std::vector<int>		 generateRandomVectori(unsigned int vSize, int vMin, int vMax);
	LBBML_DLL_EXPORTS std::vector<float>	 generateRandomVectorf(unsigned int vSize, float vMin, float vMax);
	LBBML_DLL_EXPORTS std::vector<glm::vec4> splitRectangle(int vSplitNumberX, int vSplitNumberY, int vWinWidth, int vWinHeight);
}