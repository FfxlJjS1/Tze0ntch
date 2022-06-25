#pragma once
#include <cstdint>

#include "Variable.h"

class UInt final : public Variable {
private:
	uint32_t m_unsignedByte = 0;

public:
	UInt() noexcept : Variable(VariableType::UIntType) {};

	UInt(uint32_t variable) noexcept : UInt() {
		m_unsignedByte = variable;
	}
	
	uint32_t GetUInt() noexcept {
		return m_unsignedByte;
	}

	void SetUInt(uint32_t variable) noexcept {
		m_unsignedByte = variable;
	}

	Variable* Clone() const override {
		return new UInt(m_unsignedByte);
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};