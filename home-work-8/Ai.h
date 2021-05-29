#pragma once

#include "Game.h"
#include "common.h"
#include "Collection.h"

#include <functional>

struct Ai {
	Game* pGameModel;
	Cell& cell;
	Ai(Game& game, Cell& cell) : pGameModel{ &game }, cell{ cell } {}
	DiffState tryMove(State& state, Coords coords, Cell cell = Cell::unknown);
	DiffState tryMove(State& state, int x, int y, Cell cell = Cell::unknown);
	function<bool(Collection& collect, Coords& otherCoords)> emptyFunc{};
		//= [](Collection& collect, Coords& otherCoords) {};
	void checkWon(
		Collection& collect, State whoWon,
		Coords& resCoords, bool& foundMove,
		function<bool(Collection& collect, Coords& otherCoords)>* fnCheckNextWon = nullptr);
	void restore(DiffState& move);
	void restore(stack<DiffState>& moves);
	bool mind(Coords& res);
};