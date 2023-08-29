#pragma once

#include <thread>
#include <unordered_set>

#include "IniConfigure.h"
#include "DBMS/DBMS.h"
#include "Interpretator/Interpretator.h"
#include "NetworkInterface/NetworkInterface.h"

using std::thread;

class Server final {
private:
	boost::asio::io_service my_io_service;
	boost::asio::tcp::acceptor my_acceptor;
	shared_ptr<DBMS::DBMS> my_dbms;
	std::unordered_set<std::shared_ptr<Interpretator::Interpretator>> my_active_connections;

public:
	Server(int port, shared_ptr<DBMS::DBMS> dbms);

	void run();

	void stop();

private:
	void do_accept();
};


int main(int argc, char* argv[]);
