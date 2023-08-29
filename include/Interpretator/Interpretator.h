#pragma once
#include "DBMS.h"
#include "NetworkInterface.h"

#include "RecursionParsingStage.h"
//#include "SyntacticalAnalyzingStage.h"
#include "InterpretingStage.h"


namespace Interpretator {
	using std::shared_ptr;

	using DBMS::DBMS;
	using DBMS::DbController;
	using NetworkInterface::NetworkInterface;

	class Interpretator final {
	private:
		shared_ptr<DBMS> my_dbms;
		shared_ptr<NetworkInterface> my_network_interface;

		shared_ptr<unsigned char> interpretator_proccessing_code = 0;
		DBMS::ModuleDbController::objectIdType my_request_start_module_id = 1;


	public:
		Interpretator(shared_ptr<DBMS> dbms, shared_ptr<NetworkInterface> network_interface);


		void run_interpreting();

		void stop_interpreting();


		~Interpretator();
	};
}
