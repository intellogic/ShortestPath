#pragma once

#include "point.h"
#include "node.h"

class Node;

class Edge {
public:
	
	Node *start = 0;
	Node *end = 0;

	std::vector <Node*> vertices;
	bool onFace;

	Edge();

	Edge(Node* start, Node* end, bool oF);

	float length();

	Point getCentre();

};
