#include "Collection.h"
#include "utils.h"

Coords* Collection::next()
{
	if (vec.begin() + (curIdx + 1) >= vec.end())
		return nullptr;
	while (excludes.find(curIdx += 1) != excludes.end());
	return (curIdx < vec.size()) ? &vec[curIdx] : nullptr;
}

Coords* Collection::next(size_t& idx)
{
	Coords* res = next();
	idx = curIdx;
	return res;
}

void Collection::addExclude(size_t idx)
{
	excludes[idx] = true;
}

void Collection::removeExclude(size_t idx)
{
	excludes.erase(idx);
}

void Collection::resetIter()
{
	curIdx = idx_max;
}
