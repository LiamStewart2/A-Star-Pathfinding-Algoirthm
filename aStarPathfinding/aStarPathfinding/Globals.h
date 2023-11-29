#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>

#define CENTER SDL_WINDOWPOS_CENTERED

const int CELL_SIZE = 20;

const int COLLUMNS = 30;
const int ROWS = 30;

const int WINDOW_WIDTH = COLLUMNS * CELL_SIZE;
const int WINDOW_HEIGHT = ROWS * CELL_SIZE;

enum CellStates {
	EMPTY, WALL, START, END, PATH
};