#pragma once

class Point {
	
	float x;
	float y;
	float z;

public:

	Point();

	Point(float x, float y, float z);

	Point(const Point& other);

	friend std::ostream& operator<<(std::ostream& os, Point& point);

	friend float dot(Point a, Point b);

	bool operator<(const Point &a) const;

	bool operator== (Point a) const;

	Point operator+(const Point& other) const;

	Point operator-() const;

	Point operator-(const Point& other);

	Point operator+=(const Point& other);

	Point operator-=(const Point& other);

	Point operator*(const float& scalar) const;

	Point operator/(const float& scalar) const;

	float distanceTo(const Point& other) const;

	void print();	
};

