#pragma once

#include <vector>
#include <map>
#include <string>
#include <filesystem>

#include "DbControllers/DbController.h"
#include "DbControllers/System/SystemDbController.h"
#include "DbControllers/Customizable/CustomizableDbController.h"
#include "DbControllers/KeyValue/KeyValueDbController.h"

namespace DBMS {
	using std::string;
	using std::vector;
	using SystemDbController::dbStructTypes;

	class DBMS final {
	private:
		vector<std::pair<dbIdType, string>> my_database_id_name;
		std::map<dbIdType, DbController*> my_db_controllers;
		string my_databases_folder;

		/*
		* Get id of the database has name as databaseName
		* Get it from system database but if it doesn't exist return 0
		*/
		SystemDbController::SystemDbController* get_sys_db_contrl();
		

		DbController* create_database_controller(dbIdType database_id, string database_name, dbStructTypes database_struct_type);

	public:
		DBMS(string databases_folder);


		void create_database(string database_name, dbStructTypes database_struct_type);


		/*
		* Cannot return system database controller (id 1), it'll return nullptr
		* If database controller doesn't created will create new db controller for such database structure type
		*/
		DbController* get_database_controller(dbIdType database_id);

		/*
		* Cannot return system database controller (id 1), it'll return nullptr
		* If database controller doesn't created will create new db controller for such database structure type
		*/
		DbController* get_database_controller(string database_name);
		

		bool include_database_into_sys_db(string database_file_name, dbStructTypes database_struct_type);


		void exclude_database_from_sys_db(dbIdType database_id);

		void exclude_database_from_sys_db(string database_name);


		void exclude_and_delete_database_file(dbIdType database_id);

		void exclude_and_delete_database_file(string database_name);


		~DBMS();
	};
}
