#pragma once
#include "ExpressionAction.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ExpressionTypes {
			class CreateComplexVar : public ExpressionAction {
			private:
				Variable* my_var_type_for_create = nullptr;

			public:
				CreateComplexVar() : ExpressionAction(VariableType::CreateComplexVarType) {}

				CreateComplexVar(Variable* var_for_create) : CreateComplexVar() {
					my_var_type_for_create = var_for_create;
				}

				Variable* get_var_type_for_create() {
					return my_var_type_for_create;
				}

				void set_var_type_for_create(Variable* var) {
					my_var_type_for_create = var;
				}

				Variable* clone() const override {
					return new CreateComplexVar(my_var_type_for_create->clone());
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return my_var_type_for_create->is_equal_var_structures(st_var);
				}

				~CreateComplexVar() {
					delete my_var_type_for_create;
				}
			};
		}
	}
}
