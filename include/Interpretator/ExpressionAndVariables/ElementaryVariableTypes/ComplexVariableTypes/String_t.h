#pragma once
#include <string>

#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			using std::string;

			class String_t final : public ElementaryVariable {
			private:
				string my_string = "";

			public:
				String_t() : ElementaryVariable(VariableType::StringType) {};

				String_t(const string& variable) : String_t() {
					my_string = variable;
				}

				string get_string() {
					return my_string;
				}

				void set_string(const string& variable) {
					my_string = variable;
				}

				Variable* clone() const override {
					return new String_t(my_string);
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					return st_var->get_var_type() == my_var_type;
				}
			};
		}
	}
}
