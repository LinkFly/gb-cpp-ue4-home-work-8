#include "Ai.h"
#include "utils.h"

using namespace std;

DiffState Ai::tryMove(State& state, Coords coords)
{
	return tryMove(state, coords.x, coords.y);
}

DiffState Ai::tryMove(State& state, int x, int y)
{
	DiffState predDiff = pGameModel->getDiffState(x, y);
	pGameModel->setCell(x, y, cell);
	state = pGameModel->getNewState();
	return predDiff;
}

void Ai::restore(DiffState& move)
{
	pGameModel->setCell(move.x, move.y, move.oldCell);
	pGameModel->state = move.state;
}

void Ai::restore(stack<DiffState, vector<DiffState>>* pMoves)
{
	auto& moves = *pMoves;
	while (!moves.empty()) {
		DiffState& move = pMoves->top();
		restore(move);
		moves.pop();
	}
}

Coords Ai::mind()
{
	stack<DiffState, vector<DiffState>> diffs{};
	vector<Coords> emptyCells = pGameModel->getEmptyCells();
	Coords res{};
	bool foundMove = false;
	for (Coords& coords : emptyCells) {
		State state;
		DiffState diff = tryMove(state, coords);
		//diffs.push(diff);
		restore(diff);
		if (state == State::AI_WON) {
			res = coords;
			foundMove = true;
			break;
		}
	}
	if (!foundMove) {
		res = emptyCells[getRandom<size_t>() % emptyCells.size()];
	}
	restore(&diffs);
	return res;
}
