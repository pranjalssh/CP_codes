#include <random>
#include <vector>
#include <queue>
#include <utility>
#include <iostream>
#include <cassert>

#include "../Util/StaticGraph_Simple.cpp"
#include "../Util/TimeIt_Simple.cpp"
#include "../Util/RandomRootedTree.cpp"
#include "SchieberVishkinLCA.cpp"
#include "DoublingLCA.cpp"
#include "EulerTourLCA.cpp"

void test(int n, int q) {
	std::cout << "TEST n=" << n << ", q=" << q << std::endl;

	std::vector<std::pair<int,int> > edges;
	int root = generateRandomRootedTree(n, edges);
	Graph g(n, edges);

	std::vector<std::pair<int,int> > queries(q);
	std::uniform_int_distribution<int> rndn(0, n-1);
	
	for(int i = 0; i < q; i ++)
		queries[i] = std::make_pair(rndn(global_re), rndn(global_re));

#if 1	//Test
	EulerTourLCA eulerLCA;

	if(n <= DoublingLCA::MaxN) TIMEIT("Doubling init") {
		DoublingLCA::g = &g;
		DoublingLCA::lca_init(root);
	}

	SchieberVishkinLCA sv;
	TIMEIT("SchieberVishkin build") {
		sv.build(g, root);
	}

	TIMEIT("EulerTour build") {
		eulerLCA.build(g, root);
	}

	std::vector<int> answers1(q), answers2(q), answers3(q);

	if(n <= DoublingLCA::MaxN) TIMEIT("Doubling queries") {
		for(int i = 0; i < q; i ++)
			answers1[i] = DoublingLCA::lca_query(queries[i].first, queries[i].second);
	}

	TIMEIT("SchieberVishkin queries") {
		for(int i = 0; i < q; i ++)
			answers2[i] = sv.query(queries[i].first, queries[i].second);
	}

	TIMEIT("EulerTour queries") {
		for(int i = 0; i < q; i ++)
			answers3[i] = eulerLCA.query(queries[i].first, queries[i].second);
	}

	for(int i = 0; i < q; i ++) {
		int v = queries[i].first, u = queries[i].second;
		if((n <= DoublingLCA::MaxN && answers1[i] != answers2[i]) || answers2[i] != answers3[i]) {
			int b = DoublingLCA::bruteLCA(v, u);
			std::cout << v << ", " << u << ": " << answers1[i] << ", " << answers2[i] << ", " << answers3[i] << " (brute: " << b << ")" << std::endl;
			int a2 = sv.query(v, u);
			int a3 = eulerLCA.query(v, u);
		}
	}
#else	//Performance Test
	SchieberVishkinLCA sv;
	TIMEIT("SchieberVishkin build") {
		sv.build(g, root);
	}

	EulerTourLCA eulerLCA;
	TIMEIT("EulerTour build") {
		eulerLCA.build(g, root);
	}

	unsigned sum = 0;
	TIMEIT("SchieberVishkin queries") {
		for(int i = 0; i < q; i ++)
			sum ^= sv.query(queries[i].first, queries[i].second);
	}

	TIMEIT("EulerTour queries") {
		for(int i = 0; i < q; i ++)
			sum ^= eulerLCA.query(queries[i].first, queries[i].second);
	}

	if(sum != 0) {
		std::cerr << "Error!!!" << std::endl;
	}
#endif

	std::cout << std::endl;
}

int main() {
	test(1, 100);
	test(3, 100);
	test(5, 100);
	test(10, 1000);
	test(16, 1000);
	test(20, 1000);
	test(50, 10000);
	test(100, 10000);
	test(1000, 100000);
	test(10000, 1000000);
	test(100000, 20000000);
	test(1000000, 20000000);
	test(10000000, 20000000);
#if 0
	std::vector<Graph::Edge> edges;
	int n;
	scanf("%d", &n);
	for(int i = 0; i < n-1; i ++) {
		int x, y;
		scanf("%d %d", &x, &y); x --; y --;
		edges.push_back(Graph::Edge(x, y));
	}
	Graph g(n, edges);
	SchieberVishkin lca;
	lca.build(g, 0);
	for(int i = 0; i < n; i ++)
	for(int j = 0; j < n; j ++) {
		std::cout << i+1 << ", " << j+1 << ": " << lca.query(i, j)+1 << std::endl;
	}
#endif
	return 0;
}

/*

2013/09/09 00:47

TEST n=100000, q=20000000
SchieberVishkin build: 0.018715 sec.
EulerTour build: 0.013790 sec.
SchieberVishkin queries: 1.317108 sec.
EulerTour queries: 2.609335 sec.

TEST n=1000000, q=20000000
SchieberVishkin build: 0.834230 sec.
EulerTour build: 0.305482 sec.
SchieberVishkin queries: 5.510048 sec.
EulerTour queries: 8.020057 sec.

TEST n=10000000, q=20000000
SchieberVishkin build: 10.301257 sec.
EulerTour build: 3.744845 sec.
SchieberVishkin queries: 6.605217 sec.
EulerTour queries: 11.091087 sec.

*/
