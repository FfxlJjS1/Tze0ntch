#pragma once
#include <string>

#include "../ExpressionAndVariables/ElementaryVariableTypes/SpecificVariableTypes/LinkToVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			using std::string;

			class ContainerVariable final : public LinkToVariable {
			private:
				string my_name = "";
				Variable* my_var = nullptr;

			public:
				ContainerVariable() : LinkToVariable(VariableType::NamedVariableType) {}

				ContainerVariable(string name, Variable* variable) : ContainerVariable() {
					my_name = name;
					my_var = variable;
				}

				void set_name(string name) {
					my_name = name;
				}

				string get_name() {
					return my_name;
				}

				void set_var(Variable* variable) {
					my_var = variable;
				}

				Variable* get_var() {
					return my_var;
				}

				Variable* clone() const override {
					return new ContainerVariable(my_name, my_var->clone());
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					if (st_var->get_var_type() != VariableType::NamedVariableType) {
						return false;
					}

					ContainerVariable* cont_var = static_cast<ContainerVariable*>(st_var);

					if (my_name != cont_var->get_name()
						|| !my_var->is_equal_var_structures(cont_var->get_var())) {
						return false;
					}

					return true;
				}

				bool operator==(string name) {
					return my_name == name;
				}

				~ContainerVariable() {
					delete my_var;
				}
			};
		}
	}
}
