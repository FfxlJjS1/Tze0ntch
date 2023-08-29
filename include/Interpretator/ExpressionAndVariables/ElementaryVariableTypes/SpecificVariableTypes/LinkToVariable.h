#pragma once
#include "../ElementaryVariable.h"

namespace Interpretator {
	namespace ExpressionAndVariables {
		namespace ElementaryVariableTypes {
			class LinkToVariable : public ElementaryVariable {
			public:
				LinkToVariable(VariableType var_type) noexcept : ElementaryVariable(var_type) {};
			};
		}
	}
}
