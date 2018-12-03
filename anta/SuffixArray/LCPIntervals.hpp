#pragma once
#include "SuffixArray.hpp"
#include "ChildTable.hpp"
#include <cassert>

template<typename Process>
static bool enumChildIntervals(const Index *lcpArray, const Index *childTable, Index i, Index j, Process process) {
	if(i == j) return false;
	Index lIndex1, lIndex2;
	Index up = getChildTableEntryUp(childTable, j+1);
	if(i < up && up <= j)
		lIndex1 = up;
	else
		lIndex1 = getChildTableEntryDown(childTable, i);
	if(process(Index(i), lIndex1-1)) return true;
	while((lIndex2 = getChildTableEntryNext(lcpArray, childTable, lIndex1)) != -1) {
		if(process(Index(lIndex1), lIndex2-1)) return true;
		lIndex1 = lIndex2;
	}
	return process(lIndex1, j);
}

static Index getLCPIntervalValue(const Index *suffixArray, const Index *lcpArray, const Index *childTable, Index n, Index i, Index j) {
	if(i == j) return n - suffixArray[i];
	Index up = getChildTableEntryUp(childTable, j+1);
	return i < up && up <= j ? lcpArray[up] : lcpArray[getChildTableEntryDown(childTable, i)];
}
