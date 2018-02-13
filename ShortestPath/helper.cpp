#include "helper.h"
#include "stdafx.h"
#include "helper.h"

float eps = 1e-4;
float pathAccuracyEps = 1e-9;

const long int MaxN = 1000000;

bool isEqual(float a, float b) {
	if (fabs(a - b) < eps)
		return true;
	return false;
}

std::ostream& operator<<(std::ostream& os, Point& point) {
	os << point.x << " " << point.y << " " << point.z;
	return os;
}

float dot(Point a, Point b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

