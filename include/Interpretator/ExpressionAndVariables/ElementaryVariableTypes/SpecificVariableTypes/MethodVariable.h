#pragma once
#include "LinkToVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class MethodVariable : public LinkToVariable {
			protected:
				size_t my_var_index = 0;

				MethodVariable(VariableType method_var_type,
					size_t var_index) :
					LinkToVariable(method_var_type) {
					my_var_index = var_index;
				};

			public:
				MethodVariable(size_t var_index)
					: LinkToVariable(VariableType::MethodVariableLinkType) {
					my_var_index = var_index;
				}

				size_t get_var_index() const {
					return my_var_index;
				}

				Variable* clone() const {
					return new MethodVariable(my_var_index);
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
