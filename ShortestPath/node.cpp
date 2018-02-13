#include "stdafx.h"
#include "node.h"

Node::Node() {}

Node::Node(Point p) {
	this->point = p;
	distance = 10000.0;
}

bool Node::operator== (Node a) const {
	return this->point == a.point;
}

bool Node::operator< (Node a)const {
	return this->point < a.point;
}

float Node::distanceTo(Node *other) {
	return this->point.distanceTo(other->point);
}