#include "../../../../include/DBMS/DbControllers/SemanticWebWithIndexing/Vertex.h"

namespace DBMS {
	namespace SemanticWebWithIndexingDbController {
		Vertex::Vertex() {};

		Vertex::Vertex(const vertexIdType id, const string text) : my_id(id), my_text(text) {};


		bool Vertex::operator==(const Vertex& vertex) const {
			return my_id == vertex.my_id && my_text == vertex.my_text;
		}

		bool Vertex::operator==(const vertexIdType vertex_id) const {
			return my_id == vertex_id;
		}

		bool Vertex::operator==(const string& text) const {
			return my_text == text;
		}

		std::ostream& operator<<(std::ostream& ostream, const Vertex& vertex) {
			ostream.write((char*)(&(vertex.my_id)), sizeof(vertex.my_id));

			ostream << vertex.my_text.c_str();

			return ostream;
		}

		std::istream& operator>>(std::istream& istream, Vertex& vertex) {
			auto& vertex_id = vertex.my_id;
			auto& text = vertex.my_text;

			istream.read((char*)&vertex_id, sizeof(vertexIdType));
			std::getline(istream, text, '\0');

			return istream;
		}
	}
}