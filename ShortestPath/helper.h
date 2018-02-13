#pragma once

#include "point.h"

extern float eps;
extern float pathAccuracyEps ;

extern const long int MaxN;

extern bool isEqual(float a, float b);

extern std::ostream& operator<<(std::ostream& os, Point& point);

extern float dot(Point a, Point b);

