//未検証！

struct UnionFind {
	vector<int> data;
	UnionFind(int size_) : data(size_, -1) { }
	bool unionSet(int x, int y) {
		x = root(x); y = root(y);
		if (x != y) {
			if (data[y] < data[x]) swap(x, y);
			data[x] += data[y]; data[y] = x;
		}
		return x != y;
	}
	bool findSet(int x, int y) { return root(x) == root(y); }
	int root(int x) { return data[x] < 0 ? x : data[x] = root(data[x]); }
	int size(int x) { return -data[root(x)]; }
};

typedef long long Weight;
struct Edge {
	int x, y;
	Weight w;
	Edge() { }
	Edge(int x_, int y_, Weight w_): x(x_), y(y_), w(w_) { }
};
bool operator<(const Edge &a, const Edge &b) { return a.w < b.w; }

//((極大)最小全域森のコスト,連結成分数(木なら1) )
pair<Weight,int> minimumSpanningForest(int N, vector<Edge> edges) {
	sort(edges.begin(), edges.end());
	int t = N; Weight w = 0;
	UnionFind uf(N);
	for(int i = 0; i < (int)edges.size(); i ++) {
		if(uf.unionSet(edges[i].x, edges[i].y)) {
			w += edges[i].w;	//ここで復元が可能
			t --;
		}
	}
	return make_pair(w, t);
}

