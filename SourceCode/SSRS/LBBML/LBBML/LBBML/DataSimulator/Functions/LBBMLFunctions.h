#pragma once
#include <vector>
#include "CosFunction.h"
#include "SinFunction.h"
#include "PowFunction.h"
#include "LogFunction.h"
#include "LinearFunction.h"
#include "SigmoidFunction.h"

class CLBBMLFunctions
{
public:
	CLBBMLFunctions(void);
	virtual ~CLBBMLFunctions(void);

	CFunctionBase* getFunctionByIndex(int vIndex);

private:
	void __initialise();

	std::vector<CFunctionBase*> m_FunctionDatabase;
};