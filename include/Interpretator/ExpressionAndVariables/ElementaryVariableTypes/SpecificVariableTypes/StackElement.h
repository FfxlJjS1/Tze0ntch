#pragma once
#include "LinkToVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class StackElement final : public LinkToVariable {
			public:
				StackElement() : LinkToVariable(VariableType::StackElementType) {};

				Variable* clone() const override {
					return new StackElement();
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
