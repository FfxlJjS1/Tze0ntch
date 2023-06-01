#include "../../../../include/DBMS/DbControllers/System/SystemDbController.h"

namespace DBMS {
	namespace SystemDbController {
		void SystemDbController::read_records_from_db_file() {
			my_db_records.clear();

			my_database_stream.seekg(0, ios_base::beg);

			while (!my_database_stream.eof()) {
				sys_db_record new_record = read_record_from_db_file();

				my_db_records.push_back(new_record);
			}
		}

		sys_db_record SystemDbController::read_record_from_db_file() {
			sys_db_record new_record;

			dbIdType database_id = 0;
			string database_name = "";
			dbStructTypes database_struct_type = UndefinedDatabaseController;

			
			my_database_stream.read((char*)&database_id, sizeof(dbIdType));
			
			std::getline(my_database_stream, database_name, '\0');

			my_database_stream.read((char*)&database_struct_type, sizeof(dbStructTypes));

			
			new_record.db_id = database_id;
			new_record.db_name = database_name;
			new_record.db_struct_type = database_struct_type;

			return new_record;
		}


		SystemDbController::SystemDbController(string database_file_path) : DbController(1, database_file_path) {
			open_database_file(ios_base::binary | ios::in | ios::out);

			read_records_from_db_file();
		}


		dbIdType SystemDbController::get_database_id(string database_name) const {
			for (sys_db_record record : my_db_records) {
				if (record.db_name == database_name) {
					return record.db_id;
				}
			}

			return 0;
		}

		string SystemDbController::get_database_name(dbIdType database_id) const {
			for (sys_db_record record : my_db_records) {
				if (record.db_id == database_id) {
					return record.db_name;
				}
			}

			return "";
		}

		dbStructTypes SystemDbController::get_database_struct_type(dbIdType database_id) const {
			for (sys_db_record record : my_db_records) {
				if (record.db_id == database_id) {
					return record.db_struct_type;
				}
			}

			return UndefinedDatabaseController;
		}

		sys_db_record SystemDbController::get_database_record(dbIdType database_id) const {
			for (sys_db_record record : my_db_records) {
				if (record.db_id == database_id) {
					return record;
				}
			}

			return sys_db_record();
		}


		bool SystemDbController::is_exist_database_record(dbIdType database_id) const {
			for (sys_db_record record : my_db_records) {
				if (record.db_id == database_id) {
					return true;
				}
			}

			return false;
		}

		bool SystemDbController::is_exist_database_record(string database_name) const {
			for (sys_db_record record : my_db_records) {
				if (record.db_name == database_name) {
					return true;
				}
			}

			return false;
		}


		bool SystemDbController::add_database_record(dbIdType database_id, string database_name, dbStructTypes database_struct_type) {
			for (sys_db_record record : my_db_records) {
				if (record.db_id == database_id || record.db_name == database_name) {
					return false;
				}
			}

			sys_db_record new_record = {
				database_id,
				database_name,
				database_struct_type
			};

			my_db_records.push_back(new_record);

			return true;
		}

		bool SystemDbController::add_database_record(string database_name, dbStructTypes database_struct_type) {
			dbIdType current_db_max_id = 1;
			
			for (sys_db_record record : my_db_records) {
				if (record.db_name == database_name) {
					return false;
				}
				if (current_db_max_id < record.db_id) {
					current_db_max_id = record.db_id;
				}
			}

			sys_db_record new_record = {
				current_db_max_id + 1,
				database_name,
				database_struct_type
			};

			my_db_records.push_back(new_record);

			return true;
		}


		void SystemDbController::remove_database_record(dbIdType database_id) {
			for (auto iterator = my_db_records.begin(); iterator != my_db_records.end(); iterator++) {
				if (iterator->db_id == database_id) {
					my_db_records.erase(iterator);
					
					return;
				}
			}
		}


		void SystemDbController::flush_cache() {
			close_database_file();

			open_database_file(ios::trunc | ios::in | ios::out || ios_base::binary);

			for (sys_db_record record : my_db_records) {
				dbIdType database_id = record.db_id;
				string database_name = record.db_name;
				dbStructTypes database_struct_type = record.db_struct_type;
				

				my_database_stream.write((char*)&database_id, sizeof(dbIdType));
				
				my_database_stream << database_name.c_str();
				
				my_database_stream.write((char*)&database_struct_type, sizeof(dbStructTypes));
			}

			my_database_stream.flush();
		}
	}
}
