#pragma once
#include "basefunction.h"

class CSigmoidFunction : public CFunctionBase
{
public:
	CSigmoidFunction(void);
	virtual ~CSigmoidFunction(void);

	float calculateFunctionValueV(float vX) override;
	void  printMyFormulaV() override;
};