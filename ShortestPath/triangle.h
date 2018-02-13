#pragma once

#include "point.h"
#include "edge.h"
#include "node.h"


class Triangle {
public:
	
	Point normal;
	Point vertices[3];
	std::vector <Node *> nodes;
	std::vector <Edge *> edges;

	Triangle(Point n, Point v1, Point v2, Point v3);

	Triangle(Point n, std::vector <Point> v);

	void clear();

	void print();

	Point getCentre();

	bool isPointInside(Point P);
};
