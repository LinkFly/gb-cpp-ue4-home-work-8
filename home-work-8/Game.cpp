#include "Game.h"
#include "Ai.h"

#include <cmath>

using namespace std;

Game::Game() : Game{ 3, 3 } {
	//field.init();
}

Game::Game(int width, int height) : Game{ width, height, min(width, height) }
{

}

Game::Game(int width, int height, int countForWin) :
	playerCell{ Cell::x }, aiCell{ Cell::o }, field{ width, height }, countForWin{ countForWin }
{
}

void Game::run()
{
	initRand();
	while (!bGameEnded) {
		field.draw();
		state = PLAYER_MOVE;
		Coords playerMove = reqMove();
		makePlayerMove(playerMove.x, playerMove.y);
		if (isEndGame()) break;
		makeAiMove();
	}
	doEndGame();
}

void Game::handleState(State stateIfNotEnd)
{
	state = getNewState();
	if (isEndGame()) {
		doEndGame();
		bGameEnded = true;
	}
	else state = stateIfNotEnd;
}

void Game::makePlayerMove(int x, int y)
{
	if (state != State::PLAYER_MOVE)
		error("Failed: now not player a move");
	if (!isEmpty(x, y))
		error("Cell isn't empty");
	setCell(x, y, playerCell);
	handleState(AI_MOVE);
}

void Game::makeAiMove()
{
	static Ai ai(*this, this->aiCell);
	if (state != State::AI_MOVE)
		error("Failed: not not ai a move");
	Coords aiMove;
	bool bAiMoved = ai.mind(aiMove);
	if (bAiMoved)
		setCell(aiMove.x, aiMove.y, aiCell);
	handleState(PLAYER_MOVE);
}

DiffState Game::getDiffState(int x, int y)
{
	return { x, y, getCell(x, y), state };
}

void Game::doEndGame()
{
	field.draw();
	switch (state) {
	case PLAYER_WON: cout << "Player WON!"; break;
	case AI_WON: cout << "Computer WON!"; break;
	case DRAW: cout << "DRAW!"; break;
	default: error("Failed: bad state");
	}
	bGameEnded = true;
}

bool Game::isProgress()
{
	return state == PLAYER_MOVE || state == AI_MOVE || state == IN_PROGRESS;
}

bool Game::isEndGame()
{
	return bGameEnded || !isProgress();
}

Coords Game::reqMove()
{
	int x, y;
	do {
		cout << "Your move (x y): ";
		cin >> x >> y;
		--x, --y;
		if (!isEmpty(x, y)) {
			cout << "Cell busy, try again...\n";
			x = -1;
		}
	} while (!(x >= 0 && y >= 0));
	return { x, y };
}

Cell Game::getCell(int x, int y)
{
	return field.getCell(x, y);
}

void Game::setCell(int x, int y, Cell cell)
{
	return field.setCell(x, y, cell);
}

State Game::getNewState()
{
	if (getEmptyCells().empty())
		return DRAW;
	static auto whoWon = [this](Cell cell) {
		if (cell == aiCell)
			return AI_WON;
		else if (cell == playerCell)
			return PLAYER_WON;
		return UNKNOWN;
	};
	auto& width = field.width;
	auto& height = field.height;
	function<Cell(int, int)> fnGetCell = [this](int x, int y) {return getCell(x, y); };
	function<Cell(int, int)> fnRevGetCell = [this](int x, int y) {return getCell(y, x); };
	auto checkHorizVert = [this](function<Cell(int, int)> fnGet, int width, int height) {
		Cell predCell = Cell::unknown;
		for (int y = 0; y < width; ++y) {
			//Cell curCell = Cell::unknown;
			int x = 0;
			for (; x < width; ++x)
			{
				predCell = fnGet(x, y);
				if (predCell != Cell::empty) {
					++x;
					break;
				}
			}
			if (x == width || (width - (x - 1)) < countForWin)
				continue;
			int count = 1;
			for (; x < width; ++x)
			{
				Cell curCell = fnGet(x, y);
				if (curCell != predCell)
					break;
				++count;
			}
			if (count >= countForWin)
				return whoWon(predCell);
		}
		return UNKNOWN;
	};

	State state = checkHorizVert(fnGetCell, width, height);
	if (state != UNKNOWN)
		return state;

	state = checkHorizVert(fnRevGetCell, height, width);
	if (state != UNKNOWN)
		return state;

	auto fnOnDiagonal = [this, &width, &height](function<bool(int& x, int& y)> fnNextXY, int x, int y) {
		Cell predCell = Cell::unknown;
		do {
			predCell = getCell(x, y);
			if (predCell != Cell::empty) {
				fnNextXY(x, y);
				break;
			}	
		} while(fnNextXY(x, y));
		if (x == width || (width - (x - 1)) < countForWin)
			return UNKNOWN;

		int count = 1;
		do {
			Cell curCell = getCell(x, y);
			if (curCell != predCell)
				break;
			++count;
		} while (fnNextXY(x, y));
		return (count < this->countForWin) ? UNKNOWN : whoWon(predCell);
	};

	auto fnNextXY = [&width, &height](int& x, int& y) {
		++x, ++y;
		return x < width && y < height;
	};
	auto fnNextXYMirror = [&width](int& x, int& y) {
		++x, --y;
		return x < width && y >= 0;
	};

	for (int x = 0; x <= width - countForWin; ++x)
	{
		state = fnOnDiagonal(fnNextXY, x, 0);
		if (state != UNKNOWN)
			return state;
	}
	for (int y = 0; y <= height - countForWin; ++y)
	{
		state = fnOnDiagonal(fnNextXY, 0, y);
		if (state != UNKNOWN)
			return state;
	}
	
	for (int x = 0; x <= width - countForWin; ++x)
	{
		state = fnOnDiagonal(fnNextXYMirror, x, height - 1);
		if (state != UNKNOWN)
			return state;
	}
	for (int y = height - 1; y >= height - countForWin; --y)
	{
		state = fnOnDiagonal(fnNextXYMirror, 0, y);
		if (state != UNKNOWN)
			return state;
	}

	return IN_PROGRESS;
}

std::vector<Coords> Game::getNotEmptyCells()
{
	vector<Coords> res;
	function<void(int, int, Cell)> callback = [&res](int x, int y, Cell cell) {
		if (cell != Cell::empty)
			res.push_back(Coords{ x, y });
	};
	field.doCells(callback);
	return res;
}

std::vector<Coords> Game::getEmptyCells()
{
	vector<Coords> res;
	function<void(int, int, Cell)> callback = [&res](int x, int y, Cell cell) {
		if (cell == Cell::empty)
			res.push_back(Coords{ x, y });
	};
	field.doCells(callback);
	return res;
}
