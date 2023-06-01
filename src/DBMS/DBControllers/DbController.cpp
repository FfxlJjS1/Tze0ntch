#include "../../../include/DBMS/DbControllers/DbController.h"

namespace DBMS {
	DbController::DbController(const dbIdType database_id, const std::string database_name) {
		my_datase_id = database_id;
		my_database_file_name = database_name;
	}


	void DbController::open_database_file(ios_base::openmode ios_base_open_mode) {
		my_database_stream.open(my_database_file_name, ios_base_open_mode);
	}


	void DbController::close_database_file() {
		my_database_stream.close();
	}


	dbIdType DbController::get_database_id() const {
		return my_datase_id;
	}


	DbController::~DbController() {
		if (my_database_stream.is_open()) {
			close_database_file();
		}
	}
}
