#pragma once
#include <vector>

#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			using std::vector;

			class Array final : public ElementaryVariable {
			private:
				vector<Variable*> my_array;
				Variable* my_basic_var = nullptr;

			public:
				Array() noexcept : ElementaryVariable(VariableType::ArrayType) {};

				Array(Variable* basic_var) noexcept : Array() {
					my_basic_var = basic_var;
				};

				Array(Variable* basic_var, const size_t size) : Array() {
					make_new_array(basic_var, size);
				}

				void define_array(const size_t size) {
					Variable* basic_var = my_basic_var;
					my_basic_var = nullptr;

					make_new_array(basic_var, size);
				}

				void resize_array(const size_t new_size) {
					size_t old_count = my_array.size();

					my_array.resize(new_size);

					for (; old_count < new_size; old_count++) {
						my_array.at(old_count) = static_cast<Variable*>(my_basic_var->Clone());
					}
				}

				void set_array(vector<Variable*>& var_array, Variable* basic_Var) {
					my_array = var_array;
					my_basic_var = basic_Var;
				}

				void set_array_copy_from(const Array* from_array) {
					for (Variable* var : my_array) {
						delete var;
					}

					delete my_basic_var;

					my_basic_var = static_cast<Variable*>(from_array->get_basic_var()->clone());

					for (size_t index = 0; index < from_array->get_size(); index++) {
						my_array.push_back(from_array->get_copy_at(index));
					}
				}

				Variable* get_basic_var() const noexcept {
					return my_basic_var;
				}

				size_t get_size() const noexcept {
					return my_array.size();
				}

				vector<Variable*>& get_array() noexcept {
					return my_array;
				}

				size_t get_array_size_var() const {
					return my_array.size();
				}

				Variable* get_at(size_t index) {
					return my_array.at(index);
				}

				Variable* get_copy_at(const size_t index) const {
					return static_cast<Variable*>(my_array.at(index)->Clone());
				}

				void set_at(size_t index, Variable* var) {
					delete my_array.at(index);

					my_array.at(index) = var;
				}

				Variable* clone() const override {
					Array* clone = new Array();

					clone->set_array_copy_from(this);

					return clone;
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					if (st_var->get_var_type() != VariableType::ArrayType) {
						return false;
					}

					Array* var_ar = static_cast<Array*>(st_var);

					if (var_ar->my_array.size() != 0 && my_array.size() != 0) {
						if (var_ar->get_array_size_var() != my_array.size()) {
							return false;
						}

						for (size_t index = 0; index < my_array.size(); index++) {
							if (!my_array[index]->is_equal_var_structures(var_ar->get_at(index))) {
								return false;
							}
						}
					}

					return true;
				}

				~Array() {
					for (Variable* var : my_array) {
						delete var;
					}

					delete my_basic_var;
				}

			private:
				void make_new_array(Variable* basic_var, const size_t size) {
					for (Variable* var : my_array) {
						delete var;
					}

					delete my_basic_var;

					my_basic_ = basic_var;

					for (size_t index = 0; index < size; index++) {
						my_array.push_back(static_cast<Variable*>(basic_var->clone()));
					}
				}
			};
		}
	}
}
