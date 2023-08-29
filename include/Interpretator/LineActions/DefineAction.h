#pragma once
#include "LineAction.h"
#include "../ExpressionAndVariables/ElementaryVariableTypes/SpecificVariableTypes/LinkToVariable.h"

namespace Interpretator {
	namespace LineActions {
		class DefineAction final : public LineAction {
		private:
			LinkToVariable* my_set_to = nullptr;
			Variable* my_setting_var = nullptr;

		public:
			DefineAction() noexcept : LineAction(LineActionType::DefineSpecificVariableType) {};

			DefineAction(LinkToVariable* setTo, Variable* variable) noexcept
				: DefineAction() {
				my_set_to = setTo;
				my_setting_var = variable;
			};

			void set_set_to_type(LinkToVariable*& set_to) noexcept {
				my_set_to = set_to;
			}

			LinkToVariable*& get_set_to_type() noexcept {
				return my_set_to;
			}

			void set_var_link(Variable*& variable) noexcept {
				my_setting_var = variable;
			}

			Variable*& get_var_link() noexcept {
				return my_setting_var;
			}

			~DefineAction() {
				delete my_set_to;
				delete my_setting_var;
			}
		};
	}
}
