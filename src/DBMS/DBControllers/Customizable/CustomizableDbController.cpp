#include "../../../../include/DBMS/DbControllers/Customizable/CustomizableDbController.h"

namespace DBMS {
	namespace CustomizableDbController {
		CustomizableDbController::CustomizableDbController(dbIdType database_id, std::string database_name) : DbController(database_id, database_name) {}


		void CustomizableDbController::open_file(bool only_for_read = true, bool make_empty_file = false) {
			ios_base::openmode open_mode = ios::in | ios_base::binary;

			if (!only_for_read) {
				open_mode |= ios::out;
			}

			if (make_empty_file) {
				open_mode |= ios_base::trunc;
			}

			my_database_stream.open(my_database_file_name, open_mode);
		}


		bool CustomizableDbController::is_open() {
			return my_database_stream.is_open();
		}


		void CustomizableDbController::seek_to_put(streampos point, ios_base::seekdir point_relate_position) {
			my_database_stream.seekp(point, point_relate_position);
		}
		
		streampos CustomizableDbController::tell_put() {
			return my_database_stream.tellp();
		}


		void CustomizableDbController::seek_to_get(streampos point, ios_base::seekdir point_relate_position) {
			my_database_stream.seekg(point, point_relate_position);
		}

		streampos CustomizableDbController::tell_get() {
			return my_database_stream.tellg();
		}


		std::string CustomizableDbController::read_until_symbol(char symbol) {
			std::string text = "";

			std::getline(my_database_stream, text, symbol);

			return text;
		}


		void CustomizableDbController::read(char** object, size_t size_of) {
			my_database_stream.read(*object, size_of);
		}

		void CustomizableDbController::write(char** object, size_t size_of) {
			my_database_stream.write(*object, size_of);
		}


		void CustomizableDbController::close_file() {
			my_database_stream.close();
		}


		CustomizableDbController::~CustomizableDbController() {
			my_database_stream.close();
		}
	}
}
