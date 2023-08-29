#pragma once
#include ".GotoAction.h"

namespace Interpretator {
	namespace LineActions {
		class GotoActionWithIf final : public GotoAction {
		private:
			Variable* my_logical_var = nullptr;

		public:
			GotoActionWithIf() noexcept : GotoAction(LineActionType::GotoActionWithIfType) {}

			GotoActionWithIf(intptr_t go_to) noexcept : GotoActionWithIf() {
				my_go_to = go_to;
			};

			GotoActionWithIf(Variable* logical_var) : GotoActionWithIf() {
				my_logical_var = logical_var;
			}

			GotoActionWithIf(intptr_t go_to, Variable* logical_var) : GotoActionWithIf() {
				my_go_to = go_to;
				my_logical_var = logical_var;
			}

			void set_operation(Variable*& logical_var) noexcept {
				my_logical_var = logical_var;
			}

			Variable*& get_operation() noexcept {
				return my_logical_var;
			}

			~GotoActionWithIf() {
				delete my_logical_var;
			}
		};
	}
}
