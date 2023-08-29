#include "../../include/Interpretator/Interpretator.h"

namespace Interpretator {
	Interpretator(shared_ptr<DBMS> dbms, shared_ptr<NetworkInterface> network_interface)
		: my_dbms(dbms),
		my_network_interface(network_interface) {}

	void Interpretator::run_interpreting() {
		vector<ModuleTable*> tables;
		MethodTable* start_module_method_table = nullptr;

		// Parsing start module
		{
			RecursionParser recur_parsing_stage(tables, my_dbms, interpretator_proccessing_code);

			recur_parsing_stage.MakeRecourseParsing(my_request_start_module_id);

			start_module_method_table = recur_parsing_stage.GetStartModuleMethodTable();
		}

		// Semantical analyzing. Maybe will not exist


		// Interpretating
		InterpretingStage interpeting_stage(tables, my_dbms, my_network_interface, interpretator_proccessing_code, start_module_method_table);

		interpeting_stage.run();
	}
}
