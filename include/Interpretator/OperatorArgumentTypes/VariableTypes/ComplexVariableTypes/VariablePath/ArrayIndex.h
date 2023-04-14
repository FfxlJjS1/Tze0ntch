#pragma once
#include "LinkToVariable.h"

class ArrayIndex final : public LinkToVariable {
private:
	Variable* m_indexVar = nullptr;

public:
	ArrayIndex() noexcept : LinkToVariable(VariableType::ArrayIndexType) {};

	ArrayIndex(Variable* indexVar) : ArrayIndex() {
		m_indexVar = indexVar;
	}

	Variable* GetIndex() noexcept {
		return m_indexVar;
	}

	void SetIndex(Variable* indexVar) noexcept {
		m_indexVar = indexVar;
	}

	Variable* Clone() const override {
		return new ArrayIndex(m_indexVar->Clone());
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType;
	}

	~ArrayIndex() {
		delete m_indexVar;
	}
};