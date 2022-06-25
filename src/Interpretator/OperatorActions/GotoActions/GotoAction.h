#pragma once
#include <cstdint>

#include "LineAction.h"

class GotoAction : public LineAction {
protected:
	intptr_t m_goTo = 0;

	GotoAction(LineActionType lineActType) : LineAction(lineActType) {};

public:
	GotoAction() : LineAction(LineActionType::GotoActionType) {};

	GotoAction(intptr_t goTo) : GotoAction() {
		m_goTo = goTo;
	}

	intptr_t GetPtrDiff() noexcept {
		return m_goTo;
	}

	void SetPtrDiff(intptr_t goTo) noexcept {
		m_goTo = goTo;
	}
};