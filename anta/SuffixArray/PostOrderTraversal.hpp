#pragma once
#include "SuffixArray.hpp"
#include <cassert>
#include <utility>
#include <vector>

//ただ単に順番に回るだけで親子関係は気にしない
template<typename Process>
static void postOrderTraversal(const Index *lcpArray, Index n, Process process) {
	struct St {
		Index left, lcp;
		St(Index l, Index p): left(l), lcp(p) { }
	};
	std::vector<St> stk;
	stk.push_back(St(0, 0));
	for(Index i = 1; i <= n; i ++) {
		Index lcp = lcpArray[i], lb = i - 1;
		St st = stk.back();
		while(lcp < st.lcp) {
			lb = st.left;
			process(Index(lb), i - 1, Index(st.lcp));
			stk.pop_back();
			st = stk.back();
		}
		if(lcp > st.lcp)
			stk.push_back(St(lb, lcp));
	}
	if(n != 0) process(0, n, 0);
}
