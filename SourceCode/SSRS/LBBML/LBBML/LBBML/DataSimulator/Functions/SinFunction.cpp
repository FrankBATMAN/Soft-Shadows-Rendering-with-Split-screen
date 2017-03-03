#include "SinFunction.h"

CSinFunction::CSinFunction(void)
{
}

CSinFunction::~CSinFunction(void)
{
}

//******************************************************************
//FUNCTION:
float CSinFunction::calculateFunctionValueV(float vX)
{
	return sin(vX);
}

//******************************************************************
//FUNCTION:
void CSinFunction::printMyFormulaV()
{
	std::cout << "sin(x)";
}