#include <string>

#include "pch.h"
#include "CppUnitTest.h"

#include "main.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ISSInformationprogramsUnitTests
{
	TEST_CLASS(ISSInformationprogramsUnitTests)
	{
	public:
		boost::asio::io_service io_service;

		TEST_METHOD(ReservedSetStringVertexWithId)
		{
			std::string codeText = "using \"DbController\"\nvoid main() {\n SetStrToVertexWithId(31, \"Hello\"); \n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("Hello"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}
			
			db->CloseDataBase();
		
			db->DeleteDataBaseFile();
		}

		TEST_METHOD(ReserverGetStringVertexWithId) {
			std::string codeText = "using \"DbController\"\nvoid main() {\n SetStrToVertexWithId(31, GetStrFromVertexWithId(34)); \n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), db->ReadVertex(34)->GetText());
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();
		
			db->DeleteDataBaseFile();
		}

		TEST_METHOD(DefineVariables) {
			std::string codeText = "using \"DbController\"\nvoid main() {\n string t = \"678\"; SetStrToVertexWithId(31, t);\n }";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("678"));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(AssertVariables) {
			std::string codeText = "using \"DbController\"\nvoid main() {\n string t = \"678\"; t = \"Hell\"; SetStrToVertexWithId(31, t);\n }";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("Hell"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			//catch (...) {
			//	Assert::IsTrue(false);
			//}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(IfTrueConstr) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring t = \"dd\"; if(true) {t = \"zz\";} SetStrToVertexWithId(31, t);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("zz"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(IfFalseConstr) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring t = \"dd\"; if(false) {t = \"zz\";} SetStrToVertexWithId(31, t);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("dd"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(IfElseFirstPerfConstr) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring t = \"dd\"; if(true) {t = \"zz\";} else {t = \"fals\";} SetStrToVertexWithId(31, t);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("zz"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(IfElseSecondPerfConstr) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring t = \"dd\"; if(false) {t = \"zz\";} else {t = \"fals\";} SetStrToVertexWithId(31, t);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("fals"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(IfElseTherdPerfConstr) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring t = \"dd\"; if(false) {t = \"zz\";} else if(false) {t = \"fals\";} else {t = \"tru\";} SetStrToVertexWithId(31, t);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("tru"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(IfElseTherdElseIfPerfConstr) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring t = \"dd\"; if(false) {t = \"zz\";} else if(false) {t = \"fals\";} else if (true) {t = \"tru\";} else {t = \"else last\";} SetStrToVertexWithId(31, t);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("tru"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(DoWhileConst) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring t = \"dd\"; do {t = t + \"d\";} while(t != \"dddd\") SetStrToVertexWithId(31, t);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("dddd"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(DoWhileSkipOtherConst) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring t = \"dd\"; do {t = t + \"d\";} while(t == \"1111\") SetStrToVertexWithId(31, t);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("ddd"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(WhileConst) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring t = \"dd\"; int i = 0; while(i < 3) {\nt= t + \"r\"; i = i + 1;\n}\nSetStrToVertexWithId(31, t);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("ddrrr"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(WhileSkipConst) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring t = \"dd\"; int i = 0; while(i > 3) {\nt= t + \"r\"; i = i + 1;\n}\nSetStrToVertexWithId(31, t);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("dd"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(StringConnectConstr) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring t = \"dd\"; t = t + \" ff\"; SetStrToVertexWithId(31, t);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("dd ff"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(IntConnectConstr) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring st = \"z\";\nint t = 3;\nt = t + 7;\nif(t == 10) {\nst = \"Ok\";\n}\nelse\n{\nst = \"False\";\n}\nSetStrToVertexWithId(31, st);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("Ok"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(SecondMethodInModuleStart) {
			string codeText = "using \"DbController\"\nvoid main() {\nsetHello();} void setHello() {SetStrToVertexWithId(31, \"Hello\");\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("Hello"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(SecondMethodWithArgs) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring st = \"Hell\";\nsetHello(st);} void setHello(string st) {SetStrToVertexWithId(31, st);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("Hell"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(SecondMethodWithReturnVarConstr) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring st = \"No\";\nst = setHello(st);SetStrToVertexWithId(31, st);} string setHello(string st) {\nreturn \"False \" + st + \" and True Yes.\";}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("False No and True Yes."));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(SecondMethodWithReturnMethodLocalVar) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring st = setHello();\nSetStrToVertexWithId(31, st);} string setHello() {\nstring t = \"NoYes\";return t;}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("NoYes"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(SecondMethodWithReturnChangedArg) {
			string codeText = "using \"DbController\"\nvoid main() {\nstring st = \"No\";\nst = setHello(st);SetStrToVertexWithId(31, st);} string setHello(string st) {\nst = st + \" or Yes.\"; return st;}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("No or Yes."));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(CreateStructSetAndGetStructVar) {
			string codeText = "using \"DbController\"\nvoid main() {\nsubNode subNodeT;\nsubNodeT.subNodeId = 55;SetStrToVertexWithId(31, ULongToString(subNodeT.subNodeId));}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("55"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(CreateVertexWithId) {
			string codeText = "using \"DbController\"\nvoid main() {\nCreateVertexWithDataInDb(61, \"77\");SetStrToVertexWithId(31, GetStrFromVertexWithId(61));}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("77"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(CreateNode) {
			string codeText = "using \"DbController\"\nvoid main() {\nCreateNodeWithIdInDb(61);\nSetStrToVertexWithId(31, ULongToString(len(GetSubNodeIdsOnNodeWithIdInDb(61))));}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("0"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(CreateSubNodeGetSubNodeId) {
			string codeText = "using \"DbController\"\nvoid main() {\nsubNode sNT; sNT.subNodeId = 11; sNT.firstVertexId = 6785; CreateSubNodeWithDataInNodeWithIdInDb("+ std::to_string(SWObjectNodeId) +", sNT); sNT = GetSubNodeWithIdFromNodeWithIdInDb(" + std::to_string(SWObjectNodeId) + ", 11); SetStrToVertexWithId(31, ULongToString(sNT.secondVertexId)); }";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("0"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(CreateModuleCheckByModuleName) {
			string codeText = "using \"ModuleManage\"\nvoid main() {\nCreateModule(10, \"DataSet\", \"\");\nSetStrToVertexWithId(31, GetModuleName(10));\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("DataSet"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (string exception) {
				Assert::AreEqual(exception, string(""));
			}
			catch(...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(CreateModuleCheckByModuleText) {
			string codeText = "using \"ModuleManage\"\nvoid main() {\nCreateModule(10, \"DataSet\", \"gfff\");\nSetStrToVertexWithId(31, GetModuleText(10));\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("gfff"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (string exception) {
				Assert::AreEqual(exception, string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(CreateRemoveModule) {
			string codeText = "using \"ModuleManage\"\nvoid main() {\nCreateModule(10, \"DataSet\", \"hgf\");\nRemoveModule(10);\n}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::IsTrue(true);	
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (string exception) {
				Assert::AreEqual(exception, string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(FindMethodIdByName) {
			string codeText = 
				"using \"ModuleManage\"\n"
				"void main() {\n"
					"SetStrToVertexWithId(31,"
						"GetModuleText(\"Main\")"
					");"
				"}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string(codeText));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (string exception) {
				Assert::AreEqual(exception, string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(GetModuleId) {
			string codeText =
				"using \"ModuleManage\"\n"
				"void main() {\n"
				"SetStrToVertexWithId(31,"
				"ULongToString(GetModuleId(\"Main\"))"
				");"
				"}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("1"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (string exception) {
				Assert::AreEqual(exception, string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(UsingParsedModuleAdd) {
			string codeText =
				"using \"IIS\"void main() {\n"
				"UsingModuleWithId("+std::to_string(DbControllerModuleIdInSW)+");"
				"SetStrToVertexWithId(31, \"1\");"
				"}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("1"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (string exception) {
				Assert::AreEqual(exception, string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(SetGetGlobalVar) {
			string codeText =
				"using \"DbController\"string gg = \"Gh\";\nvoid main() {\n"
				"gg = \"FF\";"
				"SetStrToVertexWithId(31, gg);"
				"}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("FF"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (string exception) {
				Assert::AreEqual(exception, string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(GetSetGetGlobalVar) {
			string codeText =
				"using \"DbController\"string gg = \"Gh\";\nvoid main() {\n"
				"if(gg == \"Gh\") {\n gg = \"R\";\n}\n"
				"SetStrToVertexWithId(31, gg);"
				"}";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("R"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (string exception) {
				Assert::AreEqual(exception, string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(StructMethodRun) {
			string codeText =
				"using \"DbController\"\n"
				"struct gg {\n"
				"	void E() {\n"
				"		SetStrToVertexWithId(31,\"11\");\n"
				"	}\n"
				"}\n"
				"void main() {\n"
				"	gg g;\n"
				"	g.E();\n"
				"}\n";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("11"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (string exception) {
				Assert::AreEqual(exception, string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}

		TEST_METHOD(StructMethodRunWithOwnData) {
			string codeText =
				"using \"DbController\"\n"
				"struct gg {\n"
				"	ulong t = 0;\n"
				"	void E() {\n"
				"		SetStrToVertexWithId(31,ULongToString(t));\n"
				"	}\n"
				"	void S(ulong f) {\n"
				"		t = f;\n"
				"	}\n"
				"}\n"
				"void main() {\n"
				"	gg g;\n"
				"	g.S(55);\n"
				"	g.E();\n"
				"}\n";

			shared_ptr<DbController> db = std::make_shared<DbController>();

			if (!db->OpenDb()) {
				db->CreateStructuredDbFile();

				db->OpenDb();
			}

			IIS iis(db, std::make_shared<AcceptedClient>(io_service));

			try {
				iis.RunExecute("Run interpret", codeText);

				Assert::AreEqual(iis.GetResponce(), string("55"));
			}
			catch (const char* exception) {
				Assert::AreEqual(string(exception), string(""));
			}
			catch (string exception) {
				Assert::AreEqual(exception, string(""));
			}
			catch (...) {
				Assert::IsTrue(false);
			}

			db->CloseDataBase();

			db->DeleteDataBaseFile();
		}
	};
}
