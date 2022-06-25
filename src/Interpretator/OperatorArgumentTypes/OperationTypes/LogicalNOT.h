#pragma once
#include "LogicalAction.h"

class LogicalNOT final : public LogicalAction {
private:
	Variable* m_logicalVar = nullptr;

public:
	LogicalNOT(Variable* logicalVariable)
		: LogicalAction(VariableType::NotLogicalActionType) {
		m_logicalVar = logicalVariable;
	};

	void SetLogicalVar(Variable*& logVar) noexcept {
		m_logicalVar = logVar;
	}

	Variable* GetLogicalVar() const noexcept {
		return m_logicalVar;
	}

	Variable* Clone() const override {
		return new LogicalNOT(m_logicalVar->Clone());
	}

	bool IsEqualVarStructures(Variable* stVar) const override {
		return stVar->GetVariableType() == m_variableType
			&& m_logicalVar->IsEqualVarStructures(static_cast<LogicalNOT*>(stVar)->GetLogicalVar());
	}

	~LogicalNOT() {
		delete m_logicalVar;
	}
};
