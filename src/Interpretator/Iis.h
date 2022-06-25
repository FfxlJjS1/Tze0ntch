#pragma once
#include "DbController.h"
#include "AcceptedClient.h"

#include "RecursionParser.h"
//#include "SyntacticalAnalyzer.h"
#include "Interpretator.h"

#ifdef MULTIUSERS
extern std::mutex globalMutex;
#endif

using std::shared_ptr;

class IIS {
private:
	shared_ptr<DbController> m_dbController;
	shared_ptr<AcceptedClient> m_acceptedClient;
	SemanticWeb m_operatingSW;
	SemanticWeb m_sessionSW;
	vertexIdType m_requestStartModuleId = MainModuleIdInSW;

public:
	IIS(shared_ptr<DbController> dbController, shared_ptr<AcceptedClient> acceptedClient)
		: m_dbController(dbController),
		m_acceptedClient(acceptedClient) {
		m_operatingSW.fullByBasicElements();
	};

#ifdef UNIT_TESTS
	// Only for unit tests
	void RunExecute(string request, string addText);

	// Only for unit tests
	string GetResponce() {
		return m_dbController->ReadVertex(SWObjectOutputSystemResponse)->GetText();
	}
#endif

	void RunInterpret();

#ifdef MULTIUSERS
	shared_ptr<AcceptedClient> GetAcceptedClient() {
		return m_acceptedClient;
	}
#endif // MULTIUSERS

	~IIS() {
		m_operatingSW.Clear();
	}
};
