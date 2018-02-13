#include "stdafx.h"
#include "graph.h"
#include "helper.h"

using namespace std;

Graph::Graph(int k, int m, const char *fileName) {
	this->fileName = fileName;
	stlFile file(fileName);
	triangles = file.read();
	cout << "STL file with " << triangles.size() << " triangles was read" << endl;
	this->k = k;
	this->m = m;
	createGraph();
}

void Graph::createGraph() {
	nodesTree.clear();
	edgesTree.clear();
	if (nodesCount != 0) {
		for (Triangle& triangle : triangles)
			triangle.clear();
	}
	
	nodesCount = 0;
	edgesCount = 0;

	for (size_t triangleIndex = 0; triangleIndex < triangles.size(); triangleIndex++) {

		Triangle triangle = triangles[triangleIndex];

		vector <Node *> currentNodes;

		vector <Edge *> currentEdges;

		for (int pointIndex = 0; pointIndex < 3; pointIndex++) {
			Point p = triangle.vertices[pointIndex];
			auto searchResult = nodesTree.find(p);
			Node *currentNodeLink;
			if (searchResult == nodesTree.end()) {
				nodes[nodesCount] = Node(p);
				currentNodeLink = &nodes[nodesCount];
				nodesTree.insert(make_pair(p, &nodes[nodesCount]));
				nodesCount++;
			}
			else {
				currentNodeLink = searchResult->second;
			}
			currentNodes.push_back(currentNodeLink);
			triangles[triangleIndex].nodes.push_back(currentNodeLink);
		}



		for (int i = 0; i < 2; i++) {
			for (int j = i + 1; j < 3; j++) {
				Node *nodeA = currentNodes[i];
				Node *nodeB = currentNodes[j];
				if (nodeA > nodeB)
					swap(nodeA, nodeB);
				int nodeCIndex = 0;
				if (i == 0 && j == 1)
					nodeCIndex = 2;
				else if (i == 0 && j == 2)
					nodeCIndex = 1;

				Node *nodeC = currentNodes[nodeCIndex];

				float edgeLength = nodeA->distanceTo(nodeB);

				Edge *currentEdge;
				auto searchResult = edgesTree.find(make_pair(nodeA, nodeB));
				if (searchResult == edgesTree.end()) {
					edges[edgesCount] = Edge(nodeA, nodeB, false);
					currentEdge = &edges[edgesCount];
					edgesTree.insert(make_pair(make_pair(nodeA, nodeB), currentEdge));
					edgesCount++;

					Point delta = (nodeB->point - nodeA->point) / (float)(k + 1);
					Point currentPoint = nodeA->point;

					Node* previousNode = nodeA;
					Node* currentNode;
					for (int t = 0; t < k; t++) {
						currentPoint += delta;
						nodes[nodesCount] = Node(currentPoint);
						currentNode = &nodes[nodesCount];
						nodesTree.insert(make_pair(currentPoint, currentNode));
						currentEdge->vertices.push_back(currentNode);
						connectNodes(currentNode, previousNode, false);
						previousNode = currentNode;
						nodesCount++;
					}
					connectNodes(previousNode, nodeB, false);

				}
				else {
					currentEdge = searchResult->second;

				}
				for (Node* currentNode : currentEdge->vertices) {
					connectNodes(nodeC, currentNode, true);
				}
				currentEdges.push_back(currentEdge);
				triangles[triangleIndex].edges.push_back(currentEdge);
			}
		}



		for (int i = 0; i < 2; i++) {
			for (int j = i + 1; j < 3; j++) {
				Edge *edgeA = currentEdges[i];
				Edge *edgeB = currentEdges[j];

				for (size_t edgeAIndex = 0; edgeAIndex < edgeA->vertices.size(); edgeAIndex++) {
					for (size_t edgeBIndex = 0; edgeBIndex < edgeB->vertices.size(); edgeBIndex++) {
						Node *edgeANode = edgeA->vertices[edgeAIndex];
						Node *edgeBNode = edgeB->vertices[edgeBIndex];
						connectNodes(edgeANode, edgeBNode, true);
					}
				}
			}
		}

	}

}

Node* Graph::addNodeToList(Point p) {
	if (nodesTree[p] != 0) {
		return nodesTree[p];
	}
	nodes[nodesCount] = Node(p);
	Node *currentNode = &nodes[nodesCount];
	nodesTree[p] = currentNode;
	nodesCount++;
	return currentNode;
}

void Graph::connectNodes(Node *A, Node *B, bool onFace) {
	Node *nodeA = A;
	Node *nodeB = B;
	
	if (nodeA == 0 || nodeB == 0)
		return;
	if (nodeA == nodeB)
		return;

	if (nodeA > nodeB)
		swap(nodeA, nodeB);

	if (edgesTree[make_pair(nodeA, nodeB)] == 0) {
		nodeA->adj.push_back(nodeB);
		nodeB->adj.push_back(nodeA);
		edges[edgesCount] = Edge(nodeA, nodeB, onFace);
		Edge *currentEdge = &edges[edgesCount];
		edgesTree.insert(make_pair(make_pair(nodeA, nodeB), currentEdge));
		nodeA->adjEdges.push_back(currentEdge);
		nodeB->adjEdges.push_back(currentEdge);
		edgesCount++;
	}
}

void Graph::putNodeInsideTriangle(Node* node, Triangle &triangle) {
	for (int i = 0; i < 3; i++) {
		if (node->point == triangle.vertices[i]) {
			return;
		}
	}

	for (int i = 0; i < 3; i++)
		connectNodes(node, triangle.nodes[i], true);

	for (int i = 0; i < 3; i++) {
		Edge *currentEdge = triangle.edges[i];
		for (size_t j = 0; j < currentEdge->vertices.size(); j++)
			connectNodes(node, currentEdge->vertices[j], true);
	}
}

unordered_set <int> Graph::addNodeToMesh(Node *node) {
	unordered_set <int> adjTriangles;
	for (size_t i = 0; i < triangles.size(); i++) {

		bool liesOnTriangle = false;

		for (int j = 0; j < 3; j++) {
			if (node->point == triangles[i].vertices[j]) {
				liesOnTriangle = true;
				break;
			}
		}

		if (liesOnTriangle) {
			adjTriangles.insert(i);
			continue;
		}

		for (int j = 0; j < 3; j++) {
			Edge *currentEdge = triangles[i].edges[j];
			for (Node* SP : currentEdge->vertices)
				if (SP == node) 
					liesOnTriangle = true;
				
			if (liesOnTriangle)
				break;
			if (isEqual(currentEdge->length(), node->distanceTo(currentEdge->start) + node->distanceTo(currentEdge->end))) {
				liesOnTriangle = true;
				connectNodes(node, currentEdge->start, false);
				connectNodes(node, currentEdge->end, false);

				for (Node* triangleNode : triangles[i].nodes)
					if (triangleNode != currentEdge->start && triangleNode != currentEdge->end) {
						connectNodes(node, triangleNode, true);
					}
				for (Node* SP : currentEdge->vertices)
					connectNodes(node, SP, false);
				for (Edge *triangeEdge : triangles[i].edges) {
					if (triangeEdge != currentEdge) {
						for (Node* SP : triangeEdge->vertices) {
							connectNodes(node, SP, true);
						}
					}
				}
				liesOnTriangle = true;
				break;
			}
		}


		if (liesOnTriangle) {
			adjTriangles.insert(i);
			continue;
		}

		if (triangles[i].isPointInside(node->point))
			liesOnTriangle = true;

		if (liesOnTriangle) {
			adjTriangles.insert(i);
			putNodeInsideTriangle(node, triangles[i]);
		}

	}

	return adjTriangles;
}

void Graph::findPath(Node *source, Node *destination, unordered_map <Node *, Node *> &previous) {
	for (int i = 0; i < nodesCount; i++)
		nodes[i].distance = 10000000.0;
	BinaryHeap heap(nodes, nodesCount);
	heap.updateDistance(source, 0.0);

	while (heap.size > 0) {
		Node *currentNode = heap.getMin();
		for (size_t i = 0; i < currentNode->adj.size(); i++) {
			Node *currentAdjNode = currentNode->adj[i];
			if (heap.contains(currentAdjNode) && currentAdjNode != currentNode) {
				float distanceFromCurrentNode = currentNode->distance + currentNode->distanceTo(currentAdjNode);
				if (currentAdjNode->distance > distanceFromCurrentNode) {
					previous[currentAdjNode] = currentNode;
					heap.updateDistance(currentAdjNode, distanceFromCurrentNode);
				}
			}
		}
	}
}


vector <Point> Graph::findTrueShortestPathBetween(Point sourcePoint, Point destinationPoint, bool& success, float& pathLength) {
	
	createGraph();
	
	vector <Point> trueShortestPath;

	Node *source = addNodeToList(sourcePoint);
	Node *destination = addNodeToList(destinationPoint);

	success = true;

	unordered_set <int> sourceAdjTriangles = addNodeToMesh(source);
	unordered_set <int> destinationAdjTriangles = addNodeToMesh(destination);

	if (sourceAdjTriangles.size() == 0 || destinationAdjTriangles.size() == 0)
	{
		success = false;
		return trueShortestPath;
	}


	for (auto sourceTriangle : sourceAdjTriangles) {
		for (auto destinationTriangle : destinationAdjTriangles) {
			if (sourceTriangle == destinationTriangle) {
				trueShortestPath.push_back(source->point);
				trueShortestPath.push_back(destination->point);
				pathLength = source->distanceTo(destination);
				return trueShortestPath;
			}
		}
	}

	float currentPathLength = 1e+7;

	vector <Node *> path;

	while (true) {

		unordered_map <Node *, Node *> previous;

		findPath(source, destination, previous);

		if (previous[destination] == 0) {
			success = false;
			return trueShortestPath;
		}

		Node* currentNodeInPath = destination;

		vector <Node *> currentPath;

		while (currentNodeInPath != 0) {
			currentPath.push_back(currentNodeInPath);
			currentNodeInPath = previous[currentNodeInPath];
		}
		
		int leftIndex = 0, rightIndex = currentPath.size() - 1;

		while (leftIndex < rightIndex)
			swap(currentPath[leftIndex++], currentPath[rightIndex--]);


		if (currentPathLength - destination->distance > pathAccuracyEps) {
			currentPathLength = destination->distance;
			path = currentPath;
		}
		else {
			for (size_t nodeIndex = 0; nodeIndex < path.size(); nodeIndex++)
				trueShortestPath.push_back(path[nodeIndex]->point);
			pathLength = currentPathLength;
			return trueShortestPath;
		}


		for (size_t i = 0; i < currentPath.size(); i++) {
			currentPath[i]->adj.clear();
			currentPath[i]->SPs.clear();
		}


		for (size_t i = 1; i < currentPath.size() - 1; i++) {
			Node *current = currentPath[i];
			Node *previous = currentPath[i - 1];
			Node *next = currentPath[i + 1];

			vector <Edge *> originalEdges;
			for (size_t j = 0; j < current->adjEdges.size(); j++)
				if (!current->adjEdges[j]->onFace)
					originalEdges.push_back(current->adjEdges[j]);


			current->adjEdges.clear();

			if (originalEdges.size() == 2) {

				for (size_t j = 0; j < originalEdges.size(); j++) {
					Edge *currentDividedEdge = originalEdges[j];
					Node *nodeA = current;
					Node *nodeB;

					if (currentDividedEdge->start == nodeA)
						nodeB = currentDividedEdge->end;
					else
						nodeB = currentDividedEdge->start;

					if (nodeB == previous || nodeB == next)
						break;

					nodeB->adj.clear();
					nodeB->SPs.clear();
					nodeB->adjEdges.clear();

					Point deltaPoint = (nodeB->point - nodeA->point) / (float)(m + 1);
					Point currentPoint = nodeA->point;

					Node *currentNode;
					Node *previousNode = nodeA;

					for (int t = 0; t < m; t++) {
						currentPoint += deltaPoint;
						nodes[nodesCount] = Node(currentPoint);
						currentNode = &nodes[nodesCount];
						nodesTree.insert(make_pair(currentPoint, currentNode));
						connectNodes(previousNode, currentNode, false);
						current->SPs.push_back(currentNode);
						previousNode = currentNode;
						nodesCount++;
					}
					connectNodes(previousNode, nodeB, false);
					current->SPs.push_back(nodeB);
				}
			}

		}

		edgesCount = 0;
		edgesTree.clear();


		for (size_t index = 0; index < currentPath.size() - 1; index++) {
			Node *current = currentPath[index];
			Node *next = currentPath[index + 1];

			vector <Node *> currentNodePoints(current->SPs);
			currentNodePoints.push_back(current);
			vector <Node *> nextNodePoints(next->SPs);
			nextNodePoints.push_back(next);


			for (size_t currentIndex = 0; currentIndex < currentNodePoints.size(); currentIndex++) {
				for (size_t nextIndex = 0; nextIndex < nextNodePoints.size(); nextIndex++) {
					connectNodes(currentNodePoints[currentIndex], nextNodePoints[nextIndex], true);
				}
			}
		}

	}
	
}

void Graph::visualize(Point& source, Point& target, vector <Point>& path, float pathLength) {
	string pythonCommand = "python STL/visualization.py ";
	ofstream out("STL/path.txt");
	
	cout << "The true shortest path from point A(" << source << ") to B(" << target << ")" << endl;
	cout << "Path length = " << pathLength << endl;
	cout << "Path:" << endl;

	for (size_t i = 0; i < path.size(); i++) {
		cout << path[i] << endl;
		if (i != path.size() - 1) {
			out << path[i] << " " << path[i + 1] << endl;
		}
	}
	out.close();
	system((pythonCommand + fileName).c_str());
}


void Graph::findTrueShortestPathBetweenRandomPoints(bool visualizitaion) {
	int i = rand() % nodesCount;
	int j = rand() % nodesCount;
	while (j == i)
		j = rand() % nodesCount;

	bool success;
	float pathLength;
	vector <Point> path = findTrueShortestPathBetween(nodes[i].point, nodes[j].point, success, pathLength);
	if (success && visualizitaion)
		visualize(nodes[i].point, nodes[j].point, path, pathLength);
}

void Graph::findTrueShortestPathBetweenRandomTriangles(bool visualizitaion) {
	int trianglesCount = triangles.size();
	int i = rand() % trianglesCount;
	int j = rand() % trianglesCount;
	while (j == i)
		j = rand() % trianglesCount;

	bool success;
	float pathLength;
	Point source = triangles[i].getCentre(), target = triangles[j].getCentre();

	vector <Point> path = findTrueShortestPathBetween(source, target, success, pathLength);

	if (success)
		visualize(source, target, path, pathLength);
}

void Graph::findTrueShortestPathBetweenRandomEdges(bool visualizitaion) {
	int i = rand() % edgesCount;
	int j = rand() % edgesCount;
	while (j == i)
		j = rand() % edgesCount;

	bool success;
	float pathLength;
	Point source = edges[i].getCentre(), target = edges[j].getCentre();

	vector <Point> path = findTrueShortestPathBetween(source, target, success, pathLength);

	if (success && visualizitaion)
		visualize(source, target, path, pathLength);
}

