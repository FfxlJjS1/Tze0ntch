#include "DBMS/DbControllers/Module/Module.h"

namespace DBMS {
	namespace ModuleDbController {
		Module::Module() {}

		Module::Module(const objectIdType id, const std::string name, const std::string code)
			: my_id(id), my_name(name), my_code(code) {}


		Module::operator==(const Module& module) const {
			return my_id == module.my_id
				&& my_name == module.my_name
				&& my_code == module.my_code;
		}

		Module::operator==(const objectIdType module_id) const {
			return my_id == module_id;
		}

		Module::operator==(const std::string& name) const {
			return my_name == name;
		}
	}
}

