#include "../../include/Interpretator/Interpretator.h"

#ifdef UNIT_TESTS
void IIS::RunExecute(string request, string addText) {
	m_dbController->OperationVertexesInBuffer(
		vector<Vertex*> {
		&Vertex(SWObjectInputUserRequest, request),
			& Vertex(SWObjectAdditionalToUserRequest, addText),
			&Vertex(SWObjectMainModuleTextId, addText)
	}, OperationParametrs::Changing);

	RunInterpret();
}
#endif // UNIT_TESTS

void IIS::RunInterpret() {
	vector<ModuleTable*> tables;
	MethodTable* startModuleMethodTable = nullptr;

	// Парсинг запускающего модуля
	{
		RecursionParser parser(tables, m_dbController);

		parser.MakeRecourseParsing(m_requestStartModuleId);

		startModuleMethodTable = parser.GetStartModuleMethodTable();
	}

	// Семантический анализ. Возможно не нужен


	// Интерпретация
	Interpretator interpetator(tables, m_dbController, m_acceptedClient, startModuleMethodTable);

	interpetator.Run();

	interpetator.ClearInterpretator();
}
