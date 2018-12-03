#include "RMSQ.cpp"
#include <iostream>
#include <algorithm>
#include <memory>
#include <utility>
#include <random>

#include "../Util/MyAssert.cpp"

int main() {
	void testsRMSQ();
	testsRMSQ();
	return 0;
}

void testRMSQ(const int *a, int n) {
	typedef RangeMaximumSumSegmentQuery RMSQ;
	RMSQ rmsq;
	rmsq.build(a, n);
	for(int i = 0; i < n; i ++) for(int j = i; j < n; j ++) {
		for(int k = 0; k < n; k ++) for(int l = k; l < n; l ++) {
			if(l < i) continue;
			RMSQ::Segment s = rmsq.query(i, j, k, l);
			long long x = rmsq.sum(s);
			long long y = std::numeric_limits<long long>::min();
			for(int L = i; L <= j; L ++) {
				long long z = 0;
				int Rleft = std::max(L, k);
				for(int R = L; R < Rleft; R ++)
					z += a[R];
				for(int R = Rleft; R <= l; R ++) {
					z += a[R];
					y = std::max(y, z);
				}
			}
			my_assert(x == y);
			if(i == k && j == l)
				my_assert(rmsq.sum(rmsq.query(i, j)) == x);
		}
	}
}

void randomtestRMSQ(int n, int xl, int xu, std::default_random_engine &re) {
	std::unique_ptr<int[]> a(new int[n]);
	std::uniform_int_distribution<int> rng_val(xl, xu);
	for(int i = 0; i < n; i ++)
		a[i] = rng_val(re);
	testRMSQ(a.get(), n);
}

void testsRMSQ() {
	std::default_random_engine re;
	for(int i = 0; i < 1000; i ++) {
		if(i % 100 == 0) std::cout << "\rtestsRMSQ (0) " << i << "..."; std::cout.flush();
		randomtestRMSQ(0, -1, 1, re);
		randomtestRMSQ(1, -1, 1, re);
	}
	for(int i = 0; i < 2000; i ++) {
		if(i % 100 == 0) std::cout << "\rtestsRMSQ (1) " << i << "..."; std::cout.flush();
		randomtestRMSQ(3, -2, 2, re);
	}
	for(int i = 0; i < 1000; i ++) {
		if(i % 10 == 0) std::cout << "\rtestsRMSQ (2) " << i << "..."; std::cout.flush();
		randomtestRMSQ(5, -2, 2, re);
	}
	for(int i = 0; i < 1000; i ++) {
		if(i % 1 == 0) std::cout << "\rtestsRMSQ (3) " << i << "..."; std::cout.flush();
		randomtestRMSQ(10, -2, 2, re);
	}
	for(int i = 0; i < 100; i ++) {
		if(i % 1 == 0) std::cout << "\rtestsRMSQ (4) " << i << "..."; std::cout.flush();
		randomtestRMSQ(20, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), re);
		randomtestRMSQ(20, 1, std::numeric_limits<int>::max(), re);
		randomtestRMSQ(20, std::numeric_limits<int>::min(), -1, re);
	}
	std::cout << "OK                            " << std::endl;
}

