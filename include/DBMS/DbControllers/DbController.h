#pragma once

#include <fstream>
#include <utility>

#include "System/SystemDbController.h"

namespace DBMS {
	using std::ios;
	using std::ios_base;
	using std::streampos;
	typedef uint16_t dbIdType;

	enum dbStructTypes : char8_t {
		UndefinedDatabaseController = 0,
		SystemDatabaseController = 1,
		ModuleDatabaseController = 2,
		CustomizableDatabaseController = 3,
		SemanticWebWithIndexingDbController = 4,
		RelationalDatabaseController = 5
	};

	class DbController {
	protected:
		dbIdType my_datase_id;

		std::string my_database_file_name;
		std::fstream my_database_stream;


		void open_database_file(ios_base::openmode ios_base_open_mode);
		
		void close_database_file();

	public:
		DbController(const dbIdType database_id, const std::string& database_name);


		dbIdType get_database_id() const;


		virtual void flush_cache() = 0;
	};
}
