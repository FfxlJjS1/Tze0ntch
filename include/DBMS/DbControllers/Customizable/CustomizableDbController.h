#pragma once

#include "../DbController.h"

namespace DBMS {
	namespace CustomizableDbController {
		using std::streampos;

		class CustomizableDbController final : public DbController {
		public:
			CustomizableDbController(dbIdType database_id, std::string database_file_path);


			static void init_database_file(std::string database_file_path);


			void open_file(bool only_for_read = true, bool make_empty_file = false);


			bool is_open();


			void seek_to_put(streampos point, ios_base::seekdir point_relate_position);

			streampos tell_put();


			void seek_to_get(streampos point, ios_base::seekdir point_relate_position);

			streampos tell_get();


			std::string read_until_symbol(char symbol);


			void read(char** object, size_t size_of);

			void write(char** object, size_t size_of);


			void close_file();


			void flush_cache() override;
		};
	}
}