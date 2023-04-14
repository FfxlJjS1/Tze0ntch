#pragma once

#include <filesystem>

#include "../DbController.h"

namespace DBMS {
	namespace CustomizableDbController {
		using std::ios_base, std::streampos;

		class CustomizableDbController : DbController final {
		public:
			void open_file(bool only_for_read = true, bool make_empty_file = false);


			bool is_open();


			void seek_to_point(streamposs point, ios_base point_relate_position);


			streamposs tell_point();


			void seek_to_g(streamposs point, ios_base);


			streamposs tell_g();


			string read_until_symbol(char symbol);


			void read(char** object, size_t size_of);

			void write(char** object, size_t size_of);


			void close_file();
		};
	}
}