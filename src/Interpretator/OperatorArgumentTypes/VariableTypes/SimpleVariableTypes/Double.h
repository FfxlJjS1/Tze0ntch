#pragma once
#include "Variable.h"

class Double final : public Variable {
private:
	double m_unsignedByte = 0.0;

public:
	Double() noexcept  : Variable(VariableType::DoubleType) {};

	Double(const double variable) noexcept : Double() {
		m_unsignedByte = variable;
	}

	double GetDouble() noexcept {
		return m_unsignedByte;
	}

	void SetDouble(const double variable) noexcept {
		m_unsignedByte = variable;
	}

	Variable* Clone() const override {
		return new Double(m_unsignedByte);
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};
