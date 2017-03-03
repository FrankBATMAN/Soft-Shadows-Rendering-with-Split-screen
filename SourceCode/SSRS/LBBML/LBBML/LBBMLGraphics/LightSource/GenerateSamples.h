#pragma once
#include <vector>
#include <time.h>
#include <GLM/glm.hpp>
#include <boost/random.hpp>
#include <LBBML_ROOT/include/LBBML/MathWorker/LBBMLMathWorker.h>

namespace LBBMLGraphicsSoftShadowLighting
{
	std::vector<glm::vec2> splitRectangle(const glm::vec2& vLeftBottom, const glm::vec2& vRightTop, const glm::vec2& vSampleInterval, const glm::vec2& vSplitNumberY);
	std::vector<glm::vec2> calculateSubLightPositions(const std::vector<glm::vec2>& vSubRectangles);
	std::vector<glm::vec2> generateLightSamples2D(const glm::vec2& vLeftBottom, const glm::vec2& vRightTop, float vInterval);
	std::vector<glm::vec3> generateLightSamples3D(const glm::vec2& vLeftBottom, const glm::vec2& vRightTop, float vInterval);
	std::vector<glm::vec3> generateLightSamples3DBySize(const glm::vec2& vLeftBottom, const glm::vec2& vRightTop, int vSize);
};