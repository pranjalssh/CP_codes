#pragma once
#include "SuffixArray.hpp"
#include <cassert>
#include <vector>

static void computeChildTable_upDown(const Index *lcpArray, Index n, Index *childTable) {
	std::vector<Index> stk;
	stk.push_back(0);
	for(Index i = 1; i <= n; i ++) {
		Index lastIndex = -1;
		while(lcpArray[i] < lcpArray[stk.back()]) {
			lastIndex = stk.back(); stk.pop_back();
			Index top = stk.back();
			if(lcpArray[i] <= lcpArray[top] && lcpArray[top] != lcpArray[lastIndex])
				childTable[top] = lastIndex;	//[top].down := lastIndex
		}
		if(lastIndex != -1)
			childTable[i-1] = lastIndex;	//[i].up := lastIndex
		stk.push_back(i);
	}
}

static void computeChildTable_lastLIndex(const Index *lcpArray, Index n, Index *childTable) {
	std::vector<Index> stk;
	stk.push_back(0);
	for(Index i = 1; i <= n; i ++) {
		while(lcpArray[i] < lcpArray[stk.back()])
			stk.pop_back();
		if(lcpArray[i] == lcpArray[stk.back()]) {
			childTable[stk.back()] = i;
			stk.pop_back();
		}
		stk.push_back(i);
	}
}

//childTable‚ÍƒTƒCƒY(n+2)
static void computeChildTable(const Index *lcpArray, Index n, Index *childTable) {
	childTable[n] = childTable[n+1] = -1;
	computeChildTable_upDown(lcpArray, n, childTable);
	computeChildTable_lastLIndex(lcpArray, n, childTable);
}

static inline Index getChildTableEntryUp(const Index *childTable, Index i) {
	return childTable[i-1];
}
static inline Index getChildTableEntryDown(const Index *childTable, Index i) {
	return childTable[i];
}

static Index getChildTableEntryNext(const Index *lcpArray, const Index *childTable, Index i) {
	Index lcp = lcpArray[i], next = childTable[i];
	return next != -1 && lcp <= lcpArray[i+1] && lcp == lcpArray[next] ? next : -1;
}
