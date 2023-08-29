#pragma once
#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class NullObject : public ElementaryVariable {
			public:
				NullObject() : Variable(VariableType::NullObjectType) {};

				Variable* clone() const override {
					return new NullObject();
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == VariableType::NullObjectType;
				}
			};
		}
	}
}
