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
	}
}