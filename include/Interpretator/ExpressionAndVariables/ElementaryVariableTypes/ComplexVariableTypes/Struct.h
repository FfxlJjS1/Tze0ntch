#pragma once
#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class Struct final : public ElementaryVariable {
			private:
				string my_struct_name = "";
				vector<Variable*> my_vars;

			public:
				Struct(const string& struct_name) : ElementaryVariable(VariableType::StructType) {
					my_struct_name = struct_name;
				};

				Struct(const string& struct_name, vector<Variable*>& vars)
					: Struct(struct_name) {
					my_vars = vars;
				}

				string get_struct_name() const {
					return my_struct_name;
				}

				void set_struct_name(string struct_name) {
					my_struct_name = struct_name;
				}

				void add_var_to_back(Variable* var) {
					my_vars.push_back(var);
				}

				vector<Variable*>& get_variables() {
					return my_vars;
				}

				size_t get_variable_count() const {
					return my_vars.size();
				}

				Variable*& get_at(const size_t index) {
					return my_vars.at(index);
				}

				Variable* get_copy_at(const size_t index) const {
					return my_vars.at(index)->Clone();
				}

				void set_struct_from(Struct* from_struct) {
					my_struct_name = from_struct->get_struct_name();

					for (Variable* var : my_vars) {
						delete var;
					}

					my_vars.clear();

					for (size_t index = 0; index < from_struct->get_variable_count(); index++) {
						my_vars.push_back(from_struct->get_at(index));
					}
				}

				void set_struct_copy_from(const Struct* from_struct) {
					my_struct_name = from_struct->Getstruct_name();

					for (Variable* var : my_vars) {
						delete var;
					}

					my_vars.clear();

					for (size_t index = 0; index < from_struct->get_variable_count(); index++) {
						my_vars.push_back(from_struct->get_copy_at(index));
					}
				}

				Variable* clone() const override {
					Struct* clone = new Struct(my_struct_name);

					for (Variable* var : my_vars) {
						clone->add_var_to_back(static_cast<Variable*>(var->clone()));
					}

					return clone;
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					if (st_var->get_var_type() != VariableType::StructType) {
						return false;
					}

					Struct* var_ar = static_cast<Struct*>(st_var);

					if (my_struct_name == "") {
						if (var_ar->get_variable_count() != my_vars.size()) {
							return false;
						}

						for (size_t index = 0; index < my_vars.size(); index++) {
							if (!my_vars[index]->is_equal_var_structures(var_ar->get_at(index))) {
								return false;
							}
						}
					}
					else if (var_ar->get_struct_name() != my_struct_name) {
						return false;
					}
					/* For entity equal between structs. Not using because it not helped method with argument and parametr where they is stuct but argument with name and arguments and parametr with only name of structur without arguments
					if (varAr->GetVariableCount() != my_vars.size()) {
						return false;
					}

					for (size_t index = 0; index < my_vars.size(); index++) {
						if (!my_vars[index]->IsEqualVarStructures(varAr->GetAt(index))) {
							return false;
						}
					}
					*/

					return true;
				}

				~Struct() {
					if (my_struct_name != "") {
						for (Variable* var : my_vars) {
							delete var;
						}
					}
				}
			};
		}
	}
}
