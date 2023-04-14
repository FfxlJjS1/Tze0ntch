#pragma once

#include <fstream>
#include <utility>

namespace DBMS {
	typedef uint16_t dbIdType;

	class DbController {
	protected:
		dbIdType my_datase_id;

		std::fstream my_database_stream;

		DatabaseCache* my_database_cache;


		void open_database();
		
		void close_database();

	public:
		dbIdType get_database_id();


		void flush_chache(bool write_to_file);
	};
}
