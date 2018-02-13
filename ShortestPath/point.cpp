#include "stdafx.h"
#include "point.h"
#include "helper.h"

Point::Point() {}

Point::Point(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Point::Point(const Point& other) : Point(other.x, other.y, other.z) {

}

bool Point::operator<(const Point &a) const {
	if (!isEqual(this->x, a.x))
		return this->x < a.x;
	if (!isEqual(this->y, a.y))
		return this->y < a.y;

	return this->z < a.z;
}

bool Point::operator== (Point a) const {
	return isEqual(this->distanceTo(a), 0.0);
}

Point Point::operator+(const Point& other) const {
	return Point(this->x + other.x, this->y + other.y, this->z + other.z);
}

Point Point::operator-() const {
	return Point(-this->x, -this->y, -this->z);
}

Point Point::operator-(const Point& other) {
	return (*this) + (-other);
}

Point Point::operator+=(const Point& other) {
	*this = *this + other;
	return *this;
}

Point Point::operator-=(const Point& other) {
	*this = *this - other;
	return *this;
}

Point Point::operator*(const float& scalar) const {
	return Point(this->x * scalar, this->y * scalar, this->z * scalar);
}

Point Point::operator/(const float& scalar) const {
	return Point(this->x / scalar, this->y / scalar, this->z / scalar);
}

float Point::distanceTo(const Point& other) const {
	return sqrt((this->x - other.x)*(this->x - other.x) + (this->y - other.y)*(this->y - other.y) + (this->z - other.z)*(this->z - other.z));
}

void Point::print() {
	std::cout << x << " " << y << " " << z;
}