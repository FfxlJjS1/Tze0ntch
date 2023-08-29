#pragma once
#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class Double final : public ElementaryVariable {
			private:
				double my_unsigned_byte = 0.0;

			public:
				Double() noexcept : ElementaryVariable(VariableType::DoubleType) {};

				Double(const double ubyte_var) noexcept : Double() {
					my_unsigned_byte = ubyte_var;
				}

				double get_double() noexcept {
					return my_unsigned_byte;
				}

				void set_double(const double ubyte_var) noexcept {
					my_unsigned_byte = ubyte_var;
				}

				Variable* clone() const override {
					return new Double(my_unsigned_byte);
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
