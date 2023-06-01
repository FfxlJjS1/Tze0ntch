#include "../../include/DBMS/DBMS.h"

namespace DBMS {
	SystemDbController::SystemDbController* DBMS::get_sys_db_contrl() {
		SystemDbController::SystemDbController* sys_db_conrl = reinterpret_cast<SystemDbController::SystemDbController*>(my_db_controllers[1]);

		return sys_db_conrl;
	}


	DbController* DBMS::create_database_controller(dbIdType database_id, string database_name, dbStructTypes database_struct_type) {
		if (database_id == 0
			|| my_db_controllers.contains(database_id)
			|| database_name == ""
			|| database_struct_type == dbStructTypes::UndefinedDatabaseController) {
			return nullptr;
		}

		DbController* dbController = nullptr;

		if (database_struct_type == dbStructTypes::SystemDatabaseController && database_id == 1) {
			dbController = new SystemDbController::SystemDbController(database_name);
		}
		else if (database_struct_type == dbStructTypes::CustomizableDatabaseController) {
			dbController = new CustomizableDbController::CustomizableDbController(database_id, database_name);
		}
		else if (database_struct_type == dbStructTypes::KeyValueDatabaseController) {
			dbController = new KeyValueDBController::KeyValueDbController(database_id, database_name);
		}
		else {
			return nullptr;
		}

		my_db_controllers.insert({ database_id, dbController });

		return dbController;
	}
		

	DBMS::DBMS(string database_folder) {
		my_databases_folder = database_folder;

		my_db_controllers.insert({ 1, new SystemDbController::SystemDbController(my_databases_folder + "\\__system_db.mdd") });
	}


	void DBMS::create_database(string database_name, dbStructTypes database_struct_type) {
		string database_file_path = my_databases_folder + "\\" + database_name + ".mdd";

		switch (database_struct_type)
		{
		case SystemDbController::SystemDatabaseController:
			SystemDbController::SystemDbController::init_database_file(database_file_path);
			break;
		case SystemDbController::CustomizableDatabaseController:
			CustomizableDbController::CustomizableDbController::init_database_file(database_file_path);
			break;
		case SystemDbController::KeyValueDatabaseController:
			KeyValueDBController::KeyValueDbController::init_database_file(database_file_path);
			break;
		}
	}


	DbController* DBMS::get_database_controller(dbIdType database_id) {
		if (database_id <= 1) {
			return nullptr;
		}

		if (my_db_controllers.contains(database_id)) {
			return my_db_controllers[database_id];
		}
		else {
			auto sys_db_contrl = get_sys_db_contrl();

			if (!sys_db_contrl->is_exist_database_record(database_id)) {
				return nullptr;
			}

			SystemDbController::sys_db_record record = sys_db_contrl->get_database_record(database_id);

			DbController* new_db_controller = create_database_controller(
				database_id,
				record.db_name,
				record.db_struct_type
			);

			return new_db_controller;
		}
	}

	DbController* DBMS::get_database_controller(string database_name) {
		dbIdType database_id = get_sys_db_contrl()->get_database_id(database_name);

		return get_database_controller(database_id);
	}


	bool DBMS::include_database_into_sys_db(string database_name, dbStructTypes database_struct_type) {
		SystemDbController::SystemDbController* sys_db_conrl = reinterpret_cast<SystemDbController::SystemDbController*>(my_db_controllers[1]);

		if (sys_db_conrl->get_database_id(database_name) != 0) {
			return false;
		}

		sys_db_conrl->add_database_record(database_name, database_struct_type);

		return true;
	}


	void DBMS::exclude_database_from_sys_db(dbIdType database_id) {
		if (database_id <= 1) {
			return;
		}

		SystemDbController::SystemDbController* sys_db_conrl = reinterpret_cast<SystemDbController::SystemDbController*>(my_db_controllers[1]);

		sys_db_conrl->remove_database_record(database_id);
		
		delete my_db_controllers[database_id];
		my_db_controllers.erase(database_id);
	}

	void DBMS::exclude_database_from_sys_db(string database_name) {
		dbIdType database_id = get_sys_db_contrl()->get_database_id(database_name);

		if (database_id > 1) {
			exclude_database_from_sys_db(database_id);
		}
	}


	void DBMS::exclude_and_delete_database_file(dbIdType database_id) {
		if (database_id == 1) {
			return;
		}

		string database_file_name = get_sys_db_contrl()->get_database_name(database_id);

		exclude_database_from_sys_db(database_id);

		std::remove((my_databases_folder + "\\" + database_file_name + ".mdd").c_str());
	}

	void DBMS::exclude_and_delete_database_file(string database_name) {
		dbIdType database_id = get_sys_db_contrl()->get_database_id(database_name);

		if (database_id > 1) {
			exclude_database_from_sys_db(database_id);

			std::remove((my_databases_folder + "\\" + database_name + ".mdd").c_str());
		}
	}


	DBMS::~DBMS() {
		for (auto& pair : my_db_controllers) {
			delete pair.second;
		}
	}
}
