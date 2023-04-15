#pragma once

#include <vector>
#include <string>

#include "../DbController.h"

namespace DBMS {
	namespace SystemDbController {
		using std::string;
		using std::vector;
		

		enum dbStructTypes : char8_t {
			UndefinedDatabaseController = 0,
			SystemDatabaseController = 1,
			CustomizableDatabaseController = 2,
			KeyValueDatabaseController = 3,
			RelationalDatabaseController = 4
		};

		struct sys_db_record {
			dbIdType db_id = 0;
			string db_name = "";
			dbStructTypes db_struct_type = UndefinedDatabaseController;
		};

		class SystemDbController final : public DbController {
		private:
			vector<sys_db_record> my_db_records;


			void read_records_from_db_file();

			sys_db_record read_record_from_db_file();

		public:
			SystemDbController(string database_file_name);


			dbIdType get_database_id(string database_name) const;

			string get_database_name(dbIdType database_id) const;

			dbStructTypes get_database_struct_type(dbIdType database_id) const;

			sys_db_record get_database_record(dbIdType database_id) const;


			bool is_exist_database_record(dbIdType database_id) const;

			bool is_exist_database_record(string database_name) const;

			
			bool add_database_record(dbIdType database_id, string database_name, dbStructTypes database_struct_type);
			
			bool add_database_record(string database_name, dbStructTypes database_struct_type);


			void remove_database_record(dbIdType database_id);


			void flush_cache() override;


			~SystemDbController();
		};
	}
}
