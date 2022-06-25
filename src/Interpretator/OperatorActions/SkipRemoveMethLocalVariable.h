#pragma once
#include "LineAction.h"

class SkipRemoveMethLocalVariable final : public LineAction {
public:
	SkipRemoveMethLocalVariable() : LineAction(LineActionType::SkipRemoveMethLocalVariableType) {}
};