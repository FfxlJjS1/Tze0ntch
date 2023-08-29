#pragma once
#include "ExpressionAction.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ExpressionTypes {
			class LogicalAction : public ExpressionAction {
			public:
				LogicalAction(VariableType logical_option_type) noexcept : ExpressionAction(logical_option_type) {};

				bool IsEqualVarStructures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
