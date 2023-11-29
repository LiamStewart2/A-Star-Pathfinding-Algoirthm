#pragma once
#include "Globals.h"
class Node
{
public:
	Node(SDL_Rect* nodePosition, SDL_Rect* endNodePosition);
	Node() {}
	~Node() {}

	int fCost() const { return gCost + hCost; }
	int hCost; int gCost = 0;

	Node* parentNode = nullptr;
	CellStates state = EMPTY;

	int GridNodeX; int GridNodeY;
};

