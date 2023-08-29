#pragma once
#include <vector>

#include "ContainerTable.h"

#include "../../DBMS/DbControllers/Module/Module.h"

#include "CommonTypeDef.h"

namespace Interpretator {
	namespace Tables {
		using std::vector;

		using DBMS::ModuleDbController::objectIdType;

		class ModuleTable final : public ContainerTable {
		private:
			objectIdType my_module_id = 0;
			vector<objectIdType> my_need_modules; // TODO: Change to links to modules

		public:
			ModuleTable(objectIdType module_id, string module_name) : ContainerTable(module_name, ContainerTableType::ModuleTableType) {
				my_module_id = module_id;
			};

			objectIdType get_module_id() const {
				return my_module_id;
			}

			vector<objectIdType> get_need_modules() const {
				return my_need_modules;
			}

			void add_need_module(objectIdType module_id) {
				if (find(my_need_modules.begin(), my_need_modules.end(), module_id) == my_need_modules.end()) {
					my_need_modules.push_back(module_id);
				}
			}

			void RemoveNeedModule(objectIdType module_id) {
				auto iterator = find(my_need_modules.begin(), my_need_modules.end(), module_id);

				if (iterator != my_need_modules.end()) {
					my_need_modules.erase(iterator);
				}
			}

			bool operator==(const objectIdType& module_id) const {
				return my_module_id == module_id;
			}
		};
	}
}
