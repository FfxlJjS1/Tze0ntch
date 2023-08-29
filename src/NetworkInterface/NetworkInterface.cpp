#include "../../include/NetworkInterface/NetworkInterface.h"

namespace NetworkInterface {
	tcp::socket& NetworkInterface::sock() {
		return my_client_socket;
	}

	bool NetworkInterface::NetworkInterface::is_open() {
		return my_client_socket.is_open();
	}

	void NetworkInterface::send_msg(const string& message) {
		streambuf bufForEntityMsg(message.length() + sizeof(uint32_t));

		std::ostream strStream(&bufForEntityMsg);

		uint32_t msgSize = static_cast<uint32_t>(message.length());

		strStream.put(msgSize & 0xFF);
		strStream.put((msgSize >> 8) & 0xFF);
		strStream.put((msgSize >> 16) & 0xFF);
		strStream.put((msgSize >> 24) & 0xFF);

		strStream.write(message.c_str(), msgSize);

		write(my_client_socket, bufForEntityMsg);
	}

	string NetworkInterface::receive_msg() {
		uint32_t messageSize = 0;

		{
			// Get message size from first bytes about rest size
			streambuf bufForMsgSize(sizeof(uint32_t));

			read(my_client_socket, bufForMsgSize);

			const unsigned char* uint32Msg = buffer_cast<const unsigned char*>(bufForMsgSize.data());

			messageSize = (uint32_t)uint32Msg[3] << 24 |
				(uint32_t)uint32Msg[2] << 16 |
				(uint32_t)uint32Msg[1] << 8 |
				(uint32_t)uint32Msg[0];
		}

		streambuf bufForEntityMsg(messageSize);

		read(my_client_socket, bufForEntityMsg);

		auto bufIterator = boost::asio::buffers_begin(bufForEntityMsg.data());

		string data(bufIterator, bufIterator + messageSize);

		data = utf2cp(data);

		return data;
	}

	void NetworkInterface::disconnect_client() {
		if (my_client_socket.is_open()) {
			cout << my_client_socket.remote_endpoint().address().to_string() << " is disconnected by server" << endl;

			my_client_socket.close();
		}
		else {
			cout << "Client already is closed socket" << endl;
		}
	}
}