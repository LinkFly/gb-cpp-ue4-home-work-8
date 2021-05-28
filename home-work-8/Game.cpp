#include "Game.h"
#include "Ai.h"

Game::Game(): playerCell{ Cell::x }, aiCell{ Cell::o }/*, ai{ *this, aiCell }*/ {

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
	Coords aiMove = ai.mind();
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
	return !isProgress();
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
		for (int y = 0; y < height; ++y) {
			//Cell curCell = Cell::unknown;
			Cell predCell = fnGet(0, y);
			int x = 1;
			for (; x < width; ++x) {
				Cell curCell = fnGet(x, y);
				if (curCell == Cell::empty)
					break;
				if (curCell != predCell)
					break;
				predCell = curCell;
			}
			if (x == width)
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

	Cell predCell = getCell(0, 0);
	int x = 1, y = 1;
	for (; x < width; ++x, ++y) {
		Cell curCell = getCell(x, y);
		if (curCell == Cell::empty)
			break;
		if (curCell != predCell)
			break;
		predCell = curCell;
	}
	if (x == width)
		return whoWon(predCell);

	predCell = getCell(0, height - 1);
	x = 1, y = height - 2;
	for (; x < width; ++x, --y) {
		Cell curCell = getCell(x, y);
		if (curCell == Cell::empty)
			break;
		if (curCell != predCell)
			break;
		predCell = curCell;
	}
	if (x == width)
		return whoWon(predCell);

	if (getNotEmptyCells().empty())
		return DRAW;
	else return IN_PROGRESS;
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
