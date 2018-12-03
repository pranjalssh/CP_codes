//Edgeのsrcは必要ない時は適当な値を入れておく
//operator<は逆なので注意

typedef int Weight;
struct Edge {
  int src, dst;
  Weight weight;
	Edge(int src_, int dst_, Weight weight_) : src(src_), dst(dst_), weight(weight_) { }
};
bool operator<(const Edge &e, const Edge &f) {
  return e.weight != f.weight ? e.weight > f.weight : e.dst < f.dst;
}
typedef vector<Edge> Edges;
typedef vector<Edges> Graph;

//EdgesやGraph構築のヘルパー

//Edgesのsrcに基づきGraphを構築する
void edgesToGraph(const Edges &e, int n, Graph &g) {
	g.assign(n, Edges());
	each(i, e) g[i->src].push_back(*i);
}
//Edgesのsrc, dstに基づきGraphを構築する
//無向グラフ用に、src->dst, dst->src の2辺を張る
void edgesToGraphUndir(const Edges &e, int n, Graph &g) {
	g.assign(n, Edges());
	each(i, e) {
		g[i->src].push_back(*i);
		g[i->dst].push_back(Edge(i->dst, i->src, i->weight));
	}
}

