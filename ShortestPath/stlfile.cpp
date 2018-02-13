#include "stdafx.h"
#include "stlfile.h"

void stlFile::readBytes(int number) {
	char *bytes = new char[number];
	file->read(bytes, number);
}

std::string stlFile::readHeader() {
	char header[80];
	file->read(header, 80);

	return std::string(header);
}

unsigned stlFile::readUInt() {
	char numberChar[4];
	file->read(numberChar, 4);
	unsigned *number = (unsigned *)numberChar;

	return *number;
}

float stlFile::readfloat() {
	char floatChar[4];
	file->read(floatChar, 4);
	float *floatNum = (float *)floatChar;

	return (float)*floatNum;
}

Point stlFile::readPoint() {
	float x = readfloat();
	float y = readfloat();
	float z = readfloat();

	return Point(x, y, z);
}

Triangle stlFile::readTriangle() {
	Point normal = readPoint();
	Point v1 = readPoint();
	Point v2 = readPoint();
	Point v3 = readPoint();
	readBytes(2);
	return Triangle(normal, v1, v2, v3);
}

	stlFile::stlFile(const char* fileName) {
		file = new std::ifstream(fileName, std::ios_base::in | std::ios_base::binary);
	}

	std::vector <Triangle> stlFile::read() {
		std::vector <Triangle> triangles;
		std::string header = readHeader();
		unsigned numberOfTriangles = readUInt();

		for (unsigned int index = 0; index < numberOfTriangles; index++) {
			std::vector <Point> vertices;
			Point normal = readPoint();
			vertices.push_back(readPoint());
			vertices.push_back(readPoint());
			vertices.push_back(readPoint());
			readBytes(2);
			Triangle triangle = Triangle(normal, vertices);
			triangles.push_back(triangle);
		}

		file->close();

		return triangles;
	}
