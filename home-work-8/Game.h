#pragma once

#include <iostream>
//#include <windows.h>
#include <iostream>
#include <stack>
#include <vector>
#include <functional>

#include "utils.h"
#include "common.h"
#include "Field.h"
//#include "Ai.h"

using namespace std;

struct Game {
	bool bGameEnded = false;
	State state = State::PLAYER_MOVE;
	Field field{};
	Cell playerCell;
	Cell aiCell;
	int countForWin;
	Game();
	Game(int width, int height);
	Game(int width, int height, int countForWin);

	void run();

	void handleState(State stateIfNotEnd);

	void makePlayerMove(int x, int y);

	void makeAiMove();

	DiffState getDiffState(int x, int y);

	void doEndGame();

	bool isProgress();

	bool isEndGame();

	Coords reqMove();

	// For call methods from field
	Cell getCell(int x, int y);

	void setCell(int x, int y, Cell cell);

	bool isEmpty(int x, int y) {
		return field.isEmpty(x, y);
	}

	State getNewState();

	vector<Coords> getNotEmptyCells();

	std::vector<Coords> getEmptyCells();
};

