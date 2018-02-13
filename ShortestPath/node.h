#pragma once

#include "point.h"
#include "edge.h"

class Edge;

class Node {
public:

	Point point;
	std::vector <Node*> adj;
	std::vector <Edge*> adjEdges;
	std::vector <Node*> SPs;
	float distance;

	Node();

	Node(Point p);

	bool operator== (Node a) const;

	bool operator< (Node a)const;

	float distanceTo(Node *other);
};