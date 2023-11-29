#include "Solver.h"

Solver::Solver()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("A-Star", CENTER, CENTER, WINDOW_WIDTH, WINDOW_HEIGHT, NULL);
	renderer = SDL_CreateRenderer(window, 0, NULL);
	isRunning = true;

	initMap();
	solve();
}

Solver::~Solver()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Solver::initMap()
{
	int startx = 3; int starty = 3;
	int endx = COLLUMNS - 4; int endy = ROWS - 4;
	SDL_Rect endNodePosition = SDL_Rect({
		endx * CELL_SIZE, endy * CELL_SIZE,
		CELL_SIZE, CELL_SIZE
	});
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLLUMNS; j++)
		{
			SDL_Rect nodePosition = SDL_Rect({ j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE });
			map[i][j] = Node(&nodePosition, &endNodePosition);
		}
	}
	map[starty][startx].state = START;
	map[endy][endx].state = END;

	StartNode = &map[starty][startx];
	EndNode = &map[endy][endx];
}

void Solver::handleEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			isRunning = false;
	}
	handleMouseDown();
}

void Solver::handleMouseDown()
{
	int mouseX; int mouseY;
	if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON_LEFT)
	{
		int cellX = (int)(floor(mouseX / CELL_SIZE));
		int cellY = (int)(floor(mouseY / CELL_SIZE));
		if (map[cellY][cellX].state == EMPTY || map[cellY][cellX].state == PATH) {
			map[cellY][cellX].state = WALL;
			solve();
		}
	}
}

void Solver::update()
{
}

void Solver::render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	renderCells();

	SDL_RenderPresent(renderer);
}

void Solver::renderCells()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLLUMNS; j++)
		{
			SDL_Rect cellPosition = SDL_Rect({ j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE });
			switch (map[i][j].state)
			{
			case START:
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				SDL_RenderFillRect(renderer, &cellPosition);
				break;
			case END:
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderFillRect(renderer, &cellPosition);
				break;
			case WALL:
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &cellPosition);
				break;
			case PATH:
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				SDL_RenderFillRect(renderer, &cellPosition);
				break;
			default:
				break;
			}

			// Render the lines of the grid //
			SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
			SDL_RenderDrawRect(renderer, &cellPosition);
		}
	}
}

void Solver::clearPath()
{
	openList.clear();
	closedList.clear();

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLLUMNS; j++)
		{
			if (map[j][i].state == PATH)
				map[j][i].state = EMPTY;
		}
	}
}

bool Solver::compareNodes(const Node* node1, const Node* node2)
{
	return node1->fCost() < node2->fCost();
}
void Solver::removeNode(std::vector<Node*>& nodes, Node* node)
{
	std::vector<Node*>::iterator theNode = std::find(nodes.begin(), nodes.end(), node);
	nodes.erase(theNode);
}
bool Solver::CheckNode(std::vector<Node*>& nodes, Node* node)
{
	std::vector<Node*>::iterator theNode = std::find(nodes.begin(), nodes.end(), node);
	if (theNode != nodes.end())
		return true;
	return false;
}
void Solver::traceBack(Node* nextParent)
{
	if (nextParent != nullptr) {
		if(nextParent->state != START)
			nextParent->state = PATH;
		traceBack(nextParent->parentNode);
	}
}

std::vector<Node*> Solver::getNeighbours(int gridx, int gridy)
{
	std::vector<Node*> neighbours;
	if (gridx > 0)
	{
		neighbours.push_back(&map[gridy][gridx - 1]);
		if (gridy > 0)
			neighbours.push_back(&map[gridy - 1][gridx - 1]);
		if (gridy < ROWS - 1)
			neighbours.push_back(&map[gridy + 1][gridx - 1]);
	}
	if (gridx < COLLUMNS - 1)
	{
		neighbours.push_back(&map[gridy][gridx + 1]);
		if (gridy > 0)
			neighbours.push_back(&map[gridy - 1][gridx + 1]);
		if (gridy < ROWS - 1)
			neighbours.push_back(&map[gridy + 1][gridx + 1]);
	}
	if (gridy > 0)
		neighbours.push_back(&map[gridy - 1][gridx]);
	if (gridy < ROWS - 1)
		neighbours.push_back(&map[gridy + 1][gridx]);

	return neighbours;
}


void Solver::solve()
{
	clearPath();
	openList.push_back(StartNode);

	while (openList.size() > 0)
	{
		//find the node in the open list with the lowest fcost
		auto currentNode = std::min_element(openList.begin(), openList.end(), [](const Node* node1, const Node* node2) {
			return node1->fCost() < node2->fCost();
		});
		Node* current = *currentNode;

		if (current->state == END)
		{
			//TRACE BACK PATH // ALGORITHM FINISHED
			traceBack(current->parentNode);
			openList.clear();
		}
		else
		{
			closedList.push_back(current);
			removeNode(openList, current);

			//gets all of the neighbour nodes
			std::vector<Node*> neighbours = getNeighbours(current->GridNodeX, current->GridNodeY);
			for (Node* successor : neighbours)
			{
				if(successor->state != WALL)
				{
					//if the successor node is not in the closed list
					if (!CheckNode(closedList, successor))
					{
						//determines how much the gcost should increase for the next node
						int increaseGcost = 10;
						if (successor->GridNodeX != current->GridNodeX && successor->GridNodeY != current->GridNodeY)
							increaseGcost = 14;
						//if the successor node is not in the open list
						if (!CheckNode(openList, successor))
						{
							//add it to the open list and set its parent node + gcost
							openList.push_back(successor);
							successor->parentNode = current;
							successor->gCost = current->gCost + increaseGcost;
						}
						//if the successor has found a more efficient path
						else if (successor->gCost > current->gCost + increaseGcost)
						{
							//replace the parent node and gcost
							successor->gCost = current->gCost + increaseGcost;
							successor->parentNode = current;
						}
					}
				}
			}
		}
	}
}