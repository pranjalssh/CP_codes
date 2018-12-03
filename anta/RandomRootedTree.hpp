#pragma once
#include <random>
#include <vector>
#include <utility>

namespace RandomRootedTree_global {
static std::default_random_engine global_re;
};

//–ß‚è’l: root
static int generateRandomRootedTree(int n, std::vector<std::pair<int,int> > &out_edges) {
	using namespace RandomRootedTree_global;
	out_edges.clear(); out_edges.reserve(n - 1);

	std::vector<int> nodePermutation(n);
	for(int i = 0; i < n; ++ i) nodePermutation[i] = i;
	std::random_shuffle(nodePermutation.begin(), nodePermutation.end());

	for(int i = 1; i < n; ++ i) {
		int j = std::uniform_int_distribution<int>(0, i-1)(global_re);
		out_edges.push_back(std::make_pair(nodePermutation[j], nodePermutation[i]));
	}
	return nodePermutation[0];
}
