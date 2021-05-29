#pragma once

#include <vector>
#include "utils.h"

using namespace std;

struct Collection {
	static const size_t idx_max = numeric_limits<size_t>::max();
	vector<Coords>& vec;
	unordered_map<size_t, bool> excludes;
	size_t curIdx = idx_max;
	Collection(vector<Coords>& vec) : vec{ vec } {

	}
	Collection(const Collection& other): vec{other.vec} {
		curIdx = idx_max;
		excludes = other.excludes;
	}
	Coords* next();
	void addExclude(size_t idx);
	void removeExclude(size_t idx);
	void resetIter();
	Coords* next(size_t& idx);
};
