#pragma once
#include "Variable.h"

class Bool final : public Variable {
private:
	bool m_unsignedInt = false;

public:
	Bool() noexcept : Variable(VariableType::BoolType) {};

	Bool(const bool variable) noexcept : Bool() {
		m_unsignedInt = variable;
	};

	bool GetBool() const noexcept {
		return m_unsignedInt;
	}

	void SetBool(const bool variable) noexcept {
		m_unsignedInt = variable;
	}

	Variable* Clone() const override {
		return new Bool(m_unsignedInt);
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};