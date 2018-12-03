vi p, st, en, l;
vector<vi> jump;
int n;
vector<vi> g;

void dfs(int u) {
   static int ctr = 0;
   st[u] = ++ctr;
   for (auto it : g[u]) {
      if (it == p[u]) continue;
      p[it] = u;
      l[it] = l[u] + 1;
      dfs(it);
   }
   en[u] = ++ctr;
}

void pre() {
   jump = vector<vi>(20, vi(n + 1));
   l = st = en = p = vi(n + 1);
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

int lca(int u, int v, int root) {
   int x = lca(u, v);
   if (insub(root, u) and insub(root, v)) return x;
   if (insub(root, u) xor insub(root, v)) return root;
   if (insub(x, root)) {
      NFOR (i, 19, 0) {
         int to = jump[i][root];
         if (to and !(insub(to, u) or insub(to, v)))
            root = to;
      }
      return p[root];
   }

   return x;
}