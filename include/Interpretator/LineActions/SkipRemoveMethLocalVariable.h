#pragma once
#include "LineAction.h"

namespace Interpretator {
	namespace LineActions {
		class SkipRemoveMethLocalVariable final : public LineAction {
		public:
			SkipRemoveMethLocalVariable() : LineAction(LineActionType::SkipRemoveMethLocalVariableType) {}
		};
	}
}
