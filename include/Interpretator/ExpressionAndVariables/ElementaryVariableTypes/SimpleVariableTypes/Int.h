#pragma once
#include <cstdint>

#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class Int final : public ElementaryVariable {
			private:
				int32_t my_signed_int32 = 0;

			public:
				Int() noexcept : ElementaryVariable(VariableType::IntType) {};

				Int(int32_t signed_int32_var) noexcept : Int() {
					my_signed_int32 = signed_int32_var;
				}

				int32_t get_int() noexcept {
					return my_signed_int32;
				}

				void set_int(int32_t signed_int32_var) noexcept {
					my_signed_int32 = signed_int32_var;
				}

				Variable* clone() const override {
					return new Int(my_signed_int32);
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
