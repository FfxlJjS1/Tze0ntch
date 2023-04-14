#include "main.h"

int main(int argc, char* argv[]) {
	system("chcp 1251");
	setlocale(LC_ALL, "");

	// TODO: Reading parametrs from ini file. Directory for DBMS, configuration for NetworkInterface

	shared_ptr<DbController> dbController = std::make_shared<DbController>();

	if (!dbController->OpenDb()) {
		dbController->CreateStructuredDbFile();

		dbController->OpenDb();
	}

#ifdef MULTIUSERS
	try {
		Server srv(12001, dbController);

		srv.Run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << endl;
	}
#else
	boost::asio::io_service io_service;
	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12001));
	shared_ptr<AcceptedClient> accCl = std::make_shared<AcceptedClient>(io_service);

	cout << "Listening on " << acceptor.local_endpoint() << "\n";

	cout << "Waiting for new client connection" << endl;

	acceptor.accept(accCl->sock());

	cout << "Run iis thread for connected client from address " << accCl->sock().remote_endpoint().address().to_string() << " with port " << accCl->sock().remote_endpoint().port() << endl;

	IIS(dbController, accCl).RunInterpret();
#endif // MULTIUSERS

	return 0;
};
