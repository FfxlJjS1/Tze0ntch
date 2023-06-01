#pragma once

#include <string>

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		using std::string;

		typedef __int32 vertexIdType;

		struct Vertex final {
		public:
			vertexIdType my_id = 0;
			string my_text = "";


			Vertex();

			Vertex(const vertexIdType id, const string text);


			bool operator==(const Vertex& vertex) const;

			bool operator==(const vertexIdType vertex_id) const;

			bool operator==(const string& text) const;
		};
	}
}