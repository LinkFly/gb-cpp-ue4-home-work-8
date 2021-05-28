#pragma once

#include <functional>

using namespace std;

struct Field {
	static char asChar(Cell cell) { return static_cast<char>(cell); }

	int width{ 3 }, height{ 3 };
	Cell* cells = nullptr;
	Field() {
		init();
	}
	Field(int width, int height) : width{ width }, height{ height } {
		init();
	}
	~Field() {
		delete[] cells;
	}
	void init() {
		cells = new Cell[height * width];
		for (int i = 0; i < height * width; ++i)
		{
			cells[i] = Cell::empty;
		}

	}
	Cell getCell(int x, int y) {
		return cells[y * width + x];
	}
	Cell getCell(int i) {
		return cells[i];
	}
	void setCell(int x, int y, Cell cell) {
		cells[y * width + x] = cell;
	}
	bool isEmpty(int x, int y) {
		return getCell(x, y) == Cell::empty;
	}
	void draw() {
		system("cls");
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				Cell curCell = getCell(x, y);
				outCell(curCell);
				cout << " ";
			}
			cout << endl;
		}
	}
	void outCell(Cell cell) {
		cout << Field::asChar(cell) << " ";
	}
	void doCells(function<void(int x, int y, Cell cell)>& callback) {
		for (int i = 0; i < height * width; ++i)
		{
			callback(i / height, i % height, getCell(i));
		}
	}
};