#pragma once

#include <iostream>
#include <string>

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		using std::string;

		typedef __int32 vertexIdType;

		struct Vertex final {
		public:
			vertexIdType my_id = 0;
			mutable string my_text = "";


			Vertex();

			Vertex(const vertexIdType id, const string text);


			bool operator==(const Vertex& vertex) const;

			bool operator==(const vertexIdType vertex_id) const;

			bool operator==(const string& text) const;

			friend std::ostream& operator<<(std::ostream& ostream, const Vertex& vertex);

			friend std::istream& operator>>(std::istream& istream, Vertex& vertex);
		};
	}
}