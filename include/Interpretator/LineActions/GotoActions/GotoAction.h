#pragma once
#include <cstdint>

#include "../LineAction.h"

namespace Interpretator {
	namespace LineActions {
		class GotoAction : public LineAction {
		protected:
			intptr_t my_go_to = 0;

			GotoAction(LineActionType line_act_Type) : LineAction(line_act_Type) {};

		public:
			GotoAction() : LineAction(LineActionType::GotoActionType) {};

			GotoAction(intptr_t go_to) : GotoAction() {
				my_go_to = go_to;
			}

			intptr_t get_ptr_diff() noexcept {
				return my_go_to;
			}

			void set_ptr_diff(intptr_t go_to) noexcept {
				my_go_to = go_to;
			}
		};
	}
}