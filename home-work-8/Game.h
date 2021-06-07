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
	void* pAi;
	Field* pField{};
	Cell playerCell;
	Cell aiCell;
	int countForWin;
	Game();
	Game(int width, int height);
	Game(int width, int height, int countForWin);
	//Game(int size, int countForWin);
	~Game();

	void run();

	void handleState(State stateIfNotEnd);

	void makePlayerMove(int x, int y);

	void makeAiMove();

	DiffState getDiffState(int x, int y);

	virtual void doEndGame();

	bool isProgress();

	bool isEndGame();

	virtual Coords reqMove();

	// For call methods from field
	Cell getCell(int x, int y);

	void setCell(int x, int y, Cell cell);

	bool isEmpty(int x, int y);

	State getNewState();

	vector<Coords> getNotEmptyCells();

	std::vector<Coords> getEmptyCells();
};

