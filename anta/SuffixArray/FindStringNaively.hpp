#pragma once
#include "SuffixArray.hpp"
#include <cassert>
#include <algorithm>
#include <vector>

static Index lowerboundNaively(const Alpha *str, const Index *suffixArray, Index n, const Alpha *pattern, Index m) {
	Index l = 0, count = n + 1;
	while(count > 0) {
		Index i = l + count / 2;
		Index pos = suffixArray[i];
		if(std::lexicographical_compare(str + pos, str + pos + std::min(m, n - pos), pattern, pattern + m))
			l = i + 1, count --;
		count /= 2;
	}
	return l;
}

static Index upperboundNaively(const Alpha *str, const Index *suffixArray, Index n, const Alpha *pattern, Index m) {
	Index l = 0, count = n + 1;
	while(count > 0) {
		Index i = l + count / 2;
		Index pos = suffixArray[i];
		if(!std::lexicographical_compare(pattern, pattern + m, str + pos, str + pos + std::min(m, n - pos)))
			l = i + 1, count --;
		count /= 2;
	}
	return l;
}

static Interval equalrangeNaively(const Alpha *str, const Index *suffixArray, Index n, const Alpha *pattern, Index m) {
	return Interval(
		lowerboundNaively(str, suffixArray, n, pattern, m),
		upperboundNaively(str, suffixArray, n, pattern, m));
}

static void findAllNaively(const Alpha *str, const Index *suffixArray, Index n, const Alpha *pattern, Index m, std::vector<Index> &out_v) {
	Interval interval = equalrangeNaively(str, suffixArray, n, pattern, m);
	for(Index i = interval.first; i < interval.second; i ++)
		out_v.push_back(suffixArray[i]);
}

static bool findNaively(const Alpha *str, const Index *suffixArray, Index n, const Alpha *pattern, Index m) {
	Index i = lowerboundNaively(str, suffixArray, n, pattern, m);
	if(i > n+1) return false;
	Index pos = suffixArray[i];
	if(pos + m > n) return false;
	return std::equal(str + pos, str + pos + m, pattern, std::equal_to<Alpha>());
}

