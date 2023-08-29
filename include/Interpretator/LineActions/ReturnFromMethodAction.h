#pragma once
#include "LineAction.h"

namespace Interpretator {
	namespace LineActions {
		class ReturnFromMethodAction final : public LineAction {
		public:
			ReturnFromMethodAction() : LineAction(LineActionType::ReturnFromMethodType) {};
		};
	}
}
