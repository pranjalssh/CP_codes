void unweightedShortestPath(const Graph &g, int s, int dist[]) {
	int n = g.numVertices();
	rep(i, n) dist[i] = INF;
	dist[s] = 0;
	int t = 0;
	vector<int> q, nx;
	nx.push_back(s);
	while(!nx.empty()) {
		q.swap(nx);
		while(!q.empty()) {
			Graph::Vertex v = q.back(); q.pop_back();
			for(Graph::const_iterator it = g.edgesBegin(v), et = g.edgesEnd(v); it != et; ++ it) {
				Graph::Vertex u = it->to;
				if(dist[u] == INF) {
					dist[u] = t + 1;
					nx.push_back(u);
				}
			}
		}
		t ++;
	}
}
