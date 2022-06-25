#pragma once
#ifdef MULTIUSERS
#include <thread>
#include <unordered_set>
#endif // MULTIUSERS

#include "Iis.h"
#include "AcceptedClient.h"

#ifdef MULTIUSERS
#include "GlobalIISMutex.h"

using std::thread;

class Server {
private:
	boost::asio::io_service io_service;
	tcp::acceptor m_acceptor;
	shared_ptr<DbController> m_dbController;
	std::unordered_set<std::shared_ptr<IIS>> m_activeConnections;

public:
	Server(int port, shared_ptr<DbController> dbController) : m_acceptor(io_service, tcp::endpoint(tcp::v4(), port), false), m_dbController(dbController) {};

	void Run() {
		cout << "Listening on " << m_acceptor.local_endpoint() << "\n";

		DoAccept();

		io_service.run();
	}

	void Stop() {
		m_acceptor.close();

		{
			std::vector<std::shared_ptr<IIS>> sessionToClose;

			copy(m_activeConnections.begin(), m_activeConnections.end(), back_inserter(sessionToClose));

			for (auto& s : sessionToClose) {
				//s->Shutdown();
			}
		}

		m_activeConnections.clear();
		io_service.stop();
	}

private:
	void DoAccept() {
		if (m_acceptor.is_open()) {
			std::shared_ptr<AcceptedClient> acceptedClient = std::make_shared<AcceptedClient>(io_service);
			auto iisSession = std::make_shared<IIS>(m_dbController, acceptedClient);
			
			m_acceptor.async_accept(iisSession->GetAcceptedClient()->sock(), [this, iisSession](boost::system::error_code ec) {
				DoAccept();

				if (!ec) {
					m_activeConnections.insert(iisSession);

					cout << "Accepted client on " << iisSession->GetAcceptedClient()->sock().remote_endpoint() << endl;

					iisSession->RunInterpret();
				}
				});
		}
	}
};

extern std::mutex globalMutex;
#endif // MULTIUSERS

int main(int argc, char* argv[]);
