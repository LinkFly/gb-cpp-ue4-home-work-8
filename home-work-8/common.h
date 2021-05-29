#pragma once

#include "utils.h"

//using Cell = Field::Cell;
enum class Cell : char {
	unknown = '\0',
	empty = '_',
	x = 'X',
	o = '0'
};

enum State {
	UNKNOWN, IN_PROGRESS, PLAYER_MOVE, AI_MOVE, PLAYER_WON, AI_WON, DRAW
};

struct DiffState {
	int x, y;
	Cell oldCell;
	State state;
};
