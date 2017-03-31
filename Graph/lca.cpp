struct LCA {   // 1-based
   vvi *g;
   vi p, st, en;
   vvi jump;
   int n;

   LCA() {}
   LCA(vvi &g) : g(&g), n(g.size()) {pre();}

   void dfs(int u) {
      static int ctr = 0;
      st[u] = ++ctr;
      for (auto it : (*g)[u]) {
         if (it == p[u]) continue;
         p[it] = u;
         dfs(it);
      }
      en[u] = ++ctr;
   }

   void pre() {
      jump = vvi(20, vi(n + 1));
      st = en = p = vi(n + 1);
      dfs(1);
      FOR (i, 1, n) jump[0][i] = p[i];
      FOR (i, 1, 19) {
         FOR (j, 1, n) {
            jump[i][j] = jump[i - 1][jump[i - 1][j]];
         }
      }
   }

   int lca(int u, int v) {
      if (st[u] >= st[v] and st[u] <= en[v]) return v;
      NFOR (i, 19, 0) {
         int to = jump[i][v];
         if (to and !(st[u] >= st[to] and st[u] <= en[to]))
            v = to;
      }
      return p[v];
   }
};