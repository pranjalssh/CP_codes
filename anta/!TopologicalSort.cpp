//Graphのイテレータインターフェイスを使った新ver.
//cycleがどこかに存在する場合にfalseを返す
bool topologicalSort(const Graph &g, vector<int> &order) {
	int n = g.numVertices();
	vector<char> color(n);
	vector<Graph::Vertex> stk1;
	vector<Graph::const_iterator> stk2;
	order.clear();
	rep(i, n) if(!color[i]) {
		stk1.push_back(i); stk2.push_back(g.edgesBegin(i));
		while(!stk1.empty()) {
			Graph::Vertex v = stk1.back(); Graph::const_iterator &it = stk2.back();
			if(color[v] == 0) color[v] = 1;
			if(it == g.edgesEnd(v)) {
				color[v] = 2;
				order.push_back(v);
				stk1.pop_back(); stk2.pop_back();
				continue;
			}
			Graph::Vertex u = it->to; ++ it;
			if(color[u] != 2) {
				if(color[u] == 1) return false;
				stk1.push_back(u);
				stk2.push_back(g.edgesBegin(u));
			}
		}
	}
	return true;
}

//consider[v]がtrueの頂点だけ見る。orderは逆順を返す
//<http://hbpc2012.contest.atcoder.jp/submissions/134822>
bool topologicalSort(const Graph &g, vector<bool> &consider, vector<int> &order) {
	int n = g.numVertices();
	vector<char> color(n);
	vector<Graph::Vertex> stk1;
	vector<Graph::const_iterator> stk2;
	order.clear();
	rep(i, n) if(consider[i] && !color[i]) {
		stk1.push_back(i); stk2.push_back(g.edgesBegin(i));
		while(!stk1.empty()) {
			Graph::Vertex v = stk1.back(); Graph::const_iterator &it = stk2.back();
			if(color[v] == 0) color[v] = 1;
			if(it == g.edgesEnd(v)) {
				color[v] = 2;
				order.push_back(v);
				stk1.pop_back(); stk2.pop_back();
				continue;
			}
			Graph::Vertex u = it->to; ++ it;
			if(consider[u] && color[u] != 2) {
				if(color[u] == 1) return false;
				stk1.push_back(u);
				stk2.push_back(g.edgesBegin(u));
			}
		}
	}
	return true;
}

void addReachability(const Graph &g, int s, vector<bool> &visited) {
	int n = g.numVertices();
	if(visited[s]) return;
	visited[s] = true;
	vector<int> stk;
	stk.push_back(s);
	while(!stk.empty()) {
		int v = stk.back(); stk.pop_back();
		for(Graph::const_iterator it = g.edgesBegin(v), et = g.edgesEnd(v); it != et; ++ it) {
			int u = it->to;
			if(!visited[u]) {
				visited[u] = true;
				stk.push_back(u);
			}
		}
	}
}

//sから到達できる頂点のみ見る。到達できない場所にサイクルがあっても気にしない
bool canDP(const Graph &g, int s, vector<int> &order) {
	vector<bool> reachable(g.numVertices());
	addReachability(g, s, reachable);
	return topologicalSort(g, reachable, order);
}


//未検証！
bool topologicalSort(const Graph &g, vector<int> &ts_order) {
	int n = g.n;
	vector<char> ts_color(n);
	vector<int> ts_stk1(n+1), ts_stk2(n+1);
	ts_order.assign(n, -1);
	int sp = 0, orderi = 0;
	rep(i, n) if(!ts_color[i]) {
		ts_stk1[sp] = i; ts_stk2[sp ++] = 0;
		while(sp > 0) {
			int v = ts_stk1[sp-1]; int e = ts_stk2[sp-1];
			ts_stk2[sp-1] = e + 1;
			if(e == 0) ts_color[v] = 1;
			const Graph::To *it = g.edgesBegin(v) + e;
			if(it == g.edgesEnd(v)) {
				ts_color[v] = 2;
				ts_order[orderi ++] = v;
				-- sp;
				continue;
			}
			int u = it->to;
			if(ts_color[u] != 2) {
				if(ts_color[u] == 1) return false;
				ts_stk1[sp] = u;
				ts_stk2[sp ++] = 0;
			}
		}
	}
	reverse(ts_order.begin(), ts_order.end());
	return true;
}


//非再帰・グラフを関数で生成する版
//Verified。TopologicalSort_MaxNを書き換えること
const int TopologicalSort_MaxN = 2000*2000;
char ts_color[TopologicalSort_MaxN];
int ts_stk1[TopologicalSort_MaxN+1], ts_stk2[TopologicalSort_MaxN+1];
int ts_order[TopologicalSort_MaxN];

int getedge(int v, int e);

bool topologicalSort(int n) {
	memset(ts_color, 0, n);
	int sp = 0, orderi = 0;
	rep(i, n) if(!ts_color[i]) {
		ts_stk1[sp] = i; ts_stk2[sp ++] = 0;
		while(sp > 0) {
			int v = ts_stk1[sp-1]; int e = ts_stk2[sp-1];
			ts_stk2[sp-1] = e + 1;
			if(e == 0) ts_color[v] = 1;
			int u = getedge(v, e);
			if(u == -2) {
				ts_color[v] = 2;
				ts_order[orderi ++] = v;
				-- sp;
				continue;
			}
			if(u != -1 && ts_color[u] != 2) {
				if(ts_color[u] == 1) return false;
				ts_stk1[sp] = u;
				ts_stk2[sp ++] = 0;
			}
		}
	}
	reverse(ts_order, ts_order + n);
	return true;
}

//////////////////////////////////////////////////////////////////////////////
bool visit(const vector<vector<int> > &g, int v, vector<int> &order, vector<char> &color) {
	color[v] = 1;
	each(e, g[v]) {
		if(color[*e] == 2) continue;
		if(color[*e] == 1) return false;
		if(!visit(g, *e, order, color)) return false;
	}
	order.push_back(v);
	color[v] = 2;
	return true;
}
bool topologicalSort(const vector<vector<int> > &g, vector<int> &order) {
	int n = g.size();
	order.clear(); order.reserve(n);
	vector<char> color(n);
	rep(u, n) if (!color[u] && !visit(g, u, order, color))
		return false;
	reverse(all(order));
	return true;
}


//判定のみ
bool visit(const vector<vector<int> > &g, int v, vector<char> &color) {
	color[v] = 1;
	each(e, g[v]) {
		if(color[*e] == 2) continue;
		if(color[*e] == 1) return false;
		if(!visit(g, *e, color)) return false;
	}
	color[v] = 2;
	return true;
}
bool topologicalSort(const vector<vector<int> > &g) {
	int n = g.size();
	vector<char> color(n);
	rep(u, n) if (!color[u] && !visit(g, u, color))
		return false;
	return true;
}


//////////////////////////////////////////////////////////////////////////////
//未検証！
bool outdegree1graph_ts(const int f[], int n, int depth[], int tmp[], int ord[]) {
	rep(i, n) depth[i] = -1;
	rep(i, n) if(depth[i] == -1) {
		int x = i, t = 0;
		while(1) {
			depth[x] = -2;
			tmp[t ++] = x;
			int y = f[x];
			if(y == -1 || depth[y] >= 0) break;
			if(depth[y] == -2) return false;
			x = y;
		}
		while(t >= 0) {
			x = tmp[-- t]; int y = f[x];
			depth[x] = y == -1 ? 0 : depth[y] + 1;
		}
	}
	rep(i, n) tmp[i] = 0;
	rep(i, n) tmp[depth[i]] ++;
	rep(i, n-1) tmp[i+1] += tmp[i];
	rep(i, n) ord[-- tmp[depth[i]]] = i;
	return true;
}

