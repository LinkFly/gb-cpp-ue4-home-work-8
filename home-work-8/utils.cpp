#include "utils.h"

#include <iostream>
#include <random>

using namespace std;

void error(const string& errMsg)
{
	cerr << errMsg << endl;
	exit(-1);
}

std::mt19937& initRand()
{
	static std::random_device rd;  //Will be used to obtain a seed for the random number engine
	static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	return gen;
}
