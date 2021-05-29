#include "Ai.h"
#include "utils.h"
#include "Collection.h"

#include <unordered_map>

using namespace std;

DiffState Ai::tryMove(State& state, Coords coords, Cell cell)
{
	return tryMove(state, coords.x, coords.y, cell);
}

DiffState Ai::tryMove(State& state, int x, int y, Cell cell)
{
	DiffState predDiff = pGameModel->getDiffState(x, y);
	pGameModel->setCell(x, y, cell == Cell::unknown ? this->cell : cell);
	state = pGameModel->getNewState();
	return predDiff;
}


void Ai::restore(DiffState& move)
{
	pGameModel->setCell(move.x, move.y, move.oldCell);
	pGameModel->state = move.state;
}

void Ai::restore(stack<DiffState>& moves)
{
	//auto& moves = *pMoves;
	while (!moves.empty()) {
		DiffState& move = moves.top();
		restore(move);
		moves.pop();
	}
}

void Ai::checkWon(Collection& collect, State whoWon, 
					Coords& resCoords, bool& foundMove,
					function<bool(Collection& collect, Coords& otherCoords)>* fnCheckNextWon)
{
	auto fnIfFound = [this, &resCoords, &foundMove](DiffState& diff, Coords coords) {
		resCoords = coords;
		foundMove = true;
		restore(diff);
	};
	size_t curIdx;
	Coords* pCurCoords;
	while (pCurCoords = collect.next(curIdx)) {
		Coords& coords = *pCurCoords;
		State state;
		DiffState diff = tryMove(state, coords, whoWon == AI_WON ? cell : pGameModel->playerCell);
		if (state == whoWon) {
			fnIfFound(diff, coords);
			return;
		}

		if (fnCheckNextWon) {
			Collection nestedCollect(collect);
			nestedCollect.addExclude(curIdx);
			Coords otherCoords;
			bool otherWin = (*fnCheckNextWon)(nestedCollect, otherCoords);
			if (otherWin) {
				fnIfFound(diff, otherCoords);
				return;
			}
		}

		restore(diff);
	}
	foundMove = false;
}

bool Ai::mind(Coords& res)
{
	stack<DiffState> diffs{};
	vector<Coords> emptyCells = pGameModel->getEmptyCells();
	if (emptyCells.empty()) 
		return false;
	bool foundMove = false;
	Collection collect(emptyCells);
	//Coords* pCurCoords;
	//size_t curIdx;
	function<bool(Collection&, Coords&)> fnCheckOther = [this, &foundMove](Collection& collect, Coords& otherCoords) {
		checkWon(collect, State::PLAYER_WON, otherCoords, foundMove);
		return foundMove;
	};
	checkWon(collect, State::AI_WON, res, foundMove, &fnCheckOther);

//	while (pCurCoords = collect.next(curIdx)) {
//		Coords& coords = *pCurCoords;
//		State state;
//		DiffState diff = tryMove(state, coords);
//		diffs.push(diff);
//		if (state == State::AI_WON) {
//			res = coords;
//			foundMove = true;
//			goto mEndAnalyze;
//		}
//		
//		Collection nestedCollect(emptyCells);
//		nestedCollect.addExclude(curIdx);
//		while (pCurCoords = nestedCollect.next())
//		{
//			State state;
//			DiffState diff = tryMove(state, coords);
//			diffs.push(diff);
//			if (state == State::PLAYER_WON) {
//				res = *pCurCoords;
//				foundMove = true;
//				goto mEndAnalyze;
//			}
//			restore(diff);
//			diffs.pop();
//		}
//		restore(diff);
//		diffs.pop();
//	}
//mEndAnalyze:
//	restore(diffs);
	if (!foundMove) {
		res = emptyCells[getRandom<size_t>() % emptyCells.size()];
	}
	return true;
}
