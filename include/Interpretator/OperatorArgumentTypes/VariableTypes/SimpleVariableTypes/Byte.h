#pragma once
#include <cstdint>

#include "Variable.h"

class Byte final : public Variable {
private:
	int8_t m_signedByte = '\0';

public:
	Byte() noexcept : Variable(VariableType::ByteType) {};

	Byte(const int8_t signedByte) : Byte() {
		m_signedByte = signedByte;
	}

	int8_t GetByte() noexcept {
		return m_signedByte;
	}

	void SetByte(const int8_t signedByte) noexcept {
		m_signedByte = signedByte;
	}

	Variable* Clone() const override {
		return new Byte(m_signedByte);
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};
