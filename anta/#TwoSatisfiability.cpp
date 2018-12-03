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
	each(e, g[v]) {
		int w = *e;
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
	rep(u, n) if (num[u] == 0)
		visit(g, u, scccolor, colors, S, inS, low, num, time);
	return colors;
}

bool two_satisfiability(const vector<vi> &g) {
	int n = g.size() / 2;
	vi scccolor;
	stronglyConnectedComponents(g, scccolor);
	rep(i, n) if(scccolor[i] == scccolor[n+i]) return false;
	return true;
}
