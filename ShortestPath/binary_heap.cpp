#include "stdafx.h"
#include "binary_heap.h"

int BinaryHeap::parent(int node) {
	return (node - 1) / 2;
}

int BinaryHeap::left(int node) {
	return node * 2 + 1;
}

int BinaryHeap::right(int node) {
	return node * 2 + 2;
}

void BinaryHeap::minHeapify(int node) {
	int leftNode = left(node);
	int rightNode = right(node);
	int minNode = node;

	if (leftNode < size && nodes[leftNode]->distance < nodes[minNode]->distance)
		minNode = leftNode;

	if (rightNode < size && nodes[rightNode]->distance < nodes[minNode]->distance)
		minNode = rightNode;

	if (minNode != node) {
		exchange(node, minNode);
		minHeapify(minNode);
	}
}

void BinaryHeap::exchange(int nodeA, int nodeB) {
	indices[nodes[nodeA]] = nodeB;
	indices[nodes[nodeB]] = nodeA;
	std::swap(nodes[nodeA], nodes[nodeB]);
}


void BinaryHeap::heapify() {
	for (int i = (size + 1) / 2; i >= 1; i--)
		minHeapify(i);
}

void BinaryHeap::decreaseKey(int node) {
	int currentNode = node;
	while (currentNode > 0 && nodes[parent(currentNode)]->distance > nodes[currentNode]->distance) {
		exchange(currentNode, parent(currentNode));
		currentNode = parent(currentNode);
	}
}

BinaryHeap::BinaryHeap(Node *n, int nodesCount) {
		this->nodes = new Node*[nodesCount];
		for (int i = 0; i < nodesCount; i++) {
			this->nodes[i] = &n[i];
			indices.insert(std::make_pair(&n[i], i));
		}
		size = nodesCount;
		heapify();
}

Node* BinaryHeap::getMin() {
	Node *top = nodes[0];
	indices[top] = -1;
	exchange(0, size - 1);
	size--;
	minHeapify(0);
	return top;
}

void BinaryHeap::updateDistance(Node *node, float newDistance) {
	node->distance = newDistance;
	decreaseKey(indices[node]);
}

bool BinaryHeap::contains(Node *node) {
	return indices[node] != -1;
}