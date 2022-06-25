#pragma once
#include "LineAction.h"
#include "GoToMethod.h"

class GoToMethodEmpty final : public LineAction {
private:
	Variable* m_goToMethod = nullptr;

public:
	GoToMethodEmpty() noexcept : LineAction(LineActionType::GoToMethodEmptyType) {};

	GoToMethodEmpty(GoToMethod*& goToMethod) noexcept
		: GoToMethodEmpty() {
		m_goToMethod = goToMethod;
	}

	void SetGoToMethod(Variable*& goToMethod) noexcept {
		m_goToMethod = goToMethod;
	}

	Variable*& GetGoToMethod() noexcept {
		return m_goToMethod;
	}

	~GoToMethodEmpty() {
		delete m_goToMethod;
	}
};