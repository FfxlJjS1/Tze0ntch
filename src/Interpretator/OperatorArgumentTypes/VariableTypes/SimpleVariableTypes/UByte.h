#pragma once
#include <cstdint>

#include "Variable.h"

class UByte final : public Variable {
private:
	uint8_t m_unsignedByte = '\0';

public:
	UByte() noexcept : Variable(VariableType::UByteType){};

	UByte(const uint8_t unsignedByte) : UByte() {
		m_unsignedByte = unsignedByte;
	}

	uint8_t GetUByte() noexcept {
		return m_unsignedByte;
	}

	void SetUByte(const uint8_t unsignedByte) noexcept {
		m_unsignedByte = unsignedByte;
	}

	Variable* Clone() const override {
		return new UByte(m_unsignedByte);
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};
