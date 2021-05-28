#pragma once

struct Coords { int x, y; };

//using Cell = Field::Cell;
enum class Cell : char {
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
