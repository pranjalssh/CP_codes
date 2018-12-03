void visit(const vector<vector<int> > &g, int v, vector< vector<int> >& scc,
		vector<int> &S, vector<char> &inS,
		vector<int> &low, vector<int> &num, int& time) {
	low[v] = num[v] = ++time;
	S.push_back(v); inS[v] = true;
	each(e, g[v]) {
		int w = *e;
		if (num[w] == 0) {
			visit(g, w, scc, S, inS, low, num, time);
			low[v] = min(low[v], low[w]);
		} else if (inS[w])
			low[v] = min(low[v], num[w]);
	}
	if (low[v] == num[v]) {
		scc.push_back(vector<int>());
		while (1) {
			int w = S.back(); S.pop_back(); inS[w] = false;
			scc.back().push_back(w);
			if (v == w) break;
		}
	}
}
void stronglyConnectedComponents(const vector<vector<int> >& g,
		vector< vector<int> >& scc) {
	const int n = g.size();
	vector<int> num(n), low(n);
	vector<int> S;
	vector<char> inS(n);
	int time = 0;
	rep(u, n) if (num[u] == 0)
		visit(g, u, scc, S, inS, low, num, time);
}




//ノード -> 色のvectorを返すver.

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

//////////////////////////////////////////////////////////////////////////////
//未検証！
int outdegree1graph_scc(const int f[], int n, int c[]) {
	rep(i, n) c[i] = -1;
	int t = 0;
	rep(i, n) if(c[i] == -1) {
		int x = i, u = t;
		do {
			c[x] = t ++;
			x = f[x];
		}while(x != -1 && c[x] == -1);
		if(x != -1 && u <= c[x]) {
			int s = c[x];
			do {
				c[x] = s;
				x = f[x];
			}while(c[x] != s);
			t = s + 1;
		}
	}
	return t;
}

