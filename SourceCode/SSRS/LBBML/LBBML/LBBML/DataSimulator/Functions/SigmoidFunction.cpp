#include "SigmoidFunction.h"

CSigmoidFunction::CSigmoidFunction(void)
{
}

CSigmoidFunction::~CSigmoidFunction(void)
{
}

//******************************************************************
//FUNCTION:
float CSigmoidFunction::calculateFunctionValueV(float vX)
{
	return exp(vX)/(1+exp(vX));
}

//******************************************************************
//FUNCTION:
void CSigmoidFunction::printMyFormulaV()
{
	std::cout << "e^x/(1+e^x)";
}