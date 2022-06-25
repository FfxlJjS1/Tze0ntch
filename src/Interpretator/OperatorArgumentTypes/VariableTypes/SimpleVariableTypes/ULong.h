#pragma once
#include <cstdint>

#include "Variable.h"

class ULong final : public Variable {
private:
	uint64_t m_unsignedLong = 0;

public:
	ULong() : Variable(VariableType::ULongType) {}

	ULong(uint64_t unsignedLong) : ULong() {
		m_unsignedLong = unsignedLong;
	}

	uint64_t GetULong() {
		return m_unsignedLong;
	}

	void SetULong(uint64_t unsignedLong) {
		m_unsignedLong = unsignedLong;
	}

	Variable* Clone() const override {
		return new ULong(m_unsignedLong);
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};