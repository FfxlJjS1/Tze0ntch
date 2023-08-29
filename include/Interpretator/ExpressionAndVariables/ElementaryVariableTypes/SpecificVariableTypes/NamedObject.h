#pragma once
#include <string>

#include "LinkToVariable.h"

#include "../ComplexVariableTypes/Struct.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			using std::string;

			// For unoin struct name and array name
			class NamedObject : public LinkToVariable {
			protected:
				string my_var_name = "";

				NamedObject(VariableType module_var_type,
					string module_var_name)
					: LinkToVariable(module_var_type) {
					my_var_name = module_var_name;
				};

			public:
				NamedObject(string module_var_name)
					: LinkToVariable(VariableType::NamedObjectType) {
					my_var_name = module_var_name;
				}

				string get_var_name() const {
					return my_var_name;
				}

				void set_var_name(const string& name) {
					my_var_name = name;
				}

				Variable* clone() const override {
					return new NamedObject(my_var_name);
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					switch (st_var->get_var_type()) {
					case VariableType::StructType:
					{
						Struct* st_struct = static_cast<Struct*>(st_var);

						if (st_struct->get_struct_name() != "" && st_struct->get_struct_name() != my_var_name) {
							return false;
						}

						return true;
					}
					break;
					case VariableType::NamedObjectType:
						return static_cast<NamedObject*>(st_var)->get_var_name() == my_var_name;
						break;
					default:
						return false;
						break;
					}
				}
			};
		}
	}
}
