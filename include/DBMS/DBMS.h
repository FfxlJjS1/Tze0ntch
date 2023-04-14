#pragma once

#include <vector>
#include <string>

#include "DbControllers/DbController.h"

namespace DBMS {
	using std::string;
	using std::vector;

	class DBMS final {
	private:
		static vector<std::pair<dbIdType, string>> the_database_id_name;
		static vector<DbController> the_db_controllers;

		/*
		* Get id of the database has name as databaseName
		* Get it from system database but if it doesn't exist return 0
		*/
		dbIdType get_database_id(string database_name);

		string get_database_name(dbIdType database_id);

	public:
		void create_database(string database_name);


		DbController get_database_controller(string database_name);

		DbController get_database_controller(dbIdType database_id);


		void include_database(dbIdType database_id, string database_name);

		void include_database(string database_name);


		void exclude_database(dbIdType database_id);

		void exclude_database(string database_name);


		void delete_database(dbIdType database_id);

		void delete_database(string database_name);
	};
}
