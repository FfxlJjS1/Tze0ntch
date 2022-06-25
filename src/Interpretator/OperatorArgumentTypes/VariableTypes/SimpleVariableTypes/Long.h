#pragma once
#include <cstdint>

#include "Variable.h"

class Long final : public Variable {
private:
	int64_t m_signedLong = 0;

public:
	Long() : Variable(VariableType::LongType) {}

	Long(int64_t signedLong) : Long() {
		m_signedLong = signedLong;
	}

	int64_t GetLong() {
		return m_signedLong;
	}

	void SetLong(int64_t signedLong) {
		m_signedLong = signedLong;
	}

	Variable* Clone() const override {
		return new Long(m_signedLong);
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}
};