struct WeightedStaticGraph {
	typedef int Vertex;
	typedef int Weight; typedef long long Dist;
	static const Dist InfDist;
	struct To {
		Vertex to; Weight w;
		To() { }
		To(Vertex to_, Weight w_): to(to_), w(w_) { }
	};
	typedef std::pair<Vertex,To> Edge;
	typedef const To *const_iterator;
 
	void init(int n_, const std::vector<Edge> &edges) {
		n = n_; int m = edges.size();
		tos.resize(m+1); offsets.resize(n+1);
		for(int e = 0; e < m; e ++) offsets[edges[e].first] ++;
		for(int v = 1; v <= n_; v ++) offsets[v] += offsets[v-1];
		for(int e = 0; e < m; e ++)
			tos[-- offsets[edges[e].first]] = edges[e].second;
	}
	int numVertices() const { return n; }
 
	inline const_iterator edgesBegin(int v) const { return &tos[offsets[v]]; }
	inline const_iterator edgesEnd(int v) const { return &tos[offsets[v+1]]; }
private:
	int n;
	std::vector<To> tos;
	std::vector<int> offsets;
};
const WeightedStaticGraph::Dist WeightedStaticGraph::InfDist = 0x7fffffffffffffffLL;
typedef WeightedStaticGraph Graph;

bool topologicalSort(int n, Graph::Vertex f[], Graph::Vertex ord[]) {
	vector<int> deg(n);
	for(int i = 0; i < n; i ++) if(f[i] != -1) deg[f[i]] ++;
	int q = 0;
	for(int i = 0; i < n; i ++) if(deg[i] == 0) ord[q ++] = i;
	for(int p = 0; p < q; p ++) {
		Graph::Vertex to = f[ord[p]];
		if(to != -1)
			if((-- deg[to]) == 0) ord[q ++] = to;
	}
	for(int i = 0; i < n; i ++) if(deg[i] > 0) return false;
	return true;
}
 
bool queueBasedBellmanFord(const Graph &g, int s, Graph::Dist dist[], Graph::Vertex prev[]) {
	int n = g.numVertices();
	queue<Graph::Vertex> q; vector<bool> onqueue(n);
	fill(dist, dist+n, Graph::InfDist);
	fill(prev, prev+n, -1);
	vector<Graph::Vertex> tmp_ord(n);
	int relaxes = 0;
	dist[s] = 0; q.push(s); onqueue[s] = true;
	while(!q.empty()) {
		Graph::Vertex v = q.front(); q.pop(); onqueue[v] = false;
		relaxes ++;
		for(Graph::const_iterator it = g.edgesBegin(v), et = g.edgesEnd(v); it != et; ++ it) {
			relaxes ++;
			Graph::Dist d = dist[v] + it->w; Graph::Vertex u = it->to;
			if(d < dist[u]) {
				if(!onqueue[u]) {
					onqueue[u] = true;
					q.push(u);
				}
				dist[u] = d;
				prev[u] = v;
				if(relaxes >= n) {	//n‰ñ‚²‚Æ‚É‚â‚é‚±‚Æ‚É‚æ‚Á‚Ä‚È‚ç‚·
					relaxes = 0;
					if(!topologicalSort(n, prev, &tmp_ord[0]))
						return false;
				}
			}
		}
	}
	return true;
}
