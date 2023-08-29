#pragma once
#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class Any : public ElementaryVariable {
			private:
				ElementaryVariable* my_any_var = nullptr;

			public:
				Any() : ElementaryVariable(VariableType::AnyType) {};

				Any(ElementaryVariable* any_var) : Any() {
					my_any_var = any_var;
				}

				void delete_any_var() {
					delete my_any_var;
				}

				ElementaryVariable* get_any_var() const {
					return my_any_var;
				}

				void set_any_var(ElementaryVariable* any_var) {
					my_any_var = any_var;
				}

				Variable* clone() const override {
					return new Any(my_any_var->clone());
				}

				bool is_equal_var_structures(ElementaryVariable* st_var) const override {
					return true;
				}

				~Any() {
					delete my_any_var;
				}
			};
		}
	}
}
