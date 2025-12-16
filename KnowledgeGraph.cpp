#include "KnowledgeGraph.h"

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
    stringstream s;
    s << "(" << this->from->getVertex() << ", " << this->to->getVertex() << ", " << to_string(this->weight) << ")";
    return s.str();
}

// TODO: Implement other methods of Edge:
template <class T>
bool Edge<T>::equals(Edge<T>* edge) {
    if (edge == nullptr) return false;
    return (this->from == edge->from && this->to == edge->to);
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
    this->outDegree_++;
    to->inDegree_++;
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
    for (int i = 0; i < adList.size(); ++i) {
        Edge<T>* e = adList[i];
        if (e->getDest() == to) {
            adList.erase(adList.begin() + i);
            this->outDegree_--;
            to->inDegree_--;
            delete e;
            return;
        }
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

template <class T>
string VertexNode<T>::toString() {
    stringstream s;
    s << "(" << this->vertex << ", " << this->inDegree_ << ", " << this->outDegree_ << ", [";

    for (Edge<T>* edge : adList) {
        s << edge->toString() << ", ";
    }
    
    s << "])";
    return s.str();
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
	this->clear();
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
    VertexNode<T>* newVertex = new VertexNode<T>(vertex, vertexEQ, vertex2str);
	this->nodeList.push_back(newVertex);
}

template <class T>
bool DGraphModel<T>::contains(T vertex) {
	for (VertexNode<T>* node : this->nodeList) {
		if (node->getVertex() == vertex) return true;
	}
	
	return false;
}

template <class T>
float DGraphModel<T>::weight(T from, T to) {
	VertexNode<T>* fromNode = this->getVertexNode(from);
	VertexNode<T>* toNode = this->getVertexNode(to);
	if (!fromNode || !toNode) throw VertexNotFoundException();

	Edge<T>* edge = fromNode->getEdge(toNode);
	if (!edge) throw EdgeNotFoundException();

	return edge->getWeight();
}

template <class T>
vector<Edge<T>*> DGraphModel<T>::getOutwardEdges(T from) {
	vector<Edge<T>*> res;
	VertexNode<T>* vFrom = this->getVertexNode(from);
	if (!vFrom) throw VertexNotFoundException();
	else {
		for (Edge<T>* edge : vFrom->adList) {
			res.push_back(edge);
		}

		return res;
	}
}

template <class T>
void DGraphModel<T>::connect(T from, T to, float weight) {
	VertexNode<T>* vFrom = this->getVertexNode(from);
	VertexNode<T>* vTo = this->getVertexNode(to);
	if (!vFrom || !vTo) throw VertexNotFoundException();

    Edge<T>* e = new Edge<T>(vFrom, vTo, weight);

    vFrom->adList.push_back(e);
    vTo->adList.push_back(e);

    vFrom->outDegree_++;
    vTo->inDegree_++;
}

template <class T>
void DGraphModel<T>::disconnect(T from, T to) {
	VertexNode<T>* vFrom = this->getVertexNode(from);
	VertexNode<T>* vTo = this->getVertexNode(to);
	if (!vFrom || !vTo) throw VertexNotFoundException();

	vFrom->removeTo(vTo);
}

template <class T>
bool DGraphModel<T>::connected(T from, T to) {
	VertexNode<T>* vFrom = this->getVertexNode(from);
	VertexNode<T>* vTo = this->getVertexNode(to);
	if (!vFrom || !vTo) throw VertexNotFoundException();

	Edge<T>* connectingEdge =  vFrom->getEdge(vTo);
	if (!connectingEdge) return false;
	return true;
}

template <class T>
int DGraphModel<T>::size() {
	return this->nodeList.size();
}

template <class T>
bool DGraphModel<T>::empty() {
	return this->nodeList.empty();
}

template <class T>
void DGraphModel<T>::clear() {
    for (VertexNode<T>* node : nodeList) {
        node->adList.clear();
    }

    for (VertexNode<T>* node : nodeList) {
        delete node;
    }

    nodeList.clear();
}


template <class T>
int DGraphModel<T>::inDegree(T vertex) {
	VertexNode<T>* v = getVertexNode(vertex);
	if (!v) throw VertexNotFoundException();
	return v->inDegree();
}

template <class T>
int DGraphModel<T>::outDegree(T vertex) {
	VertexNode<T>* v = getVertexNode(vertex);
	if (!v) throw VertexNotFoundException();
	return v->outDegree();
}

template <class T>
vector<T> DGraphModel<T>::vertices() {
    vector<T> res;

    for (VertexNode<T>* vertex : nodeList) {
        res.push_back(vertex->getVertex());
    }

    return res;
}

template <class T>
string DGraphModel<T>::toString(){
    stringstream s;
    s << "[";

    for (VertexNode<T>* node : this->nodeList) {
        s << node->toString() << ", ";
    }

    s << "]";
    return s.str();
}

template <class T>
string DGraphModel<T>::BFS(T start) {
	VertexNode<T>* startNode = this->getVertexNode(start);
	if (!startNode) throw VertexNotFoundException();

	vector<VertexNode<T>*> q;
	vector<VertexNode<T>*> visited;
	stringstream s;
    s << "[ ";
	q.push_back(startNode);

	while (!q.empty()) {
		VertexNode<T>* node = q.front(); q.erase(q.begin());
		if (this->isVisited(node, visited)) continue;

		visited.push_back(node);
		s << node->toString() << ", ";
		
		for (int i = 0; i < node->adList.size(); ++i) {
			VertexNode<T>* toNode = node->adList[i]->getDest();
			if (!this->isVisited(toNode, visited)) q.push_back(toNode);
		}
	}
    s << " ]";

	return s.str();
}

template <class T>
string DGraphModel<T>::DFS(T start) {
	VertexNode<T>* startNode = this->getVertexNode(start);
	if (!startNode) throw VertexNotFoundException();

	vector<VertexNode<T>*> st;
	vector<VertexNode<T>*> visited;
	stringstream s;
    s << "[ ";
	st.push_back(startNode);

	while (!st.empty()) {
		VertexNode<T>* node = st.back(); st.pop_back();
		if (this->isVisited(node, visited)) continue;

		visited.push_back(node);
		s << node->toString() << ", ";
		
		for (int i = node->adList.size() - 1; i >= 0; --i) {
			VertexNode<T>* toNode = node->adList[i]->getDest();
			if (!this->isVisited(toNode, visited)) st.push_back(toNode);
		}
	}
    s << " ]";

	return s.str();
}
// =============================================================================
// Class KnowledgeGraph Implementation
// =============================================================================

KnowledgeGraph::KnowledgeGraph() {
}

void KnowledgeGraph::addEntity(string entity) {
	if (this->hasEntity(entity)) throw EntityExistsException();

	this->graph.add(entity);
	this->entities.push_back(entity);
}

void KnowledgeGraph::addRelation(string from, string to, float weight) {
	if (!this->graph.contains(from) || !this->graph.contains(to)) throw EntityNotFoundException();

	this->graph.connect(from, to, weight);
}

vector<string> KnowledgeGraph::getAllEntities() {
	return this->entities;
}

vector<string> KnowledgeGraph::getNeighbors(string entity) {
	if (!this->hasEntity(entity)) throw EntityNotFoundException();
    VertexNode<string>* node = this->graph.getVertexNode(entity);
    return node->getOutVertices();
}

string KnowledgeGraph::bfs(string start) {
    if (!this->hasEntity(start)) throw EntityNotFoundException();

    return this->graph.BFS(start);
}

string KnowledgeGraph::dfs(string start) {
    if (!this->hasEntity(start)) throw EntityNotFoundException();

    return this->graph.DFS(start);
}

bool KnowledgeGraph::isReachable(string from, string to) {
	VertexNode<string>* vFrom = this->graph.getVertexNode(from);
	VertexNode<string>* vTo = this->graph.getVertexNode(to);

	if (!vFrom || !vTo) throw EntityNotFoundException();

	vector<VertexNode<string>*> q;	
	q.push_back(vFrom);
	vector<VertexNode<string>*> visited;

	while (!q.empty()) {
		VertexNode<string>* current = q.front(); q.erase(q.begin());
		if (this->graph.isVisited(current, visited)) continue;
		visited.push_back(current);

		if (current == vTo) return true;

		vector<Edge<string>*> neighbors = current->getAdList();
		for (Edge<string>* edge : neighbors) {
			if (edge->getStart() == current) {
				VertexNode<string>* next = edge->getDest();
				if (!this->graph.isVisited(next, visited)) q.push_back(next);
			}
		}
	}

	return false;
}

string KnowledgeGraph::toString() {
    return this->graph.toString();
}

vector<string> KnowledgeGraph::getRelatedEntities(string entity, int depth) {
    if (!hasEntity(entity)) throw EntityNotFoundException();
    if (depth < 1) return {};

    VertexNode<string>* start = graph.getVertexNode(entity);

    vector<string> result;
    vector<VertexNode<string>*> visited;
    queue<pair<VertexNode<string>*, int>> q;

    q.push({start, 0});

    while (!q.empty()) {
        auto [current, d] = q.front();
        q.pop();

        if (graph.isVisited(current, visited)) continue;
        visited.push_back(current);

        if (d > 0) {
            result.push_back(current->getVertex());
        }

        if (d == depth) continue;

		vector<Edge<string>*> neighbors = current->getAdList();
        for (Edge<string>* e : neighbors) {
            if (e->getStart() == current) {
                VertexNode<string>* next = e->getDest();
                if (!graph.isVisited(next, visited)) {
                    q.push({next, d + 1});
                }
            }
        }
    }

    return result;
}

string KnowledgeGraph::findCommonAncestors(string entity1, string entity2) {
    if (!hasEntity(entity1) || !hasEntity(entity2))
        throw EntityNotFoundException();

    vector<string> ancestors;

    for (string v : entities) {
        if (isReachable(v, entity1) && isReachable(v, entity2)) {
            ancestors.push_back(v);
        }
    }

    stringstream ss;
    ss << "[";

    for (int i = 0; i < ancestors.size(); ++i) {
        ss << ancestors[i];
        if (i + 1 < ancestors.size()) ss << ", ";
    }

    ss << "]";
    return ss.str();
}


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
