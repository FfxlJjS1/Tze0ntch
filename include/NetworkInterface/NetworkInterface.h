#pragma once
#include <iostream>
#include <string>

#include <boost/asio.hpp>

#include "EncodingConvertor.h"

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
		tcp::socket m_clientSocket;

	public:
		NetworkInterface(boost::asio::io_service& io_service) : m_clientSocket(io_service) {};

		tcp::socket& sock() noexcept;

		bool IsOpen();

		void SendMsg(const string& message);

		string ReceiveMsg();

		void DisconnectClient();

		~NetworkInterface() {
			if (m_clientSocket.is_open()) {
				m_clientSocket.close();
			}
		}
	};
}