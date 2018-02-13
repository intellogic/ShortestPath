#pragma once

#include "point.h"
#include "triangle.h"

class stlFile {

	std::ifstream *file;

	void readBytes(int number);

	std::string readHeader();

	unsigned readUInt();

	float readfloat();

	Point readPoint();

	Triangle readTriangle();

public:
	
	stlFile(const char* fileName);

	std::vector <Triangle> read();

};