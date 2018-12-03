#pragma once
#include <vector>
#include "../Util/StaticGraph_Simple.cpp"

namespace DoublingLCA {
//const int MaxN = 1000000, MaxLogN = 20;
const int MaxN = 0, MaxLogN = 0;
using namespace std;
int lca_depth[MaxN+1];
int lca_doubling[MaxN+1][MaxLogN+1];	//lca_doubling[i][k]ÇÕiÇÃ2^k-êe
vector<int> lca_tmp;
const Graph *g;

void lca_init(int root) {
	memset(lca_doubling, -1, g->n * sizeof(lca_doubling[0]));
	
	lca_tmp.reserve(g->n);
	lca_tmp.push_back(root);
	lca_depth[root] = 0;
	while(!lca_tmp.empty()) {
		int v = lca_tmp.back(); lca_tmp.pop_back();
		for(const Graph::To *it = g->edgesBegin(v); it != g->edgesEnd(v); ++ it) {
			lca_depth[it->to] = lca_depth[v] + 1;
			lca_doubling[it->to][0] = v;
			lca_tmp.push_back(it->to);
		}
	}
	for(int lv = 1; 1 << lv < g->n; ++ lv)
		for(int i = 0; i < g->n; ++ i)
			if(lca_doubling[i][lv-1] != -1)
				lca_doubling[i][lv] = lca_doubling[lca_doubling[i][lv-1]][lv-1];
}

int lca_query(int v, int u) {
	if(lca_depth[v] < lca_depth[u])
		swap(v, u);

	int level = 0;
	for(; 1 << level <= lca_depth[v]; level ++) ;

	for(int i = level-1; i >= 0; i --)
		if(lca_depth[v] - (1 << i) >= lca_depth[u])
			v = lca_doubling[v][i];

	if(v == u) return v;

	for(int i = level-1; i >= 0; i --)
		if(lca_doubling[v][i] != -1 && lca_doubling[v][i] != lca_doubling[u][i])
			v = lca_doubling[v][i], u = lca_doubling[u][i];

	return lca_doubling[v][0];
}


int bruteLCA(int v, int u) {
	static std::vector<char> tmp;
	tmp.resize(g->n);
	for(int x = v; x != -1; x = lca_doubling[x][0]) tmp[x] = 1;
	for(int y = u; y != -1; y = lca_doubling[y][0])
		if(tmp[y]) {
			for(int x = v; x != -1; x = lca_doubling[x][0]) tmp[x] = 0;
			return y;
		}
	return -1;
}

};
