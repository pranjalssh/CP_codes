//グラフイテレータインターフェイスの1つ
struct StaticGraph {
	typedef int Vertex;
	typedef std::pair<Vertex, Vertex> Edge;
	struct To { Vertex to; };
	typedef const To *const_iterator;

	void init(int n_, const std::vector<Edge> &edges) {
		n = n_; int m = edges.size();
		tos.resize(m+1); offsets.resize(n+1);
		for(int e = 0; e < m; e ++) offsets[edges[e].first] ++;
		for(int v = 1; v <= n_; v ++) offsets[v] += offsets[v-1];
		for(int e = 0; e < m; e ++)
			tos[-- offsets[edges[e].first]].to = edges[e].second;
	}
	int numVertices() const { return n; }

	inline const_iterator edgesBegin(int v) const { return &tos[offsets[v]]; }
	inline const_iterator edgesEnd(int v) const { return &tos[offsets[v+1]]; }
private:
	int n;
	std::vector<To> tos;
	std::vector<int> offsets;
};
typedef StaticGraph Graph;

//重み付きver.
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

//転置グラフ(逆グラフ)を作る
void transposeGraph(const Graph &g, Graph &h) {
	int n = g.numVertices();
	std::vector<Graph::Edge> edges;
	for(int v = 0; v < n; v ++)
		for(Graph::const_iterator it = g.edgesBegin(v), et = g.edgesEnd(v); it != et; ++ it) {
			Graph::To t = *it; t.to = v;
			edges.push_back(Graph::Edge(it->to, t));
		}
	h.init(n, edges);
}

