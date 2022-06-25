#pragma once
#include <iostream>
#include <string>

#include <boost/asio.hpp>

#include "EncodingConvertor.h"

using std::cout, std::endl;
using std::string;

using boost::asio::buffer,
	boost::asio::streambuf,
	boost::asio::buffer_cast,
	boost::asio::ip::tcp,
	boost::asio::write,
	boost::asio::read_until;

class AcceptedClient final {
private:
	tcp::socket m_clientSocket;

public:
	AcceptedClient(boost::asio::io_service& io_service) : m_clientSocket(io_service) {};

	tcp::socket& sock() noexcept {
		return m_clientSocket;
	}

	void DisconnectClient() {
		if (m_clientSocket.is_open()) {
			cout << m_clientSocket.remote_endpoint().address().to_string() << " is disconnected by server" << endl;

			m_clientSocket.close();
		}
		else {
			cout << "Client already is closed socket" << endl;
		}
	}

	bool IsOpen() {
		return m_clientSocket.is_open();
	}

	void SendMsg(const string& message) {
		streambuf bufForEntityMsg(message.length() + sizeof(uint32_t));
		
		std::ostream strStream(&bufForEntityMsg);

		uint32_t msgSize = static_cast<uint32_t>(message.length());

		strStream.put(msgSize & 0xFF);
		strStream.put((msgSize >> 8) & 0xFF);
		strStream.put((msgSize >> 16) & 0xFF);
		strStream.put((msgSize >> 24) & 0xFF);

		strStream.write(message.c_str(), msgSize);

		write(m_clientSocket, bufForEntityMsg);
	}

	string ReceiveMsg() {
		uint32_t messageSize = 0;

		{
			// Get message size from first bytes about rest size
			streambuf bufForMsgSize(sizeof(uint32_t));

			read(m_clientSocket, bufForMsgSize);

			const unsigned char* uint32Msg = buffer_cast<const unsigned char*>(bufForMsgSize.data());

			messageSize = (uint32_t)uint32Msg[3] << 24 |
				(uint32_t)uint32Msg[2] << 16 |
				(uint32_t)uint32Msg[1] << 8 |
				(uint32_t)uint32Msg[0];
		}

		streambuf bufForEntityMsg(messageSize);

		read(m_clientSocket, bufForEntityMsg);

		auto bufIterator = boost::asio::buffers_begin(bufForEntityMsg.data());
		
		string data(bufIterator, bufIterator + messageSize);

		data = utf2cp(data);

		return data;
	}

	~AcceptedClient() {
		if (m_clientSocket.is_open()) {
			m_clientSocket.close();
		}
	}
};
