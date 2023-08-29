#pragma once
#include "LineAction.h"
#include "../ExpressionAndVariables/ElementaryVariableTypes/SpecificVariableTypes/GoToMethod.h"

namespace Interpretator {
	namespace LineActions {
		class GoToMethodEmpty final : public LineAction {
		private:
			Variable* my_go_to_method = nullptr;

		public:
			GoToMethodEmpty() noexcept : LineAction(LineActionType::GoToMethodEmptyType) {};

			GoToMethodEmpty(GoToMethod*& go_to_method) noexcept
				: GoToMethodEmpty() {
				my_go_to_method = go_to_method;
			}

			void set_go_to_method(Variable*& go_to_method) noexcept {
				my_go_to_method = go_to_method;
			}

			Variable*& get_go_to_method() noexcept {
				return my_go_to_method;
			}

			~GoToMethodEmpty() {
				delete my_go_to_method;
			}
		};
	}
}
