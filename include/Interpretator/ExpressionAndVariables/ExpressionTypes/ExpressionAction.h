#pragma once
#include "../Variable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ExpressionTypes {
			class ExpressionAction : public Variable {
			public:
				ExpressionAction(VariableType var_type) : Variable(var_type) {};
			};
		}
	}
}
