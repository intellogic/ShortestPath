#include "stdafx.h"
#include "edge.h"

Edge::Edge() {}

Edge::Edge(Node* start, Node* end, bool oF) {
	this->start = start;
	this->end = end;
	this->onFace = oF;
}

float Edge::length() {
	return start->distanceTo(end);
}

Point Edge::getCentre() {
	return (start->point + end->point) / 2.0;
}