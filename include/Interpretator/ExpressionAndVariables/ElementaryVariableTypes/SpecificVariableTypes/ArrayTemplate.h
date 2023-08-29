#pragma once
#include "../ElementaryVariable.h"

#include "NamedObject.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			// For define of method method return type
			class ArrayTemplate final : public ElementaryVariable {
			private:
				ElementaryVariable* my_object_template = nullptr;
				size_t my_array_size = 0; // If it zero array with undefined size

			public:
				ArrayTemplate() : ElementaryVariable(VariableType::ArrayTemplateType) {}

				ArrayTemplate(size_t array_size) : ArrayTemplate() {
					my_array_size = array_size;
				}

				ArrayTemplate(ElementaryVariable* object_template) : ArrayTemplate() {
					my_object_template = object_template;
				}

				ArrayTemplate(ElementaryVariable* object_template, size_t array_size) : ArrayTemplate() {
					my_object_template = object_template;
					my_array_size = array_size;
				}

				ElementaryVariable* get_object_template() {
					return my_object_template;
				}

				void set_object_template(ElementaryVariable* new_object_template) {
					my_object_template = new_object_template;
				}

				size_t get_array_size_var() {
					return my_array_size;
				}

				void set_array_size(size_t array_size) {
					my_array_size = array_size;
				}

				Variable* clone() const override {
					return new ArrayTemplate(my_object_template->clone(), my_array_size);
				}

				bool is_equal_var_structures(Variable* st_var) const override {
					if (st_var->get_var_type() != VariableType::ArrayType) {
						return false;
					}

					Array* st_arr = static_cast<Array*>(st_var);

					// Check data type equal
					if (my_object_template->get_var_type() == VariableType::NamedObjectType
						&& static_cast<NamedObject*>(my_object_template)->GetVariableName() != static_cast<Struct*>(st_arr->get_basic_var())->get_struct_name()) {
						return false;
					}
					else if (!my_object_template->is_equal_var_structures(st_arr->get_basic_var())) {
						return false;
					}

					// Array size
					if (my_array_size != 0 && st_arr->get_array_size_var() != my_array_size) {
						return false;
					}

					return true;
				}

				~ArrayTemplate() {
					delete my_object_template;
				}
			};
		}
	}
}
