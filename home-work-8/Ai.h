#pragma once

#include "Game.h"
#include "common.h"

struct Ai {
	//using Cell = typename Field::Cell;
	//using State = typename Game::State;
	//using DiffState = typename Game::DiffState;
	Game* pGameModel;
	//Game* gameModel;
	//Game& game;
	Cell& cell;
	Ai(Game& game, Cell& cell) : pGameModel{ &game }, cell{ cell } {}
	DiffState tryMove(State& state, Coords coords);
	DiffState tryMove(State& state, int x, int y);
	void restore(DiffState& move);
	void restore(stack<DiffState, vector<DiffState>>* pMoves);
	Coords mind();
};