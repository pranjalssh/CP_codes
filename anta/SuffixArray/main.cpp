#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <cstring>
#include <utility>
#include <cstdio>
#include <cassert>
#include <random>
#include <limits>
#include <functional>
#include <map>

#include "../Util/MyAssert.hpp"
#include "SA_IS.hpp"
#include "InverseSuffixArray.hpp"
#include "KLAAP.hpp"
#include "ChildTable.hpp"
#include "LCPIntervals.hpp"
#include "PostOrderTraversal.hpp"
#include "LPFArray.hpp"
#include "LCPArrayRMQ.hpp"

void test_childtable(const char *s, int n);
void test_lcpArrayRMQ(const char *s, int n);

int main() {
	while(1){
	std::string str;
	std::getline(std::cin, str);
	const char *s = str.c_str();
	int n = std::strlen(s);
	std::unique_ptr<Index[]> suffixArray(new Index[n+1]);
	std::unique_ptr<Index[]> inverseSuffixArray(new Index[n+1]);
	std::unique_ptr<Index[]> lcpArray(new Index[n+2]);
	std::unique_ptr<Index[]> childTable(new Index[n+2]);
	std::unique_ptr<Index[]> lpfArray(new Index[n+1]);
	buildSuffixArray(s, n, suffixArray.get());
	computeInverseSuffixArray(suffixArray.get(), n, inverseSuffixArray.get());
	computeLCPArray(s, suffixArray.get(), inverseSuffixArray.get(), n, lcpArray.get());
	computeChildTable(lcpArray.get(), n, childTable.get());
	computeLPFArray(suffixArray.get(), lcpArray.get(), n, lpfArray.get());
	std::cout<<"i\tsuf\tlcp\tchild\tLPF\tS[suf..n]"<<std::endl;
	for(int i = 0; i <= n; i ++) {
		std::cout
			<< i << '\t'
			<< suffixArray[i] << '\t'
			<< lcpArray[i] << '\t'
			<< childTable[i] << '\t'
			<< lpfArray[i] << '\t'
			<< (s + suffixArray[i]) << '$'
			<< std::endl;
	}
	test_childtable(s, n);
	test_lcpArrayRMQ(s, n);
	}
	return 0;
}

void test_lcpArrayRMQ(const char *s, int n) {
	Index *suffixArray(new Index[n+1]);
	Index *inverseSuffixArray(new Index[n+2]);
	Index *lcpArray(new Index[n+2]);
	LCPArrayRMQ lcpArrayRMQ;
	buildSuffixArray(s, n, suffixArray);
	computeInverseSuffixArray(suffixArray, n, inverseSuffixArray);
	computeLCPArray(s, suffixArray, inverseSuffixArray, n, lcpArray);
	preprocessLCPArrayRMQ(lcpArray, n, lcpArrayRMQ);
	for(Index i = 0; i <= n; i ++)
	for(Index j = 0; j <= n; j ++) {
		Index p = 0;
		while(i+p < n && s[i+p] == s[j+p]) p ++;
		Index l = computeLCP(inverseSuffixArray, lcpArray, lcpArrayRMQ, n, i, j);
		my_assert(p == l);
	}
	Index *childTable(new Index[n+2]);
	computeChildTable(lcpArray, n, childTable);
	postOrderTraversal(lcpArray, n, [suffixArray,lcpArray,childTable,&lcpArrayRMQ,n](Index left, Index right, Index lcp) -> void {
		std::vector<Interval> v, w;
		enumChildIntervals(lcpArray, childTable, left, right, [&v](Index i, Index j) -> bool {
			v.push_back(Interval(i, j));
			return false;
		});
		enumChildIntervalsRMQ(lcpArray, lcpArrayRMQ, left, right, [&w](Index i, Index j) -> bool {
			w.push_back(Interval(i, j));
			return false;
		});
		my_assert(v == w);
	});
	delete[] suffixArray;
	delete[] lcpArray;
	delete[] childTable;
}

void test_childtable(const char *s, int n) {
	Index *suffixArray(new Index[n+1]);
	Index *inverseSuffixArray(new Index[n+2]);
	Index *lcpArray(new Index[n+2]);
	Index *childTable = nullptr;
	buildSuffixArray(s, n, suffixArray);
	computeInverseSuffixArray(suffixArray, n, inverseSuffixArray);
	computeLCPArray(s, suffixArray, inverseSuffixArray, n, lcpArray);
	std::swap(childTable, inverseSuffixArray);
	computeChildTable(lcpArray, n, childTable);
	std::map<Interval,int> m;
	std::map<int,Interval> a;
	for(int k = 0; k <= n; k ++)
		m[Interval(k,k)] = n - suffixArray[k];
	int leafs = 0;
	postOrderTraversal(lcpArray, n,
		[&m,&a,&leafs,suffixArray,lcpArray,childTable,n](Index left, Index right, Index lcp) -> void {
		int &leafs_s = leafs;
		std::map<Interval,int> &mp = m;
		std::map<int,Interval> &ap = a;
		const Index *suffixArray_s = suffixArray, *lcpArray_s = lcpArray, *childTable_s = childTable;
		Index n_s = n;
		my_assert(!enumChildIntervals(lcpArray, childTable, left, right,
			[&mp,&ap,n_s,left,right,lcp,&leafs_s,suffixArray_s,lcpArray_s,childTable_s](Index i, Index j) -> bool {
			Interval v(i,j);
			my_assert(left <= i && j <= right);
			my_assert(mp.count(v) != 0);
			int l = getLCPIntervalValue(suffixArray_s, lcpArray_s, childTable_s, n_s, i, j);
			if(i != j) {
				my_assert(lcpArray_s[i] < l);
				bool e = false;
				for(int k = i+1; k <= j; k ++) {
					my_assert(lcpArray_s[k] >= l);
					e |= lcpArray_s[k] == l;
				}
				my_assert(e);
				my_assert(lcpArray_s[j+1] < l);
			}else leafs_s ++;
			my_assert(mp[v] == l);
			mp[v] = -1;
			if(j != right) {
				my_assert(ap.count(j) == 0);
				ap[j] = Interval(left, right);
			}
			return false;
		}));
		my_assert(!m.count(Interval(left, right)));
		m[Interval(left, right)] = lcp;
	});
	my_assert(n == 0 || leafs == n + 1);
	my_assert(m[Interval(0,n)] == 0);
	m[Interval(0,n)] = -1;
	for(auto it = m.begin(); it != m.end(); ++ it)
		my_assert(it->second == -1);
	delete[] suffixArray;
	delete[] lcpArray;
	delete[] childTable;
}

#if 0
template<typename T> void buildSmallTests(const int N, const int X);
void tests() {
	void buildRandomTests(std::default_random_engine &re);
	buildSmallTests<bool>(16, 2);
	buildSmallTests<char>(10, 3);
	buildSmallTests<unsigned char>(1, 256);
	buildSmallTests<int>(10, 3);
	buildSmallTests<long long>(10, 3);
	std::default_random_engine re;
	buildRandomTests(re);
}

#pragma warning(push)
#pragma warning(disable: 4800)
template<typename T>
void buildSmallTests(const int N, const int X) {
	std::unique_ptr<T[]> buf(new T[N+1]);
	int tests = 0;
	for(int n = 0, combs = X; n < N; n ++, combs *= X) {
		for(int x = 0; x < combs; x ++) {
			if(tests++ % 4096 == 0) {
				std::printf("\rsmallTests(%d, %d): %d, %d..."
					, N, X, n, x);
			}
			int y = x;
			for(int i = 0; i <= n; i ++) {
				buf[i] = (T)(y % X);
				y /= X;
			}
			buildTest<T>(buf.get(), n);
		}
	}
}

template<typename T>
void buildRandomTest(std::default_random_engine &re, int n, int alphas);
void buildRandomTests(std::default_random_engine &re) {
	const int nList[] = {5,10,100,1000};
	for(int i = 0; i < 1000; i ++) {
		std::printf("\rbuildRandomTests %d...", i);
		for(int j = 0; j < (int)(sizeof(nList) / sizeof(*nList)); j ++) {
			int n = nList[j];
			buildRandomTest<bool>(re, n, 1);
			buildRandomTest<bool>(re, n, 2);
			buildRandomTest<char>(re, n, 1);
			buildRandomTest<char>(re, n, 2);
			buildRandomTest<char>(re, n, 3);
			buildRandomTest<char>(re, n, 5);
			buildRandomTest<char>(re, n, 10);
			buildRandomTest<char>(re, n, 26);
			buildRandomTest<char>(re, n, 50);
			buildRandomTest<char>(re, n, 100);
			buildRandomTest<char>(re, n, 128);
			buildRandomTest<unsigned char>(re, n, 128);
			buildRandomTest<unsigned char>(re, n, 140);
			buildRandomTest<unsigned char>(re, n, 255);
			buildRandomTest<unsigned char>(re, n, 256);
			buildRandomTest<short>(re, n, 1000);
		}
	}
	std::puts("buildRandomTests OK          ");
}

template<typename T>
void buildRandomTest(std::default_random_engine &re, int n, int alphas) {
	std::unique_ptr<T[]> str(new T[n+1]);
	std::uniform_int_distribution<int> alpha(0, alphas-1);
	for(int i = 0; i <= n; i ++)
		str[i] = (T)alpha(re);
	buildTest<T>(str.get(), n);
}
#pragma warning(pop)

template<typename T> void buildTest(const T *str, int n) {
	typedef SuffixArray::Index Index;
	SuffixArray sa;
	sa.build(str, n);
	for(int i = 0; i < n; i ++) {
		my_assert(0 <= sa.suffixArray[i] && sa.suffixArray[i] <= n);
		my_assert(std::lexicographical_compare(
			str + sa.suffixArray[i], str + n,
			str + sa.suffixArray[i+1], str + n));
	}
	std::unique_ptr<int[]> inverse_array(new int[n+1]);
	std::fill(&inverse_array[0], &inverse_array[n+1], -1);
	for(int i = 0; i <= n; i ++) {
		Index x = sa.suffixArray[i];
		my_assert(inverse_array[x] == -1);
		inverse_array[x] = i;
	}
	for(int i = 0; i <= n; i ++) {
		my_assert(sa.suffixArray[inverse_array[i]] == i);
	}

	//inverse suffix array & LCP-array
	sa.buildInverseSuffixArray();
	sa.buildLCPArray(str);
	for(int i = 0; i <= n; i ++) {
		my_assert(sa.inverseSuffixArray[i] == inverse_array[i]);
		int h;
		if(i == 0) h = -1;
		else {
			int j = sa.suffixArray[i];
			int k = sa.suffixArray[i-1];
			for(h = 0; j+h < n && k+h < n && str[j+h] == str[k+h]; ++ h) ;
		}
		my_assert(sa.lcpArray[i] == h);
	}
}
#endif
