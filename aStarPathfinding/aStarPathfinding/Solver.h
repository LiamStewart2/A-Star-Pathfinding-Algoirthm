#pragma once
#include "Globals.h"
#include <algorithm>

#include "Node.h"

class Solver
{
public:
	Solver();
	~Solver();

	void handleEvents();
	void update();
	void render();

	bool IsRunning() { return isRunning; }
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	Node map[ROWS][COLLUMNS];

	bool isRunning = false;

	void initMap();
	void clearPath();
	void solve();

	void renderCells();
	void traceBack(Node* nextParent);

	void handleMouseDown();
	bool compareNodes(const Node* node1, const Node* node2);
	void removeNode(std::vector<Node*>& nodes, Node* node);
	bool CheckNode(std::vector<Node*>& nodes, Node* node);

	std::vector<Node*> openList;
	std::vector<Node*> closedList;
	std::vector<Node*> getNeighbours(int gridx, int gridy);

	Node* StartNode;
	Node* EndNode;
};

