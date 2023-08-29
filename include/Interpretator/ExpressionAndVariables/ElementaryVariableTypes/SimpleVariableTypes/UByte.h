#pragma once
#include <cstdint>

#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class UByte final : public ElementaryVariable {
			private:
				uint8_t my_unsigned_byte = '\0';

			public:
				UByte() noexcept : ElementaryVariable(VariableType::UByteType) {};

				UByte(const uint8_t unsigned_byte) : UByte() {
					my_unsigned_byte = unsigned_byte;
				}

				uint8_t get_ubyte() noexcept {
					return my_unsigned_byte;
				}

				void set_ubyte(const uint8_t unsigned_byte) noexcept {
					my_unsigned_byte = unsigned_byte;
				}

				Variable* clone() const override {
					return new UByte(my_unsigned_byte);
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
