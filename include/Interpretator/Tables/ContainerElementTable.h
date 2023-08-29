#pragma once

namespace Interpretator {
	namespace Tables {
		class ContainerTable;

		class ContainerElementTable {
		protected:
			ContainerTable* my_parent = nullptr;

		public:
			ContainerTable* get_parent() {
				return my_parent;
			}

			void set_parent(ContainerTable* parent) {
				my_parent = parent;
			}
		};
	}
}

#include "ContainerTable.h"
