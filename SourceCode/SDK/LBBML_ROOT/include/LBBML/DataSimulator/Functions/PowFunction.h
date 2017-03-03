#pragma once
#include "BaseFunction.h"

class CPowFunction : public CFunctionBase
{
public:
	CPowFunction(void);
	virtual ~CPowFunction(void);

	float calculateFunctionValueV(float vX) override;
	void printMyFormulaV() override;

private:
	float m_PowerNumber;
};

