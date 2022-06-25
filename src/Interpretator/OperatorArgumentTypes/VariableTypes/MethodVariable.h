#pragma once
#include "LinkToVariable.h"

class MethodVariable : public LinkToVariable {
protected:
	size_t m_variableIndex = 0;

	MethodVariable(VariableType methodVariableType,
		size_t variableIndex) :
		LinkToVariable(methodVariableType) {
		m_variableIndex = variableIndex;
	};

public:
	MethodVariable(size_t variableIndex)
		: LinkToVariable(VariableType::MethodVariableLinkType) {
		m_variableIndex = variableIndex;;
	}

	size_t GetVariableIndex() const {
		return m_variableIndex;
	}

	Variable* Clone() const {
		return new MethodVariable(m_variableIndex);
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};