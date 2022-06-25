#pragma once
#include "Variable.h"

class ComparibleAction final : public Variable {
private:
	Variable* m_var1 = nullptr;
	Variable* m_var2 = nullptr;

public:
	ComparibleAction(VariableType comparibleType) noexcept : Variable(comparibleType) {};

	ComparibleAction(VariableType comparibleType,
		Variable* var1, Variable* var2) noexcept : ComparibleAction(comparibleType) {
		m_var1 = var1;
		m_var2 = var2;
	}

	void SetVar1(Variable* var1) noexcept {
		m_var1 = var1;
	}

	Variable*& GetVar1() noexcept {
		return m_var1;
	}
	
	void SetVar2(Variable* var2) noexcept {
		m_var2 = var2;
	}

	Variable*& GetVar2() noexcept {
		return m_var2;
	}

	Variable* Clone() const override {
		return new ComparibleAction(m_variableType, m_var1->Clone(), m_var2->Clone());
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		if (stVar->GetVariableType() != m_variableType) {
			return false;
		}

		ComparibleAction* compAct = static_cast<ComparibleAction*>(stVar);

		return m_var1->IsEqualVarStructures(compAct->GetVar1())
			&& m_var2->IsEqualVarStructures(compAct->GetVar2());
	}

	~ComparibleAction() {
		delete m_var1;
		delete m_var2;
	}
};