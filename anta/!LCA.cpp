//新ver.
//Graphの構築には!DirectTree.cppを使うこと

typedef int Vertex;
struct Graph {
	typedef std::pair<Vertex, Vertex> Edge;
	struct To {
		Vertex to;
	};

	int n, m;

	Graph(int n_, const std::vector<Edge> &edges):
		n(n_), m(edges.size()), tos(m+1), offsets(n+1, 0) {
		for(int e = 0; e < m; e ++) offsets[edges[e].first] ++;
		for(int v = 1; v <= n; v ++) offsets[v] += offsets[v-1];
		for(int e = 0; e < m; e ++)
			tos[-- offsets[edges[e].first]].to = edges[e].second;
	}

	inline const To *edgesBegin(int v) const { return &tos[offsets[v]]; }
	inline const To *edgesEnd(int v) const { return &tos[offsets[v+1]]; }

	inline const int outDegree(int v) const { return offsets[v+1] - offsets[v]; }

private:
	std::vector<To> tos;
	std::vector<int> offsets;
};
	
class SchieberVishkinLCA {
public:
	typedef unsigned Word;
private:

	static inline Word lowestOneBit(Word v) {
		return v & (~v+1);
	}
	static inline Word highestOneBitMask(Word v) {
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		return v >> 1;
	}

	std::vector<Word> indices;			//Vertex -> index
	std::vector<Word> maxHIndices;		//Vertex -> index
	std::vector<Word> ancestorHeights;	//Vertex -> Word
	std::vector<Vertex> pathParents;	//index-1 -> Vertex
public:
	//gは親→子の枝のある根付き木
	void build(const Graph &g, Vertex root) {
		assert(g.m == g.n - 1);

		ancestorHeights.resize(g.n);
		std::vector<Vertex> parents(g.n);
		maxHIndices.resize(g.n);
		std::vector<Vertex> vertices; vertices.reserve(g.n);
		indices.resize(g.n);

		//euler tour
		Word currentIndex = 1;
		parents[root] = root;	//利便さのために
		vertices.push_back(root);
		while(!vertices.empty()) {
			Vertex v = vertices.back(); vertices.pop_back();
			indices[v] = currentIndex ++;
			for(const Graph::To *it = g.edgesBegin(v); it != g.edgesEnd(v); ++ it) {
				parents[it->to] = v;
				vertices.push_back(it->to);
			}
		}

		//BFS (トポロジカル順序を求めるために)
		int head = 0, tail = 1;
		vertices.resize(g.n); vertices[0] = root;
		while(head != tail) {
			Vertex v = vertices[head ++];
			for(const Graph::To *it = g.edgesBegin(v); it != g.edgesEnd(v); ++ it)
				vertices[tail ++] = it->to;
		}

		//深い方から
		for(std::vector<Vertex>::const_iterator it = vertices.begin(); it != vertices.end(); ++ it)
			maxHIndices[*it] = indices[*it];
		for(std::vector<Vertex>::const_reverse_iterator it = vertices.rbegin(); it != vertices.rend(); ++ it) {
			Vertex v = *it, parent = parents[v];
			if(lowestOneBit(maxHIndices[parent]) < lowestOneBit(maxHIndices[v]))
				maxHIndices[parent] = maxHIndices[v];
		}

		//Aを求める
		ancestorHeights[root] = 0;
		for(std::vector<Vertex>::const_iterator it = vertices.begin(); it != vertices.end(); ++ it) {
			Vertex v = *it;
			ancestorHeights[v] = ancestorHeights[parents[v]] | lowestOneBit(maxHIndices[v]);
		}

		pathParents.swap(parents);	//メモリをけちる
		pathParents[indices[root]-1] = root;
		for(std::vector<Vertex>::const_iterator it = vertices.begin(); it != vertices.end(); ++ it) {
			Vertex v = *it;
			for(const Graph::To *jt = g.edgesBegin(v); jt != g.edgesEnd(v); ++ jt) {
				if(maxHIndices[v] != maxHIndices[jt->to])
					pathParents[indices[jt->to]-1] = v;
				else
					pathParents[indices[jt->to]-1] = pathParents[indices[v]-1];
			}
		}
	}

	Vertex query(Vertex v, Vertex u) const {
		//binary tree上でのLCAの高さを求める
		Word Iv = maxHIndices[v], Iu = maxHIndices[u];
		Word hIv = lowestOneBit(Iv), hIu = lowestOneBit(Iu);
		Word hbMask = highestOneBitMask((Iv ^ Iu) | hIv | hIu);
		Word j = lowestOneBit(ancestorHeights[v] & ancestorHeights[u] & ~hbMask);
		//j = hI(lca(v,u)) となる (ここで、hI(x) = 2^(complete binary tree上でのI(x)の高さ), I(x) = maxHIndices[x])
		//(hI(lca(v,u)) = j)はhI(v)かhI(u)かその他の最大値。そして一意であることを考えると…
		Vertex x, y;
		if(j == hIv) x = v;
		else {			//lcaはvのパス上には無い
			Word kMask = highestOneBitMask(ancestorHeights[v] & (j-1));	//vの祖先で、jよりは低いけどその中で一番上にあるパス
			x = pathParents[(indices[v] & ~kMask | (kMask+1))-1];	//indices[v]のkの高さの祖先のパスの親
		}
		if(j == hIu) y = u;
		else {			//lcaはuのパス上には無い
			Word kMask = highestOneBitMask(ancestorHeights[u] & (j-1));	//uの祖先で、jよりは低いけどその中で一番上にあるパス
			y = pathParents[(indices[u] & ~kMask | (kMask+1))-1];	//indices[u]のkの高さの祖先のパスの親
		}
		return indices[x] < indices[y] ? x : y;	//in-orderなので、インデックスが低い方が祖先なはず
	}
};


/*
	旧ver.
	いろいろと場合に応じて書き換えること
*/

int lca_depth[111111];
int lca_doubling[111111][17];	//lca_doubling[i][k]はiの2^k-親
vector<int> lca_tmp;

void lca_dfs(int parent, int i) {
	static int depth = 0;
	lca_depth[i] = depth;
	lca_tmp.push_back(i);
	for(int j = 1, k = 0; j < lca_tmp.size(); j *= 2, k ++) {
		lca_doubling[i][k] = lca_tmp[lca_tmp.size() - 1 - j];
	}
	for(vector<int>::iterator j = edges[i].begin(); j != edges[i].end(); ++ j) if(*j != parent) {
		depth ++;
		lca_dfs(i, *j);
		depth --;
	}
	lca_tmp.pop_back();
}

void lca_init(int root) {
	mset(lca_doubling, -1);
	lca_dfs(-1, 0);
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
