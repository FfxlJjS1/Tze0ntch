#pragma once
#include <cstdint>

#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class Byte final : public ElementaryVariable {
			private:
				int8_t my_signed_byte = '\0';

			public:
				Byte() noexcept : ElementaryVariable(VariableType::ByteType) {};

				Byte(const int8_t signed_byte) : Byte() {
					my_signed_byte = signed_byte;
				}

				int8_t get_byte() noexcept {
					return my_signed_byte;
				}

				void set_byte(const int8_t signed_byte) noexcept {
					my_signed_byte = signed_byte;
				}

				Variable* clone() const override {
					return new Byte(my_signed_byte);
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
