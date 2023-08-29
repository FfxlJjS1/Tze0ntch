#pragma once
#include "../Variable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class ElementaryVariable : public Variable {
			public:
				ElementaryVariable(VariableType var_type) : Variable(var_type) {};
			}
		}
	}
}
