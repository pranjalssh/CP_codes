struct CentroidDecomp {
	vvi g;
	int n, curn;
	vi sub, p, l, vis;	// vis for removed nodes
	vvi dist;	// distance from each level
	vi dd;	// dynamic distance

	void DFS(int root, int par)
	{
		curn++; sub[root] = 1;
		for (int it : g[root])
		{
			if (it == par or vis[it])continue;
			DFS(it, root);
			sub[root] += sub[it];
		}
	}
	int centroid(int root, int par)
	{
		for (int it : g[root]) if (!vis[it] and it != par and sub[it] > curn / 2) return centroid(it, root);
		return root;
	}
	void upd_dist(int u, int level, int par) {
		for (int v : g[u]) {
			if (v == par or vis[v]) continue;
			dist[level][v] = dist[level][u] + 1;
			upd_dist(v, level, u);
		}
	}
	void decompose(int root, int par)
	{
		curn = 0;
		DFS(root, -1);	// precompute sub
	
		int cent = centroid(root, -1);	// find centroid
		if (~par) l[cent] = l[par] + 1;
		upd_dist(cent, l[cent], -1);	// update distances?
		
		vis[cent] = 1;
		p[cent] = par;
	
		for (int it : g[cent]) if (!vis[it] and it != par) decompose(it, cent);
	}

	CentroidDecomp() {}
	CentroidDecomp(vvi &g) : g(g), n(g.size()), sub(n), p(n), l(n), vis(n), dist(20, vi(n)), dd(n, inf) {decompose(0, -1);}

	void upd(int u) {
		int x = u;
		do {
			dd[u] = min(dd[u], dist[l[u]][x]);
		}while(~(u = p[u]));
	}
	int qry(int u) {
		int ans = inf;
		int x = u;
		do {
			ans = min(ans, dist[l[u]][x] + dd[u]);
		}while(~(u = p[u]));
		return ans;
	}
	void clear(int u) {
		do {
			dd[u] = inf;
		}while(~(u = p[u]));
	}

};