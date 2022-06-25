#pragma once
#include "GotoAction.h"

class GotoActionWithIf final : public GotoAction {
private:
	Variable* m_logicalVar = nullptr;

public:
	GotoActionWithIf() noexcept : GotoAction(LineActionType::GotoActionWithIfType) {}

	GotoActionWithIf(intptr_t goTo) noexcept : GotoActionWithIf() {
		m_goTo = goTo;
	};

	GotoActionWithIf(Variable* logicalVar) : GotoActionWithIf() {
		m_logicalVar = logicalVar;
	}

	GotoActionWithIf(intptr_t goTo, Variable* logicalVar) : GotoActionWithIf() {
		m_goTo = goTo;
		m_logicalVar = logicalVar;
	}

	void SetOperation(Variable*& logicalVar) noexcept {
		m_logicalVar = logicalVar;
	}

	Variable*& GetOperation() noexcept {
		return m_logicalVar;
	}

	~GotoActionWithIf() {
		delete m_logicalVar;
	}
};
