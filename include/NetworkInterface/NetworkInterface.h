#pragma once
#include <iostream>
#include <string>

#include <boost/asio.hpp>

#include "../../libruaries/EncodingConvertorActions/EncodingConvertor.h"

namespace NetworkInterface {
	using std::cout, std::endl;
	using std::string;

	using boost::asio::buffer,
		boost::asio::streambuf,
		boost::asio::buffer_cast,
		boost::asio::ip::tcp,
		boost::asio::write,
		boost::asio::read_until;

	class NetworkInterface final {
	private:
		tcp::socket my_client_socket;

	public:
		NetworkInterface(boost::asio::io_service& io_service) : my_client_socket(io_service) {};

		tcp::socket& sock();

		bool is_open();

		void send_msg(const string& message);

		string receive_msg();

		void disconnect_client();

		~NetworkInterface() {
			if (my_client_socket.is_open()) {
				my_client_socket.close();
			}
		}
	};
}