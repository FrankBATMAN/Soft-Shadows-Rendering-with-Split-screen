#pragma once
#include "BaseFunction.h"

class CSinFunction : public CFunctionBase
{
public:
	CSinFunction(void);
	virtual ~CSinFunction(void);

	float calculateFunctionValueV(float vX) override;
	void  printMyFormulaV() override;
};