#pragma once
#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class Bool final : public ElementaryVariable {
			private:
				bool my_bool = false;

			public:
				Bool() noexcept : ElementaryVariable(VariableType::BoolType) {};

				Bool(const bool bool_var) noexcept : Bool() {
					my_bool = bool_var;
				};

				bool get_bool() const noexcept {
					return my_bool;
				}

				void set_bool(const bool bool_var) noexcept {
					my_bool = bool_var;
				}

				Variable* clone() const override {
					return new Bool(my_bool);
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
