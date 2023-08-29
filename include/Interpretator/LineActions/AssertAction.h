#pragma once
#include "LineAction.h"
#include "../ExpressionAndVariables/ElementaryVariableTypes/SpecificVariableTypes/LinkToVariable.h"

namespace Interpretator {
	namespace LineActions {
		class AssertAction final : public LineAction {
		private:
			LinkToVariable* m_lvalue = nullptr;
			Variable* m_rvalue = nullptr;

		public:
			AssertAction() : LineAction(LineActionType::AssertActionType) {};

			AssertAction(LinkToVariable* lvalue,
				Variable* rvalue) : AssertAction() {
				m_lvalue = lvalue;
				m_rvalue = rvalue;
			}

			void set_lvalue(LinkToVariable*& lvalue) {
				m_lvalue = lvalue;
			}

			LinkToVariable*& get_lvalue() {
				return m_lvalue;
			}

			void set_rvalue(Variable* rvalue) {
				m_rvalue = rvalue;
			}

			Variable*& get_rvalue() {
				return m_rvalue;
			}

			~AssertAction() {
				delete m_lvalue;
				delete m_rvalue;
			}
		};
	}
}
