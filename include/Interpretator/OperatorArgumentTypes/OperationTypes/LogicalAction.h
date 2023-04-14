#pragma once
#include "Variable.h"

class LogicalAction : public Variable {
public:
	LogicalAction(VariableType logicalOptionType) noexcept : Variable(logicalOptionType) {};

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};
