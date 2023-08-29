#include "DBMS/DbControllers/Module/ModuleDbController.h"

namespace DBMS {
	namespace ModuleDbController {
		ModuleDbController::ModuleDbController(const dbIdType database_id, const string& database_file_name) : DbController(database_id, database_file_name) {}

		Module ModuleDbController::read_module(const objectIdType module_id_for_read) {
			vector<Module> modules = read_modules(vector<objectIdType> {module_id_for_read});
			Module read_module;

			if (modules.size() > 0) {
				read_module = modules[0];
			}

			return read_module;
		}



		Representing ModuleDbController::read_representing(const objectIdType representing_id) {
			vector<Representing> representings = read_representings(vector<objectIdType> {representing_id});
			Representing read_representing;

			if (representings.size() > 0) {
				read_representing = representings[0];
			}

			return read_representing;
		}
	}
}

