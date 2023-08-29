#pragma once

#include <string>

#include "Representing.h"

namespace DBMS {
	namespace ModuleDbController {
		struct Module {
			objectIdType my_id = 0;
			mutable std::string my_name = "";
			mutable std::string my_code = "";


			Module();

			Module(const objectIdType id, const std::string name, const std::string code);


			bool operator==(const Module& module) const;

			bool operator==(const objectIdType module_id) const;

			bool operator==(const std::string& name) const;

			friend std::ostream& operator<<(std::ostream& ostream, const Module& module);
			
			friend std::istream& operator>>(std::istream& istream, Module& module);
		};
	}
}