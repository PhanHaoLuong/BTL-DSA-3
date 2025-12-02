#include "KnowledgeGraph.h"
#include <cstddef>

// =============================================================================
// Class Edge Implementation
// =============================================================================

template <class T>
Edge<T>::Edge(VertexNode<T>* from, VertexNode<T>* to, float weight) {
    this->from = from;
    this->to = to;
    this->weight = weight;
}

template <class T>
string Edge<T>::toString() {
    // TODO: Return the string representation of the edge
    return "";
}

// TODO: Implement other methods of Edge:
template <class T>
bool Edge<T>::equals(Edge<T>* edge) {
    if (edge == nullptr) return false;
    return (this->from == edge->from && this->to == edge->to && this->weight == edge->weight);
}

template <class T>
bool Edge<T>::edgeEQ(Edge<T> *&edge1, Edge<T> *&edge2) {
    if (edge1 == nullptr || edge2 == nullptr) return false;
    return edge1->equals(edge2);
}

// =============================================================================
// Class VertexNode Implementation
// =============================================================================

template <class T>
VertexNode<T>::VertexNode(T vertex, bool (*vertexEQ)(T&, T&), string (*vertex2str)(T&)) {
    this->vertex = vertex;
    this->vertexEQ = vertexEQ;
    this->vertex2str = vertex2str;
    this->inDegree_ = 0;
    this->outDegree_ = 0;
}

template <class T>
T& VertexNode<T>::getVertex() {
    return this->vertex;
}

template <class T>
void VertexNode<T>::connect(VertexNode<T>* to, float weight) {
    Edge<T>* newEdge = new Edge<T>(this, to, weight);
    this->adList.push_back(newEdge);
    to->adList.push_back(newEdge);
}

template <class T>
Edge<T>* VertexNode<T>::getEdge(VertexNode<T>* to) {
    Edge<T>* res = nullptr;
    for (Edge<T>* e : this->adList) {
        if (e->getDest() == to) res = e;
    }

    return res;
}

template <class T>
bool VertexNode<T>::equals(VertexNode<T>* node) {
    return this->vertexEQ(this->vertex, node->getVertex());
}

template <class T>
void VertexNode<T>::removeTo(VertexNode<T>* to) {
    for (int i = 0; i < this->adList.size(); ++i) {
        if (this->adList.at(i)->getDest() == to) this->adList.erase(this->adList.begin() + i);
    }
}

template <class T>
int VertexNode<T>::inDegree() {
    return this->inDegree_;
}

template <class T>
int VertexNode<T>::outDegree() {
    return this->outDegree_;
}

// =============================================================================
// Class DGraphModel Implementation
// =============================================================================

template <class T>
DGraphModel<T>::DGraphModel(bool (*vertexEQ)(T&, T&), string (*vertex2str)(T&)) {
    this->vertexEQ = vertexEQ;
    this->vertex2str = vertex2str;
}

template <class T>
DGraphModel<T>::~DGraphModel() {
	for (VertexNode<T>* node : this->nodeList) delete node;
}

template <class T>
VertexNode<T>* DGraphModel<T>::getVertexNode(T& vertex) {
	for (VertexNode<T>* node : this->nodeList) {
		if (node->getVertex() == vertex) return node;
	}

	return nullptr;
}

template <class T>
void DGraphModel<T>::add(T vertex) {
	VertexNode<T>* newVertex = new VertexNode<T>(vertex);
	this->nodeList.push_back(newVertex);
}

template <class T>
bool DGraphModel<T>::contains(T vertex) {
	for (VertexNode<T>* node : this->nodeList) {
		if (node->getVertex() == vertex) return true;
	}
	
	return false;
}

//TODO Change implementation
template <class T>
float DGraphModel<T>::weight(T from, T to) {

}

template <class T>
vector<T> DGraphModel<T>::getOutwardEdges(T from) {
	vector<T> res;
	VertexNode<T>* vFrom = this->getVertexNode(from);
	if (!vFrom) throw VertexNotFoundException();
	else {
		for (Edge<T>* edge : vFrom->adList) {
			res.push_back(edge->getDest()->getVertex());
		}

		return res;
	}
}

template <class T>
void DGraphModel<T>::connect(T from, T to, float weight) {
	VertexNode<T>* vFrom = this->getVertexNode(from);
	VertexNode<T>* vTo = this->getVertexNode(to);
	if (!vFrom || !vTo) throw VertexNotFoundException();

	vFrom->connect(vTo, weight);
}

template <class T>
void DGraphModel<T>::disconnect(T from, T to) {
	VertexNode<T>* vFrom = this->getVertexNode(from);
	VertexNode<T>* vTo = this->getVertexNode(to);
	if (!vFrom || !vTo) throw VertexNotFoundException();

	vFrom->removeTo(vTo);
}

// =============================================================================
// Class KnowledgeGraph Implementation
// =============================================================================

KnowledgeGraph::KnowledgeGraph() {
    // TODO: Initialize the KnowledgeGraph
}

void KnowledgeGraph::addEntity(string entity) {
    // TODO: Add a new entity to the Knowledge Graph
}

void KnowledgeGraph::addRelation(string from, string to, float weight) {
    // TODO: Add a directed relation
}

// TODO: Implement other methods of KnowledgeGraph:



// =============================================================================
// Explicit Template Instantiation
// =============================================================================

template class Edge<string>;
template class Edge<int>;
template class Edge<float>;
template class Edge<char>;

template class VertexNode<string>;
template class VertexNode<int>;
template class VertexNode<float>;
template class VertexNode<char>;

template class DGraphModel<string>;
template class DGraphModel<int>;
template class DGraphModel<float>;
template class DGraphModel<char>;
