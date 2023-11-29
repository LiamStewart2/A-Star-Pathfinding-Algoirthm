#include "Node.h"

Node::Node(SDL_Rect* nodePosition, SDL_Rect* endNodePosition)
{
	int xDistance = endNodePosition->x - nodePosition->x;
	int yDistance = endNodePosition->y - nodePosition->y;

	GridNodeX = (int)(nodePosition->x / CELL_SIZE);
	GridNodeY = (int)(nodePosition->y / CELL_SIZE);

	hCost = (int)(sqrt(abs(xDistance * xDistance) + abs(yDistance * yDistance)));
}