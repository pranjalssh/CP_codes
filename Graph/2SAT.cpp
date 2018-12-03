//2*n (nは変数の数) のグラフを作る。
//i (0≦i＜n)の頂点は変数iのリテラルに対応し、n+iの頂点は変数iの否定のリテラルに対応する。
//x→y (xならばy)というリテラルx,yに対して、xからyへの辺を張る。
//頂点iと頂点n+iが同じ強連結成分に入っている iff unsatisfiable
//適切に非再帰のSCCにしたりStaticGraphを使ったりすること！

void visit(const vector<vector<int> > &g, int v,
		vector<int> &scccolor, int &colors,
		vector<int> &S, vector<char> &inS,
		vector<int> &low, vector<int> &num, int& time) {
	low[v] = num[v] = ++time;
	S.push_back(v); inS[v] = true;
	for(int e: g[v]) {
		int w = e;
		if (num[w] == 0) {
			visit(g, w, scccolor, colors, S, inS, low, num, time);
			low[v] = min(low[v], low[w]);
		} else if (inS[w])
			low[v] = min(low[v], num[w]);
	}
	if (low[v] == num[v]) {
		while (1) {
			int w = S.back(); S.pop_back(); inS[w] = false;
			scccolor[w] = colors;
			if (v == w) break;
		}
		colors ++;
	}
}
int stronglyConnectedComponents(const vector<vector<int> >& g,
		vector<int>& scccolor) {
	const int n = g.size();
	vector<int> num(n), low(n);
	vector<int> S;
	vector<char> inS(n);
	scccolor.resize(n);
	int time = 0, colors = 0;
	for (int u = 0; u < n; ++u) if (num[u] == 0)
		visit(g, u, scccolor, colors, S, inS, low, num, time);
	return colors;
}

bool two_satisfiability(const vector<vi> &g) {
	int n = g.size() / 2;
	vi scccolor;
	stronglyConnectedComponents(g, scccolor);
	for (int i = 0; i < n; ++i) if(scccolor[i] == scccolor[n+i]) return false;
	return true;
}





struct BridgeTree {
  vector<vi> g;
  int n, ctr;
  vi num, in, low, root;
  stack <int> bc;
  // TAKE CARE OF MULTIPLE EDGES IN j != par if undir.
  void dfs(int i, int par) {
    num[i] = low[i] = ++ctr;
    bc.push(i); in[i] = 1;
    for (int j : g[i]) {
      if (num[j] == -1) dfs(j, i);
      if (in[j]) low[i] = min(low[i], low[j]); // FOR DIRECTED REMOVE j != par
    }
    if (low[i] == num[i]) {
      while (bc.top() != i)
        root[bc.top()] = i, in[bc.top()] = 0, bc.pop();
      root[bc.top()] = i, in[bc.top()] = 0, bc.pop();
    }
  }
  BridgeTree() {}
  BridgeTree(vector<vi>& g) : g(g), n(g.size()), ctr(0), num(n, -1), in(n), low(n), root(n) {
    FOR (i, 0, n - 1) if (num[i] == -1) dfs(i, -1);
  }
  vi getCol() {
    vector<vi> t(n);
    vi d(n); 
    FOR(i, 0, n - 1) {
      for (int j : g[i]) {
        // if (i > j)continue;
        if (root[i] != root[j])
          t[root[i]].push_back(root[j]), d[root[j]]++;
          // t[root[j]].push_back(root[i]);
      }
    }
    // pr(g);
    vector<vi> comp(n);
    FOR (i, 0, n - 1) comp[root[i]].push_back(i);
    vi cons, topo;
    FOR (i, 0, n - 1) {
        if (d[i] == 0) cons.push_back(i);
    }
    while (!cons.empty()) {
        int x = cons.back();
        cons.pop_back();
        topo.push_back(x);
        for (int y : t[x]) {
            d[y]--;
            if (d[y] == 0) cons.push_back(y);
        }
    }

    vi col(n / 2, -1);

    for (int i : topo) {
        for (int j : comp[i]) {
            int x = j;
            if (x >= n/2) x -= n/2;
            if (col[x] == -1) {
                if (j >= n/2) col[x] = 0;
                else col[x] = 1;
            }
        }
    }
    return col;
  }

};