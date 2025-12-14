#include "../head/graph.h"

Graph::Graph(const Cfig& cfig)
{
	ifstream in(cfig("graph", "file"));

	if (!in.is_open())
		throw GraphReadError();
		
	if (cfig("graph", "head").toBool())
	{
		string s;
		getline(in, s);
	}

	oriented = cfig("graph", "oriented").toBool();

	string a, b;
    int c;
    while (in >> a >> b >> c)
    {
        Node* node_a = addNode(a);
		Node* node_b = addNode(b);
		addEdge(node_a, node_b, c);
    }
		
}

size_t Graph::node_count() const
{
	return nodes.size();
}

size_t Graph::edges() const
{
	size_t cnt = 0;
	for (Node* node : nodes)
		cnt += node->neighbours.size();
	return cnt;
}

void Graph::removeNode(Node* node)
{
	nodes.erase(node);

	for (std::set<Node*>::iterator it = nodes.begin();
		it != nodes.end(); it++)
		(*it)->removeNeighbour(node);
}

Node* Graph::addNode(const string& name)
{

	for (Node* node : nodes)
	{
		if (node->getName() == name)
			return node;
	}
	Node* node_1 = new Node(name);
	nodes.insert(node_1);
	return node_1;
}

void Graph::addEdge(Node* begin, Node* end, int w)
{
	if (nodes.find(begin) == nodes.end())
		return;
	if (nodes.find(end) == nodes.end())
		return;
	begin->addneighbour(end, w);
	if (!oriented)
		end->addneighbour(begin, w);
}

void Graph::removeEdge(Node* begin, Node* end)
{
	if (nodes.find(begin) == nodes.end())
		return;
	if (nodes.find(end) == nodes.end())
		return;
	begin->removeNeighbour(end);
	if (!oriented)
		end->removeNeighbour(begin);
}

ostream& operator << (ostream& out, const Graph& graph) 
{
    out << "Node: countWay" << std::endl;
	for (Node* node : graph.get_nodes())
	{
		out << node->getName() << ": " << node->countWay() << std::endl;
	}
	return out;

}

Graph::~Graph()
{
    for (auto node: nodes)
        delete node;
    nodes.clear();
}
