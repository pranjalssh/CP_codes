#pragma once
#include "SuffixArray.hpp"
#include <cassert>
#include <vector>

template<typename ExData, typename AddF, typename AggregateF>
static void bottomUpTraversal(const Index *suffixArray, const Index *lcpArray, Index n, const ExData &initExData, AddF add, AggregateF aggregate) {
	struct St {
		Index left, lcp;
		ExData exData;
		Index childRight;
		St(Index l, Index p, const ExData &e):
			left(l), lcp(p), exData(e), childRight(l) { }
	};
	auto sub_add = [n,suffixArray,&add,&initExData](St &st, Index i, Index j, Index l, const ExData &e) -> void {
		add(st.exData, Index(i), Index(j), Index(l), e);
		for(Index k = st.childRight; k < i; k ++)
			add(st.exData, Index(k), Index(k), n - suffixArray[k], initExData);
		st.childRight = j + 1;
	};
	auto sub_aggregate = [n,suffixArray,&add,&aggregate,&initExData](Index childRight, Index i, Index j, Index l, ExData &e) -> void {
		for(Index k = childRight; k <= j; k ++)
			add(e, Index(k), Index(k), n - suffixArray[k], initExData);
		aggregate(Index(i), Index(j), Index(l), e);
	};

	std::vector<St> stk;
	stk.push_back(St(0, 0, initExData));
	for(Index i = 1; i <= n; i ++) {
		Index lcp = lcpArray[i], lb = i - 1, lastLCP = -1;
		ExData lastExData;
		while(lcp < stk.back().lcp) {
			lb = stk.back().left;
			lastLCP = stk.back().lcp;
			lastExData = stk.back().exData;
			sub_aggregate(stk.back().childRight, lb, i - 1, lastLCP, lastExData);
			stk.pop_back();
			if(lcp <= stk.back().lcp)
				sub_add(stk.back(), lb, i - 1, lastLCP, lastExData);
		}
		if(lcp > stk.back().lcp) {
			stk.push_back(St(lb, lcp, initExData));
			if(lastLCP != -1)
				sub_add(stk.back(), lb, i - 1, lastLCP, lastExData);
		}
	}
	sub_aggregate(stk.back().childRight, 0, n, 0, stk.back().exData);
}

