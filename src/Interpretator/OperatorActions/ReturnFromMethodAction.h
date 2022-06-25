#pragma once
#include "LineAction.h"

class ReturnFromMethodAction final : public LineAction {
public:
	ReturnFromMethodAction() : LineAction(LineActionType::ReturnFromMethodType) {};
};
