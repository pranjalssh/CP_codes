//O(nm)
int bipartiteMatching(const vector<vector<int> > &g) {
	int nleft = g.size(), nright = 0;
	each(es, g) if(!es->empty()) nright = max(nright, *max_element(es->begin(), es->end()) + 1);
	vi matchL(nleft, -1), matchR(nright, -1), prev(nleft), seen(nleft, -1);
	rep(i, nleft) {
		vi stk; stk.push_back(i);
		seen[i] = i; prev[i] = -1;
		while(!stk.empty()) {
			int v = stk.back(); stk.pop_back();
			each(ui, g[v]) {
				int u = *ui;
				int j = matchR[u];
				if(j == -1) {
					while(v != -1) {
						matchR[u] = v;
						swap(u, matchL[v]);
						v = prev[v];
					}
					goto break_;
				}else if(seen[j] < i) {
					seen[j] = i; prev[j] = v;
					stk.push_back(j);
				}
			}
		}
		break_:;
	}
	return (int)matchL.size() - count(matchL.begin(), matchL.end(), -1);
}


//Light type
//(無向グラフ)
vector<bool> done;
vector<int> match;
vector<vector<int> >e;
bool augment(int i) {
	if(done[i]) return false;
	else done[i] = true;
	each(j, e[i]) if(match[*j] == -1 || augment(match[*j])) {
		match[*j] = i;
		return true;
	}
	return false;
}
//インライン
	e = vector<vector<int> >(nleft);
	//ここでeを設定する
	//e[i].pb(j) のように(片方だけで良い)
	match = vector<int>(nright, -1);
	int count = 0;
	rep(i, nleft) {
		done = vector<bool>(nleft, false);
		if(augment(i)) ++ count;
	}

//すぱそ(有向グラフ)
typedef int Weight;
struct Edge {
  int src, dst;
	Edge(int src_, int dst_) : src(src_), dst(dst_) { }
};
typedef vector<Edge> Edges;
typedef vector<Edges> Graph;

bool augment(const Graph& g, int u,
	vector<int>& matchTo, vector<bool>& visited) {
	if(u < 0) return true;
	each(e, g[u]) if (!visited[e->dst]) {
		visited[e->dst] = true;
		if (augment(g, matchTo[e->dst], matchTo, visited)) {
			matchTo[e->src] = e->dst;
			matchTo[e->dst] = e->src;
			return true;
		}
	}
	return false;
}
int bipartiteMatching(const Graph& g, int L) {
	const int n = g.size();
	vector<int> matchTo(n, -1);
	int match = 0;
	rep(u, L) {
		vector<bool> visited(n);
		if (augment(g, u, matchTo, visited)) ++match;
	}
	return match;
}