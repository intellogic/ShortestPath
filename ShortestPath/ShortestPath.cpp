// ShortestPath.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "graph.h"
#include "point.h"

using namespace std;

int main(int argc, char* argv[])
{
	srand(time(NULL));

	string fileName = "octahedron.stl";
	string dir = "STL/";

	int k = 4;
	int m = 3;
	Graph *graph = new Graph(k, m, (dir + fileName).c_str());

	char c;
	while (true) {
		cout << "Resume?(y/n): ";
		cin >> c;
		if (c == 'n')
			return 0;
		graph->findTrueShortestPathBetweenRandomPoints(true);
	}

}
