#include "../head/node.h"

void Node::removeNeighbour(Node* node)
{
	neighbours.erase(node);
}

void Node::addneighbour(Node* node, int w)
{
	neighbours[node] = w;
}

int Node::getWeight(Node* node) const
{
    auto it = neighbours.find(node);
    if (it != neighbours.end())
        return it->second;
    return 0;
}
