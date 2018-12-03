#pragma once
#include "SuffixArray.hpp"
#include <algorithm>

//KLAAP algorithm <http://www.cs.iastate.edu/~cs548/references/linear_lcp.pdf>
//lcpArray[i]はsuffixArray[i]とsuffixArray[i+1]のLCPの長さを保持する(ただしlcpArray[n] = 0)
//	これはlcp(suffixArray[i-1],suffixArray[i])とするものが一般的なので注意。
//	'$'を最大のものとして扱っている文献があってsuffixArrayがずれるので、そのlcpArrayと同じになるようにしてある。
//lcpArrayはサイズ(n+2)
static void computeLCPArray(const Alpha *str, const Index *suffixArray, const Index *inverseSuffixArray, Index n, Index *lcpArray) {
	Index h = 0;
	for(Index i = 0; i < n; i ++) {
		Index pos = inverseSuffixArray[i];
		Index j = suffixArray[pos-1];
		Index hbound = std::min(n - j, n - i);
		for(Index k = 0; h < hbound && str[i+h] == str[j+h]; ++ h) ;
		lcpArray[pos-1] = h;
		if(h > 0) -- h;
	}
	lcpArray[n] = lcpArray[n+1] = 0;
}
