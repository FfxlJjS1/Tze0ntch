#pragma once

#include <fstream>
#include <string>

class IniConfigurator {
private:
	const std::string my_ini_file_name = "configure.ini";
	std::fstream my_ini_stream;

public:
	IniConfigurator() {
		my_ini_stream.open(my_ini_file_name, std::ios_base::out || std::ios_base::in);


		// TODO: Create and full ini file if it isn't exist
		// TODO: Reading parametrs from ini file. Directory for DBMS, configuration for NetworkInterface
	}

	std::string get_dbms_directory_path();

	int get_network_interface_port();

	~IniConfigurator() {
		my_ini_stream.close();
	}
};