#include "main.h"


Server::Server(int port, shared_ptr<DBMS::DBMS> dbms) : my_acceptor(io_service, tcp::endpoint(tpc::v4(), port), false), my_dbms(dbms) {}

void Server::run() {
	cout << "Listening on " << my_acceptor.local_endpoint() << "\n";

	do_accept();

	my_io_service.run();
}

void Server::stop() {
	my_acceptor.close();

	{
		std::vector<shared_ptr<Interpretator::Interpretator>> session_to_close;

		copy(my_active_connections.begin(), my_active_connections.end(), back_inserter(session_to_close));

		for (auto& s : session_to_close) {
			s->stop_interpreting();
		}
	}

	my_activeConnections.clear();
	my_io_service.stop();
}

void Server::do_accept() {
	if (my_acceptor.is_open()) {
		shared_ptr<NetworkInterface::NetworkInterface> accepted_client = std::make_shared<NetworkInterface::NetworkInterface>(my_io_service);

		my_acceptor.async_accept(accepted_client->sock(),
			[my_dbms, my_active_connections, accepted_client](boost::system::error_code ec) {
				do_accept();

				if (!ec) {
					auto interpretator_session = std::make_shared<Interpretator::Interpretator>(my_dbms, accepted_client);

					my_active_connections.insert(interpretator_session);

					cout << "Accepted client on " << accepted_client->sock().remote_endpoint() << endl;

					interpretator_session->run_interpreting();
				}
			}
		);
	}
}


int main(int argc, char* argv[]) {
	system("chcp 1251");
	setlocale(LC_ALL, "");

	std::string dbms_directory_path = "";
	int network_interface_port = -1;

	{
		IniConfigurator ini_configurator;

		dbms_directory_path = ini_configurator.get_dbms_directory_path();
		network_interface_port = ini_configurator.get_network_interface_port();
	}

	shared_ptr<DBMS::DBMS> dbms = std::make_shared<DBMS::DBMS>(dbms_directory_path);

	try {
		Server srv(network_interface_port, dbms);

		srv.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << endl;

		srv.stop();
	}

	return 0;
};
