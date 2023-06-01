#pragma once
#include <string>
#include<fstream>

namespace DBMS {
    namespace KeyValueDbController {
        using std::string;

        typedef size_t vertexIdType;

        struct Vertex final {
        private:
            vertexIdType m_id = 0;
            string m_text = "";

        public:
            Vertex() noexcept {};

            Vertex(const vertexIdType id) : Vertex(id, "") {};

            Vertex(const string text) : Vertex(0, text) {};

            Vertex(const vertexIdType id, const string text) : m_id(id), m_text(text) {};


            void SetId(const vertexIdType newId) {
                m_id = newId;
            }

            vertexIdType GetId() const noexcept {
                return m_id;
            }

            void SetText(const string newText) {
                m_text = newText;
            }

            string GetText() const {
                return m_text;
            }

            Vertex* Clone() const {
                return new Vertex(m_id, m_text);
            }

            // Operators
            bool operator==(Vertex const& vertex) const {
                return vertex.GetId() == m_id && vertex.GetText() == m_text;
            }

            bool operator!=(Vertex const& vertex) const {
                return vertex.GetId() != m_id || vertex.GetText() != m_text;
            }

            bool operator==(const int vertexId) const {
                return m_id == vertexId;
            }
        };
    }
}
