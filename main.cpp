#include "KnowledgeGraph.h"

int main() {
    cout << "Test toString methods of Edge, VertexNode, and DGraphModel" << endl;

    // Define function pointers
    auto vertexEQ = [](string& a, string& b) -> bool { return a == b; };
    auto vertex2str = [](string& s) -> string { return s; };

    DGraphModel<string> graph(vertexEQ, vertex2str);
    // Add vertices
    graph.add("A");
    graph.add("B");
    graph.add("C");

    // Add connections
    graph.connect("A", "B", 2.5f);
    graph.connect("B", "C", 3.0f);
    graph.connect("A", "C", 1.0f);

    // Test DGraphModel toString (which internally uses VertexNode toString and Edge toString)
    cout << "DGraphModel toString:" << endl;
    string graphString = graph.toString();
    cout << graphString << endl;

    // Test individual VertexNode toString by getting vertex and checking outward edges
    cout << "Testing individual components:" << endl;
    vector<Edge<string>*> outwardEdges = graph.getOutwardEdges("A");
    cout << "Vertex A has " << outwardEdges.size() << " outward edges" << endl;
}
