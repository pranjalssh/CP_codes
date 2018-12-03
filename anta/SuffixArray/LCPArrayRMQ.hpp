#pragma once
#include "SuffixArray.hpp"
#include "../RangeMinimumQuery/DirectRMQ.hpp"
#include <utility>

typedef DirectRMQ<Index> LCPArrayRMQ;

static void preprocessLCPArrayRMQ(const Index *lcpArray, Index n, LCPArrayRMQ &lcpArrayRMQ) {
	lcpArrayRMQ.build(lcpArray, n+1);
}

static Index computeLCP(const Index *inverseSuffixArray, const Index *lcpArray, LCPArrayRMQ &lcpArrayRMQ, Index n, Index i, Index j) {
	if(i == j) return n - i;
	Index x = inverseSuffixArray[i], y = inverseSuffixArray[j];
	if(x > y) std::swap(x, y);
	return lcpArrayRMQ.queryVal(lcpArray, x, y-1);
}

static Index getNextLIndexRMQ(const Index *lcpArray, LCPArrayRMQ &lcpArrayRMQ, Index l, Index r) {
	return lcpArrayRMQ.query(lcpArray, l+1, r);
}

static Index getLCPIntervalValueRMQ(const Index *lcpArray, LCPArrayRMQ &lcpArrayRMQ, Index l, Index r) {
	return lcpArrayRMQ.queryVal(lcpArray, l+1, r);
}

template<typename Process>
static bool enumChildIntervalsRMQ(const Index *lcpArray, LCPArrayRMQ &lcpArrayRMQ, Index i, Index j, Process process) {
	if(i == j) return false;
	Index lIndex1 = i, lIndex2, l;
	lIndex2 = getNextLIndexRMQ(lcpArray, lcpArrayRMQ, lIndex1, j);
	l = lcpArray[lIndex2];
	do {
		if(process(Index(lIndex1), lIndex2-1)) return true;
		lIndex1 = lIndex2;
	}while(lIndex1 != j && l == lcpArray[lIndex2 = getNextLIndexRMQ(lcpArray, lcpArrayRMQ, lIndex1, j)]);
	return process(lIndex1, j);
}
