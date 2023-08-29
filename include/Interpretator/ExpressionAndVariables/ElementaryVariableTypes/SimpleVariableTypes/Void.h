#pragma once
#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class Void final : public ElementaryVariable {
			public:
				Void() : ElementaryVariable(VariableType::VoidType) {};

				Variable* clone() const override {
					return new Void;
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
