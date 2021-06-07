#include "pch.h"

#include "Game.h"
#include "Ai.h"
#include "Field.h"

#include <vector>

using namespace std;

struct TestField : public Field {
	void clear() override {
		for (auto& cellsLine : cells)
			cellsLine.clear();
		cells.clear();
	}
	void outCell(int x, int y, Cell cell) override {
		cells[x][y] = cell;
	}
	void draw() override {}
	TestField(int size): Field{size} {}

	vector<vector<Cell>> cells;
};

struct TestData {
	vector<Coords> playerMoves = {{0, 0}, {1, 0}, {2, 0}};
	vector<Coords> aiMoves = { {0, 1}, {1, 1}, {2, 1} };
	// States after move
	vector<State> states = { PLAYER_MOVE, AI_MOVE, PLAYER_MOVE, AI_MOVE, PLAYER_MOVE, PLAYER_WON};
	TestData() = default;
	TestData(const vector<Coords>& playerMoves, const vector<Coords>& aiMoves, const vector<State>& states):
		playerMoves{playerMoves}, aiMoves{aiMoves}, states{states}
	{}
};

struct TestGame : public Game {
	size_t moveIdx = 0;
	bool testResult{ false };
	TestData* pTestData;
	TestGame(TestData* pTestData, int size, int countForWin);
	void doEndGame() override;
	Coords reqMove() override;
};

struct TestAi : public Ai {
	TestData* pTestData;
	//size_t moveIdx = 0;
	TestAi(TestData* pTestData, TestGame* game, Cell& cell): Ai{game, cell} {
		this->pTestData = pTestData;
	}
	bool mind(Coords& res) override {
		TestGame& game = *reinterpret_cast<TestGame*>(pGameModel);
		game.testResult = pTestData->states[game.moveIdx] == pGameModel->state;
		if (!game.testResult) {
			pGameModel->bGameEnded = true;
			return {};
		}

		res = pTestData->aiMoves[game.moveIdx++ / 2];
		return true;
	}
};

TestGame::TestGame(TestData* pTestData, int size, int countForWin) : Game(size, size, countForWin) {
	delete pField;
	pField = new TestField{ size };
	this->pTestData = pTestData;
	delete pAi;
	pAi = new TestAi{ pTestData , this, aiCell };
}

void TestGame::doEndGame() {
	//bGameEnded = true;
	testResult = pTestData->states[moveIdx] == state;

}

Coords TestGame::reqMove() {
	testResult = pTestData->states[moveIdx] == state;
	if (!testResult) {
		bGameEnded = true;
		return {};
	}

	auto& res = pTestData->playerMoves[moveIdx++ / 2];
	return res;
}

bool trivialTest() {
	TestData testData{};
	TestGame testGame{ &testData, 3, 3 };
	testGame.run();
	return testGame.testResult;
}

bool mainTest() {
	TestData testData{};
	vector<TestData> arTestData{
		{
			{{0, 0}, {1, 0}, {2, 0}},
			{ {0, 1}, {1, 1}, {2, 1} },
			{ PLAYER_MOVE, AI_MOVE, PLAYER_MOVE, AI_MOVE, PLAYER_MOVE, PLAYER_WON }
		},
		{
			{{0, 0}, {1, 0}, {2, 2}},
			{ {0, 1}, {1, 1}, {2, 1} },
			{ PLAYER_MOVE, AI_MOVE, PLAYER_MOVE, AI_MOVE, PLAYER_MOVE, AI_MOVE, AI_WON}
		},
	};
	bool res = true;
	for (auto& testData : arTestData) {
		TestGame testGame{ &testData, 3, 3 };
		testGame.run();
		if (!testGame.testResult) {
			return false;
		}
	}
	return true;
}

TEST(TestCaseName, TrivialTest) {
	EXPECT_TRUE(trivialTest());
}

TEST(TestCaseName, MainTest) {
	EXPECT_TRUE(mainTest());
}
