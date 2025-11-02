#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <string>
#include <set>
#include <fstream>
#include <iostream>

#include "ic.h"
#include "node.h"

using namespace std;


class Graph
{
	std::set<Node*> nodes;
	bool oriented = false;
	typedef std::set<Node*>::const_iterator node_iterator;

public:
	// конструкторы и деструктор
	Graph(const Ic& ic);
	Graph() {};
    ~Graph();

    // методы добавления и удаления узлов
	Node* addNode(const string& name);
	void removeNode(Node* node);
	// добавление и удаление узлов
	void addEdge(Node* begin, Node* end, int w);
	void removeEdge(Node* begin, Node* end);
	node_iterator end() const { return nodes.end(); }

	// количество узлов и ребер
	size_t node_count() const;
	size_t edges() const;
	// вернуть список узлов
	set<Node*> get_nodes() const { return nodes; }
	// вывод графа
	friend ostream& operator << (ostream& out, const Graph& graph);
};

class GraphReadError {};

#endif // !_GRAPH_H_
