#pragma once

#include "node.h"

class BinaryHeap {
	
	Node **nodes;
	std::unordered_map <Node *, int> indices;

	int parent(int node);

	int left(int node);

	int right(int node);

	void minHeapify(int node);

	void exchange(int nodeA, int nodeB);

	void heapify();

	void decreaseKey(int node);

public:

	int size;

	BinaryHeap(Node *n, int nodesCount);

	Node* getMin();

	void updateDistance(Node *node, float newDistance);

	bool contains(Node *node);
};