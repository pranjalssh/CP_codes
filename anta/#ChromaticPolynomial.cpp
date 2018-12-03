//未検証！template<int MOD>
struct ModInt {
	static const int Mod = MOD;
	unsigned x;
	ModInt(): x(0) { }
	ModInt(signed sig) { if((x = sig % MOD + MOD) >= MOD) x -= MOD; }
	unsigned get() const { return x; }
	
	ModInt &operator+=(ModInt that) { if((x += that.x) >= MOD) x -= MOD; return *this; }
	ModInt &operator-=(ModInt that) { if((x += MOD - that.x) >= MOD) x -= MOD; return *this; }
	ModInt &operator*=(ModInt that) { x = (unsigned long long)x * that.x % MOD; return *this; }
	
	ModInt operator+(ModInt that) const { return ModInt(*this) += that; }
	ModInt operator-(ModInt that) const { return ModInt(*this) -= that; }
	ModInt operator*(ModInt that) const { return ModInt(*this) *= that; }

	bool operator==(ModInt that) const { return x == that.x; }
};
typedef ModInt<1000000007> mint;

struct Polynomial {
	typedef mint Coef;
	vector<Coef> coef;	//... + coef[2] x^2 + coef[1] x + coef[0]
	Polynomial() {}
	explicit Polynomial(int n): coef(n) {}
	bool iszero() const { return coef.empty(); }
	int degree1() const { return coef.size(); }	//degree + 1
	const Coef operator[](int i) const {
		return i >= degree1() ? Coef() : coef[i];
	}
	void canonicalize() {
		int i = coef.size();
		while(i > 0 && coef[i-1] == Coef()) i --;
		coef.resize(i);
	}
	Coef evalute(Coef x) const {
		int d = degree1();
		Coef t = 0, y = 1;
		rep(i, d) {
			t += y * coef[i];
			y *= x;
		}
		return t;
	}
	Polynomial &operator+=(const Polynomial &that) {
		int d = resize(that.degree1());
		for(int i = 0; i < d; i ++) coef[i] += that[i];
		canonicalize();
		return *this;
	}
	Polynomial operator+(const Polynomial &that) const { return Polynomial(*this) += that; }
	Polynomial &operator-=(const Polynomial &that) {
		int d = resize(that.degree1());
		for(int i = 0; i < d; i ++) coef[i] -= that[i];
		canonicalize();
		return *this;
	}
	Polynomial operator-(const Polynomial &that) const { return Polynomial(*this) -= that; }
	Polynomial operator*(const Polynomial &that) const {
		if(iszero() || that.iszero()) return Polynomial();
		int x = degree1(), y = that.degree1(), d = x + y - 1;
		Polynomial res(d);
		rep(i, x) rep(j, y)
			res.coef[i+j] += coef[i] * that.coef[j];
		res.canonicalize();
		return res;
	}
	Polynomial &operator*=(const Polynomial &that) { return *this = *this * that; }

	int resize(int d) { if(degree1() < d) coef.resize(d); return d; }
};

void compute_lowlinks_dfs(int i, int parent, int &order, const vector<vector<int>> &g, vector<int> &ord, vector<int> &low, vector<bool> &art) {
	low[i] = ord[i] = order ++;
	art[i] = false;
	int children = 0;
	for(aut(j, g[i].begin()); j != g[i].end(); ++ j) {
		if(ord[*j] == -1) {
			compute_lowlinks_dfs(*j, i, order, g, ord, low, art);
			low[i] = min(low[i], low[*j]);
			if(ord[i] <= low[*j]) art[i] = true;
			children ++;
		}else if(*j != parent)
			low[i] = min(low[i], ord[*j]);
	}
	if(parent == -1) art[i] = children > 1;
}
void compute_lowlinks(const vector<vector<int>> &g, vector<int> &ord, vector<int> &low, vector<bool> &art) {
	int n = g.size();
	ord.assign(n, -1); low.assign(n, -1); art.assign(n, false);
	int order = 0;
	for(int i = 0; i < n; i ++) if(ord[i] == -1) {
		compute_lowlinks_dfs(0, -1, order, g, ord, low, art);
	}
}
inline bool is_bridge_edge(int i, int j, const vector<int> &ord, const vector<int> &low) {
	return ord[i] < low[j] || ord[j] < low[i];
}

int getrevedge(const vector<vi> &g, int i, int k) {
	each(l, g[k]) if(*l == i)
		return l - g[k].begin();
	return -1;
}

void delete_edge(vector<vi> &g, int i, int j) {
	int k = g[i][j];
	g[i].erase(g[i].begin() + j);
	g[k].erase(g[k].begin() + getrevedge(g, i, k));
}
void contract_edge(const vector<vi> &g, int i, int k, vector<vi> &contracted) {
	int n = g.size();
	int j = getrevedge(g, k, i), l = getrevedge(g, i, k);
	contracted.assign(n-1, vi());
	rep(v, n) rep(e, g[v].size()) {
		if((v == i && e == j) || (v == k && e == l)) ;
		else {
			int u = g[v][e];
			int vv = v == k ? i - (i > k) : v - (v > k);
			int uu = u == k ? i - (i > k) : u - (u > k);
			contracted[vv].pb(uu);
		}
	}
	rep(v, n-1) {
		sort(all(contracted[v]));
		contracted[v].erase(unique(all(contracted[v])), contracted[v].end());
	}
}
void get_cutted_graph(const vector<vi> &g, int pi, int i, vector<int> &color, int c) {
	if(color[i] != -1) return;
	color[i] = c;
	each(j, g[i]) if(*j != pi)
		get_cutted_graph(g, pi, *j, color, c);
}

const int MemoN = 10;
map<vector<vi>,Polynomial> memo;
Polynomial completegraphs[70];
Polynomial treegraphs[70];
//gはソートされている必要がある
Polynomial deletion_contraction(const vector<vi> &g) {
	int n = g.size();
	if(n <= MemoN) {
		map<vector<vi>,Polynomial>::iterator it = memo.find(g);
		if(it != memo.end()) return it->second;
	}
	int edges = 0;
	rep(i, n) edges += g[i].size();
	edges /= 2;
//	cerr << n <<", "<<edges << ", " <<deladd << endl;
	if(edges == 0) {
		Polynomial p(n+1);
		p.coef[n] = 1;
		return p;
	}
	rep(i, n) each(j, g[i]) if(i == *j)	//loop
		return Polynomial();
	if(edges == n * (n-1) / 2) {
		if(!completegraphs[n].iszero()) return completegraphs[n];
		Polynomial p(n+1), q(2);
		p.coef[0] = 1;
		rep(i, n) {
			q.coef[0] = -i;
			q.coef[1] = 1;
			p *= q;
		}
		return completegraphs[n] = p;
	}
	if(edges == n-1) {
		if(!treegraphs[n].iszero()) return treegraphs[n];
		Polynomial p(n+1), q(2);
		p.coef[0] = 1;
		q.coef[0] = -1;
		q.coef[1] = 1;
		rep(i, n-1) p *= q;
		q.coef[0] = 0;
		p *= q;
		return treegraphs[n] = p;
	}
	int ei = -1, ej = -1, ek = -1; bool cutedge = false, multiedge = false, artpoint = false;
	{	vector<int> ord, low; vector<bool> art;
		compute_lowlinks(g, ord, low, art);
		rep(i, n) if(ei == -1) rep(j, g[i].size()) {
			int e = g[i][j];
			if(is_bridge_edge(i, e, ord, low)) {
				ei = i, ej = j;
				cutedge = true;
				break;
			}
		}
		if(ei == -1) rep(i, n) if(art[i]) {
			ei = i;
			artpoint = true;
			break;
		}
	}
	if(ei == -1) {
		vector<int> deg(n);
		rep(i, n) each(j, g[i]) deg[i] ++, deg[*j] ++;
		/*
		if(edges >= n * (n - 1) / 2 / 2) {
			int maxdeg = -INF;
			rep(i, n) {
				int e = 0, j = 0;
				while(e < i) {
					if(j < (int)g[i].size() && g[i][j] == e) {
						e ++, j ++;
						continue;
					}
					if(maxdeg < max(deg[i], deg[e])) {
						maxdeg = max(deg[i], deg[e]);
						ei = i, ek = e;
					}
					e ++;
				}
			}
		}
		*/
		if(ei == -1) {
			int mindeg = INF;
			rep(i, n) each(j, g[i]) if(mindeg > min(deg[i], deg[*j])) {
				mindeg = min(deg[i], deg[*j]);
				ei = i;
				ej = j - g[i].begin();
			}
		}
	}
	Polynomial r;
	if(artpoint) {
		vector<int> color(n, -1);
		int colors = 0;
		each(j, g[ei]) if(color[*j] == -1)
			get_cutted_graph(g, ei, *j, color, colors ++);
		vector<int> counting(colors, 0), index(n, -1);
		rep(c, colors) counting[c] ++;
		index[ei] = 0;
		rep(i, n) if(i != ei) index[i] = counting[color[i]] ++;
		vector<vector<vi> > gs(colors);
		rep(c, colors) gs[c].assign(counting[c], vi());
		rep(i, n) each(j, g[i])
			gs[i == ei ? color[*j] : color[i]][index[i]].pb(index[*j]);
		rep(c, colors) rep(i, gs[c].size()) sort(all(gs[c][i]));
		Polynomial p(1);
		p.coef[0] = 1;
		rep(i, colors) p *= deletion_contraction(gs[i]);
		r = Polynomial(n+1);
		rep(i, n+1) r.coef[i] = p[colors-1+i];
	}else {
		if(ek == -1) ek = g[ei][ej];
		vector<vi> contracted;
		contract_edge(g, ei, ek, contracted);
		Polynomial p = deletion_contraction(contracted);
		if(cutedge) {
			Polynomial q(n+1);
			rep(i, p.degree1()) q.coef[i+1] = p[i];
			r = q - p;
		}else if(ej != -1) {
			vector<vi> deleted = g;
			delete_edge(deleted, ei, ej);
			r = deletion_contraction(deleted) - p;
		}else {
			vector<vi> added = g;
			added[ei].pb(ek); sort(all(added[ei]));
			added[ek].pb(ei); sort(all(added[ek]));
			r = deletion_contraction(added) + p;
		}
	}
	
	/*
	rep(i, n) each(j, g[i]) if(i < *j) cout << i << "--" << *j <<", ";
	for(int i = r.degree1()-1; i >= 0; i --)
		cout << r[i].get() << " x^" << i << (i == 0 ? "" : " + ");
	cout << endl;
//	*/

	if(n <= MemoN)
		return memo[g] = r;
	return r;
}

//////////////////////////////////////////////////////////////////////
//さらに
template<int MOD>
struct ModInt {
	static const int Mod = MOD;
	unsigned x;
	ModInt(): x(0) { }
	ModInt(signed sig) { if((x = sig % MOD + MOD) >= MOD) x -= MOD; }
	ModInt(signed long long sig) { if((x = sig % MOD + MOD) >= MOD) x -= MOD; }
	unsigned get() const { return x; }
	
	ModInt &operator+=(ModInt that) { if((x += that.x) >= MOD) x -= MOD; return *this; }
	ModInt &operator-=(ModInt that) { if((x += MOD - that.x) >= MOD) x -= MOD; return *this; }
	ModInt &operator*=(ModInt that) { x = (unsigned long long)x * that.x % MOD; return *this; }
	ModInt &operator/=(ModInt that) { return *this *= that.inverse(); }
	
	ModInt operator+(ModInt that) const { return ModInt(*this) += that; }
	ModInt operator-(ModInt that) const { return ModInt(*this) -= that; }
	ModInt operator*(ModInt that) const { return ModInt(*this) *= that; }
	ModInt operator/(ModInt that) const { return ModInt(*this) /= that; }
	
	ModInt inverse() const {
		long long a = x, b = MOD, u = 1, v = 0;
		while(b) {
			long long t = a / b;
			a -= t * b; std::swap(a, b);
			u -= t * v; std::swap(u, v);
		}
		return ModInt(u);
	}
	bool operator==(ModInt that) const { return x == that.x; }
};
typedef ModInt<1000000007> mint;

struct Polynomial {
	typedef mint Coef;
	vector<Coef> coef;	//... + coef[2] x^2 + coef[1] x + coef[0]
	Polynomial() {}
	explicit Polynomial(int n): coef(n) {}
	bool iszero() const { return coef.empty(); }
	int degree1() const { return coef.size(); }	//degree + 1
	const Coef operator[](int i) const {
		return i >= degree1() ? Coef() : coef[i];
	}
	void canonicalize() {
		int i = coef.size();
		while(i > 0 && coef[i-1] == Coef()) i --;
		coef.resize(i);
	}
	Coef evalute(Coef x) const {
		int d = degree1();
		Coef t = 0, y = 1;
		rep(i, d) {
			t += y * coef[i];
			y *= x;
		}
		return t;
	}
	Polynomial &operator+=(const Polynomial &that) {
		int d = resize(that.degree1());
		for(int i = 0; i < d; i ++) coef[i] += that[i];
		canonicalize();
		return *this;
	}
	Polynomial operator+(const Polynomial &that) const { return Polynomial(*this) += that; }
	Polynomial &operator-=(const Polynomial &that) {
		int d = resize(that.degree1());
		for(int i = 0; i < d; i ++) coef[i] -= that[i];
		canonicalize();
		return *this;
	}
	Polynomial operator-(const Polynomial &that) const { return Polynomial(*this) -= that; }
	Polynomial operator*(const Polynomial &that) const {
		if(iszero() || that.iszero()) return Polynomial();
		int x = degree1(), y = that.degree1(), d = x + y - 1;
		Polynomial res(d);
		rep(i, x) rep(j, y)
			res.coef[i+j] += coef[i] * that.coef[j];
		res.canonicalize();
		return res;
	}
	Polynomial &operator*=(const Polynomial &that) { return *this = *this * that; }
	pair<Polynomial, Polynomial> divmod(const Polynomial &that) const {
		int x = degree1() - 1, y = that.degree1() - 1;
		int d = max(0, x - y);
		Polynomial q(d + 1), r = *this;
		for(int i = x; i >= y; i --) {
			Coef t = r.coef[i] / that.coef[y];
			q.coef[i - y] = t;
			r.coef[i] = 0;
			if(t == 0) continue;
			for(int j = 0; j < y; j ++)
				r.coef[i - y + j] -= t * that.coef[j];
		}
		q.canonicalize(); r.canonicalize();
		return make_pair(q, r);
	}
	Polynomial operator/(const Polynomial &that) const { return divmod(that).first; }
	Polynomial operator%(const Polynomial &that) const { return divmod(that).second; }

	int resize(int d) { if(degree1() < d) coef.resize(d); return d; }
};

Polynomial operator^(Polynomial a, unsigned long long k) {
	Polynomial r(1); r.coef[0] = 1;
	while(k) {
		if(k & 1) r *= a;
		a *= a;
		k >>= 1;
	}
	return r;
}

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

void eliminationGame_minimumDegree(vector<vi> g, const vi &ordsub, vector<pii> &h) {
	int n = g.size();
	h.clear();
	set<pii> edges;
	rep(i, n) each(j, g[i]) edges.insert(mp(i, *j));
	vi deg(n); vector<bool> used(n);
	rep(i, n) each(j, g[i]) deg[*j] ++;
	rep(k, n) {
		int vk = -1;
		rep(i, n) if(!used[i] && (vk == -1 || deg[vk] * n + ordsub[vk] > deg[i] * n + ordsub[i]))
			vk = i;
//		cerr << k << ": " << vk+1 << endl;
		rep(ex, g[vk].size()) {
			int x = g[vk][ex];
			if(!used[x]) {
				deg[vk] --;
				deg[x] --;
				rep(ey, ex) {
					int y = g[vk][ey];
					if(!used[y] && !edges.count(mp(x, y))) {
						g[x].pb(y), g[y].pb(x);
						h.pb(mp(x, y));
						edges.insert(mp(x, y));
						edges.insert(mp(y, x));
						deg[x] ++, deg[y] ++;
					}
				}
			}
		}
		used[vk] = true;
	}
}

typedef vector<vector<int> > Graph;


bool isPerfectEliminationOrder(const Graph &g, const vector<int> &ord) {
	/*
		・parentの逆辺を作る
		・各頂点に対して、自分のNRをマークしてから、子どもたちにチェックさせる
	*/
	int n = g.size();
	vector<int> ord_inv(n);
	vector<int> headchild(n, -1), nextchild(n, -1);	//無駄に(リスト with 1次元)を使う(キャッシュ効率どうなんだろ？)
	vector<char> mark(n, 0);

	rep(i, n) ord_inv[ord[i]] = i;

	for(int i = n-1; i >= 0; i --) {	//childsのリストが昇順のほうが見やすいかなあって
		int parent = -1;
		for(vector<int>::const_iterator j = g[i].begin(); j != g[i].end(); ++ j)
			if(ord_inv[*j] < ord_inv[i] && (parent == -1 || ord_inv[parent] < ord_inv[*j]))
				parent = *j;
		if(parent != -1) {
			nextchild[i] = headchild[parent];
			headchild[parent] = i;
		}
	}
	rep(i, n) {
		if(headchild[i] == -1) continue;
		mark[i] = 1;
		for(vector<int>::const_iterator j = g[i].begin(); j != g[i].end(); ++ j)
			if(ord_inv[*j] < ord_inv[i]) mark[*j] = 1;
		int child = headchild[i];
		while(child != -1) {
			bool ok = true;
			for(vector<int>::const_iterator j = g[child].begin(); j != g[child].end(); ++ j)
				if(ord_inv[*j] < ord_inv[child])
					if(!mark[*j]) {
						ok = false;
						break;
					}
			if(!ok) return false;
			child = nextchild[child];
		}
		for(vector<int>::const_iterator j = g[i].begin(); j != g[i].end(); ++ j)
			if(ord_inv[*j] < ord_inv[i]) mark[*j] = 0;
		mark[i] = 0;
	}
	return true;
}


void cliqueTreeMCS(const Graph &g, vector<int> &out_ord, vector<int> &out_treenode, vector<int> &out_treeedge) {
	int n = g.size();
	out_ord.clear(); out_ord.reserve(n);
	out_treenode.assign(n, -1); out_treeedge.assign(n, -1);

	//prevlevelの方向にmaximumlevelがある(maximumlevel = l <-> prevlevel[l] = -1)
	vector<int>
		prevvertex(n, -2), nextvertex(n, -2), vertexlevel(n, -2),
		prevlevel(n+1, -2), nextlevel(n+1, -2), headvertex(n+1, -1),
		lastneighbor(n, -1);
	int maximumlevel = 0, previouslevel = 1, lastnode = -1;

	rep(i, n) vertexlevel[i] = 0, prevvertex[i] = i-1, nextvertex[i] = i+1;
	nextvertex[n-1] = -1, headvertex[0] = 0;
	prevlevel[0] = -1, nextlevel[0] = -1;

	rep(t, n) {
		int v = headvertex[maximumlevel];

		out_ord.push_back(v);

		if(maximumlevel <= previouslevel) {
			//孤立点をそのままやるとどこにも繋がれずに孤立したままになってしまうので、lastnodeにでもつないどく？
			out_treeedge[v] = lastneighbor[v] == -1 ? lastnode : out_treenode[lastneighbor[v]];
			lastnode = v;
		}
		out_treenode[v] = lastnode;
		previouslevel = maximumlevel;

		//remove v from maximumlevel and delete maximumlevel if it empty
		headvertex[maximumlevel] = nextvertex[v];
		if(nextvertex[v] != -1) prevvertex[nextvertex[v]] = -1;
		else {
			if(nextlevel[maximumlevel] != -1) prevlevel[nextlevel[maximumlevel]] = -1;
			maximumlevel = nextlevel[maximumlevel];
		}
		vertexlevel[v] = -1;

//		cout << vertexStr(v) << " | " << levelsStr(maximumlevel, nextlevel, headvertex, nextvertex) << endl;;

		//辺がソートされていればそのままの順序になるように、後ろからやってる(デバッグの時に見た目がいいように)
		for(vector<int>::const_reverse_iterator u = g[v].rbegin(); u != g[v].rend(); ++ u) {
			int ulevel = vertexlevel[*u];
			if(ulevel != -1) {
				lastneighbor[*u] = v;

				//remove u from ulevel
				if(prevvertex[*u] != -1) nextvertex[prevvertex[*u]] = nextvertex[*u];
				else headvertex[ulevel] = nextvertex[*u];
				if(nextvertex[*u] != -1) prevvertex[nextvertex[*u]] = prevvertex[*u];

				int prevnextlevel = prevlevel[ulevel], nextnextlevel = ulevel;
				//delete ulevel if it empty
				if(headvertex[ulevel] == -1) {
					nextnextlevel = nextlevel[ulevel];
					if(prevnextlevel != -1) nextlevel[prevnextlevel] = nextnextlevel;
					else maximumlevel = nextlevel[ulevel];
					if(nextnextlevel != -1) prevlevel[nextnextlevel] = prevnextlevel;
				}

				//create (ulevel+1) if it avoid
				if(prevnextlevel != ulevel+1) {
					headvertex[ulevel+1] = -1;
					prevlevel[ulevel+1] = prevnextlevel;
					nextlevel[ulevel+1] = nextnextlevel;

					if(prevnextlevel != -1) nextlevel[prevnextlevel] = ulevel+1;
					else maximumlevel = ulevel+1;
					if(nextnextlevel != -1) prevlevel[nextnextlevel] = ulevel+1;
				}

				//insert u into (ulevel+1)
				prevvertex[*u] = -1;
				nextvertex[*u] = headvertex[ulevel+1];
				if(headvertex[ulevel+1] != -1) prevvertex[headvertex[ulevel+1]] = *u;
				headvertex[ulevel+1] = *u;
				vertexlevel[*u] = ulevel+1;

//				cout << "increase level of " << vertexStr(*u) << " (" << ulevel << "+1): " << levelsStr(maximumlevel, nextlevel, headvertex, nextvertex) << endl;;
			}
		}
	}
}

void cliqueTreeMakeGraph(const Graph &g, const vector<int> &ord, const vector<int> &node, const vector<int> &edge, Graph &out_g, vector<vector<int> > &out_clique) {
	int n = g.size();

	//ノードを数え、連番をつける
	vector<int> nodenumber(n, -1);
	int nodes = 0;
	rep(i, n) if(node[i] == i) nodenumber[i] = nodes ++;

	out_g.assign(nodes, vector<int>());
	out_clique.assign(nodes, vector<int>());

	//まずエッジを
	rep(i, n) if(edge[i] != -1) {
		out_g[nodenumber[i]].push_back(nodenumber[edge[i]]);
		out_g[nodenumber[edge[i]]].push_back(nodenumber[i]);
	}

	//クリークのうちnodeにあるもの(PEO(普通の順序)で左にあるもの)を
	rep(i, n)
		out_clique[nodenumber[node[i]]].push_back(i);

	//クリークのうちNR(v)にあるもの(PEO(普通の順序)で右にあるもの)を
	vector<int> ord_rev(n);
	rep(i, n) ord_rev[ord[i]] = i;
	rep(v, n) if(nodenumber[v] != -1) {
		for(vector<int>::const_iterator u = g[v].begin(); u != g[v].end(); ++ u)
			if(ord_rev[*u] < ord_rev[v])
				out_clique[nodenumber[v]].push_back(*u);
	}
}

vector<Polynomial> cliquePolynomials;

pii chromaticPolynomial_triangulation_choiceFunction(const vector<vi> tree, const vector<vi> &cliques, const set<pii> &edges) {
	int n = tree.size();
	vector<int> subtreesize(n, -1), s;
	s.push_back(1);
	while(!s.empty()) {
		bool b = s.back() > 0; int i = abs(s.back())-1; s.pop_back();
		if(!b) {
			subtreesize[i] = -2;
			s.push_back(-(i+1));
			each(j, tree[i]) if(subtreesize[*j] == -1)
				s.push_back(*j + 1);
		}else {
			subtreesize[i] = 1;
			each(j, tree[i]) if(subtreesize[*j] != -2)
				subtreesize[i] += subtreesize[*j];
		}
	}
	vi partitionscore(n);
	vi sizes;
	rep(i, n) {
		sizes.clear();
		if(i != 0) sizes.push_back(subtreesize[0] - subtreesize[i]);
		each(j, tree[i]) if(subtreesize[*j] < subtreesize[i])
			sizes.push_back(subtreesize[*j]);
		partitionscore[i] = n - *max_element(all(sizes));
	}
	int bestscore = -1; pii best;
	rep(i, n) {
		rep(j, cliques[i].size()) rep(k, j) {
			int x = cliques[i][j], y = cliques[i][k];
			if(edges.count(mp(x, y))) continue;
			int score = partitionscore[i];
			if(bestscore < score) {
				bestscore = score;
				best = mp(x, y);
			}
		}
	}
	return best;
}

Polynomial chromaticPolynomial_triangulation_rec(int m, const vector<vi> tree, const vector<vi> &cliques, set<pii> &edges) {
	int n = tree.size();
	/*
	cerr << edges.size() << ", ";
	rep(i, n) {
		cerr << "[";
		each(j, cliques[i]) cerr << *j << ",";
		cerr << "],";
	}
	cerr << endl;
	*/
	rep(i, n)
		each(j, tree[i]) if(i == *j) return Polynomial();

	int cliqueokcount = 0, cliqueoki = -1;
	rep(i, n) {
		bool cliqueok = true;
		rep(j, cliques[i].size()) rep(k, j)
			cliqueok &= edges.count(mp(cliques[i][j], cliques[i][k])) != 0;
		if(cliqueok) {
			if(tree[i].size() > 1) cliqueoki = i;
			cliqueokcount ++;
		}
	}
	if(cliqueokcount == n) {
		if(n == 1) return cliquePolynomials[cliques[0].size()];

		vector<int> factcoef(m+1); vector<int> intersection;
		Polynomial r(1); r.coef[0] = 1;
		rep(i, n) {
			factcoef[(int)cliques[i].size()-1] ++;
			each(j, tree[i]) if(i < *j) {
				intersection.clear();
				set_intersection(all(cliques[i]), all(cliques[*j]),
					back_insert_iterator<vi>(intersection));
				factcoef[(int)intersection.size()-1] --;
			}
		}
		for(int i = m-1; i >= 0; i --)
			factcoef[i] += factcoef[i+1];
		rep(i, m+1) {
			Polynomial p(2);
			p.coef[0] = -i;
			p.coef[1] = 1;
			r *= p ^ factcoef[i];
		}
		return r;
	}
	if(cliqueoki != -1) {
		Polynomial r(1); r.coef[0] = 1;
		vi nvertices, indices(n, -1); vector<vi> ntree, ncliques;
		each(j, tree[cliqueoki]) {
			nvertices.clear();
			vector<pii> s;
			nvertices.pb(cliqueoki);
			s.pb(mp(*j, cliqueoki));
			while(!s.empty()) {
				pii p = s.back(); s.pop_back();
				int v = p.first;
				nvertices.pb(v);
				each(u, tree[v]) if(*u != p.second)
					s.push_back(mp(*u, v));
			}
			int card = 0;
			each(k, nvertices) indices[*k] = card ++;
			ntree.assign(card, vi());
			ncliques.assign(card, vi());
			each(k, nvertices) each(e, tree[*k]) {
				int x = indices[*k], y = indices[*e];
				if(x != -1 && y != -1)
					ntree[x].pb(y);
			}
			set<pii> nedges;
			each(k, nvertices) {
				ncliques[indices[*k]] = cliques[*k];
				each(ex, cliques[*k]) each(ey, cliques[*k])
					if(edges.count(mp(*ex, *ey))) nedges.insert(mp(*ex, *ey));
			}
			r *= chromaticPolynomial_triangulation_rec(m, ntree, ncliques, nedges);
			if(j != tree[cliqueoki].begin())
				r = r / cliquePolynomials[cliques[cliqueoki].size()];

			each(j, nvertices) indices[*j] = -1;
		}
		return r;
	}
	pii e = chromaticPolynomial_triangulation_choiceFunction(tree, cliques, edges);
	edges.insert(e); edges.insert(mp(e.second, e.first));
	Polynomial p1 = chromaticPolynomial_triangulation_rec(m, tree, cliques, edges);
	edges.erase(e); edges.erase(mp(e.second, e.first));
	vector<vi> ncliques = cliques;
	rep(i, n) {
		each(j, ncliques[i]) {
			if(*j == e.second) {
				*j = e.first;
				sort(all(ncliques[i]));
				ncliques[i].erase(unique(all(ncliques[i])), ncliques[i].end());
				break;
			}
		}
	}
	/*
	UnionFind uf(n);
	rep(i, n) each(j, tree[i])
		if(ncliques[i] == ncliques[*j]) uf.unionSet(i, *j);
	vector<int> indices(n, -1); int vertices = 0;
	rep(i, n) if(uf.root(i) == i)
		indices[i] = vertices ++;
	rep(i, n) indices[i] = indices[uf.root(i)];
	vector<vi> nncliques(vertices), ntree(vertices);
	rep(i, n) each(j, ncliques[i]) nncliques[indices[i]].pb(*j);
	rep(i, n) each(j, tree[i])
		ntree[indices[i]].pb(indices[*j]);
	rep(i, vertices) {
		sort(all(nncliques[i]));
		nncliques[i].erase(unique(all(nncliques[i])), nncliques[i].end());
		sort(all(ntree[i]));
		ntree[i].erase(unique(all(ntree[i])), ntree[i].end());
	}
	*/

	set<pii> nedges;
	each(i, edges) {
		if(i->first == e.second) nedges.insert(mp(e.first, i->second));
		else if(i->second == e.second) nedges.insert(mp(i->first, e.first));
		else nedges.insert(*i);
	}
	Polynomial p2 = chromaticPolynomial_triangulation_rec(m-1, tree, ncliques, nedges);
	return p1 + p2;
}

Polynomial chromaticPolynomial_triangulation(const vector<vi> &g, const vector<pair<int,int> > &triangulation) {
	int n = g.size();
	set<pii> edges;
	vector<vi> chordal(n);
	rep(i, n) each(j, g[i]) {
		chordal[i].pb(*j);
		edges.insert(mp(i, *j));
	}
	each(i, triangulation) {
		chordal[i->first].pb(i->second);
		chordal[i->second].pb(i->first);
	}
	vector<int> ord, treenode, treeedge;
	cliqueTreeMCS(chordal, ord, treenode, treeedge);
	if(!isPerfectEliminationOrder(chordal, ord)) cerr << "Error: is'n PerfectEliminationOrder" << endl;
	vector<vi> tree, cliques;
	cliqueTreeMakeGraph(chordal, ord, treenode, treeedge, tree, cliques);
	rep(i, cliques.size()) sort(all(cliques[i]));

	cliquePolynomials.resize(n+1);
	cliquePolynomials[0] = Polynomial(1);
	cliquePolynomials[0].coef[0] = 1;
	rep(i, n) {
		Polynomial p(2);
		p.coef[0] = -i;
		p.coef[1] = 1;
		cliquePolynomials[i+1] = cliquePolynomials[i] * p;
	}

	return chromaticPolynomial_triangulation_rec(n, tree, cliques, edges);
}
