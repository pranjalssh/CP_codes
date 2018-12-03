//Verified: <http://qupc2014.contest.atcoder.jp/submissions/150322>
//!INCLUDE: !BinaryHeap.cpp
//!INCLUDE: !StaticGraph.cpp (WeightedStaticGraph)

void dijkstra(const Graph &g, int s, vector<Graph::Dist> &dist) {
	int n = g.numVertices();
	dist.assign(n, Graph::InfDist);
	vector<bool> vis(n);
	BinaryHeap<int> h(n);
	h.add(s, 0);
	while(!h.empty()) {
		int v = h.argmin();
		dist[v] = h.get(v);
		h.remove(v);
		vis[v] = true;
		for(Graph::const_iterator it = g.edgesBegin(v), et = g.edgesEnd(v); it != et; ++ it) {
			if(!vis[it->to])
				h.updatemin(it->to, dist[v] + it->w);
		}
	}
}
