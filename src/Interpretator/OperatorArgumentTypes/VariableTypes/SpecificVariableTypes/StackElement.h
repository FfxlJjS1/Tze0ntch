#pragma once
#include "LinkToVariable.h"

class StackElement final : public LinkToVariable {
public:
	StackElement() : LinkToVariable(VariableType::StackElementType) {};

	Variable* Clone() const {
		return new StackElement();
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};