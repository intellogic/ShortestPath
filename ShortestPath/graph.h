#pragma once
#include "point.h"
#include "triangle.h"
#include "node.h"
#include "edge.h"
#include "stlfile.h"
#include "binary_heap.h"

const long int MaxSize = 1000000;

class Graph {

	std::string fileName;

	std::vector <Triangle> triangles;
	std::map <Point, Node*> nodesTree;
	std::map <std::pair<Node*, Node*>, Edge* > edgesTree;

	Node nodes[MaxSize];
	Edge edges[MaxSize];

	int nodesCount = 0;
	int edgesCount = 0;

	int k;
	int m;

	void createGraph();

	Node* addNodeToList(Point p);

	void connectNodes(Node *A, Node *B, bool onFace);

	void putNodeInsideTriangle(Node* node, Triangle &triangle);

	std::unordered_set <int> addNodeToMesh(Node *node);

	void findPath(Node *source, Node *destination, std::unordered_map <Node *, Node *> &path);


public:

	Graph(int k, int m, const char *fileName);

	std::vector <Point> findTrueShortestPathBetween(Point sourcePoint, Point destinationPoint, bool& success, float& pathLength);

	void visualize(Point& source, Point& target, std::vector <Point>& path, float pathLength);

	void findTrueShortestPathBetweenRandomPoints(bool visualizitaion);

	void findTrueShortestPathBetweenRandomTriangles(bool visualizitaion);

	void findTrueShortestPathBetweenRandomEdges(bool visualizitaion);
	
};