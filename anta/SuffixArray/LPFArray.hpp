#pragma once
#include "SuffixArray.hpp"
#include <vector>
#include <utility>

//M. Crochemore, L. Ilie, W. F. Smyth: "A simple algorithm for computing the Lempel-Ziv factorization"
//<http://users.csc.calpoly.edu/~dekhtyar/570-Fall2011/papers/KMP.pdf>
//lpfArrayÇÕÉTÉCÉY(n+1)
static void computeLPFArray(const Index *suffixArray, const Index *lcpArray, Index n, Index *lpfArray) {
	typedef std::pair<Index, Index> St;
	std::vector<St> S; Index topSA, topLCP;
	S.push_back(St(topSA = suffixArray[1], topLCP = 0));
	for(Index i = 1; i <= n; i ++) {
		Index iSA = i == n ? -1 : suffixArray[i+1], iLCP = lcpArray[i];
		while(iSA < topSA ||
			 (iSA > topSA && iLCP <= topLCP)) {
			if(iSA < topSA) {
				lpfArray[topSA] = std::max(topLCP, iLCP);
				iLCP = std::min(topLCP, iLCP);
			}else {
				lpfArray[topSA] = topLCP;
			}
			S.pop_back();
			topSA = S.empty() ? -2 : S.back().first;
			topLCP = S.empty() ? -2 : S.back().second;
		}
		S.push_back(St(topSA = iSA, topLCP = iLCP));
	}
	lpfArray[n] = 0;
}
