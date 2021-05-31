// home-work-8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Game.h"

using namespace std;

int main()
{
    cout << "Enter field size: ";
    int size{};
    cin >> size;
    cout << "Enter number of figure for win: ";
    int countForWin{};
    cin >> countForWin;
    Game{size, size, countForWin}.run();
    return 0;
}
