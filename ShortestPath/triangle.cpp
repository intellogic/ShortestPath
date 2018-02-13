#include "stdafx.h"
#include "triangle.h"
#include "helper.h"

Triangle::Triangle(Point n, Point v1, Point v2, Point v3) {
	this->normal = n;
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;
}

Triangle::Triangle(Point n, std::vector <Point> v) {
	normal = n;
	for (int i = 0; i < v.size(); i++)
		this->vertices[i] = v[i];
}

void Triangle::clear() {
	nodes.clear();
	edges.clear();
}

void Triangle::print() {
	std::cout << "---TRIANGLE---" << std::endl;
	std::cout << "NORMAL: ";
	normal.print();
	std::cout << std::endl;
	for (int i = 0; i < 3; i++, std::cout << std::endl) {
		std::cout << "V[" << i + 1 << "]: ";
		vertices[i].print();
	}
}

Point Triangle::getCentre() {
	return (vertices[0] + vertices[1] + vertices[2]) / 3.0;
}

bool Triangle::isPointInside(Point P) {
	Point A = this->vertices[0];
	Point B = this->vertices[1];
	Point C = this->vertices[2];

	Point v0 = C - A;
	Point v1 = B - A;
	Point v2 = P - A;

	float dot00 = dot(v0, v0);
	float dot01 = dot(v0, v1);
	float dot02 = dot(v0, v2);
	float dot11 = dot(v1, v1);
	float dot12 = dot(v1, v2);

	float invDenom = /*1.0 / */(dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) / invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) / invDenom;


	bool success = (P.distanceTo(A + v0 * u + v1 * v) < 0.001 && ((u > 0) || isEqual(u, 0.0)) && ((v > 0 || isEqual(v, 0.0))) && (u + v <= 1));
	return success;

}
