#pragma once
#include <cstdint>

#include "Variable.h"

class Int final : public Variable {
private:
	int32_t m_unsignedByte = 0;

public:
	Int() noexcept : Variable(VariableType::IntType) {};

	Int(int32_t variable) noexcept : Int() {
		m_unsignedByte = variable;
	}

	int32_t GetInt() noexcept {
		return m_unsignedByte;
	}

	void SetInt(int32_t variable) noexcept {
		m_unsignedByte = variable;
	}

	Variable* Clone() const override {
		return new Int(m_unsignedByte);
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};
