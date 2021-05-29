#pragma once

#include <random>
#include <string>
#include <unordered_map>

using namespace std;

struct Coords { int x, y; };

void error(const string& errMsg);

std::mt19937& initRand();

template<typename T = uint64_t>
uint64_t getRandom()
{
	static auto& gen = initRand();
	static std::uniform_int_distribution<T> distrib(0);
	return distrib(gen);
}
