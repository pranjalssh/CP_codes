struct BridgeTree {
  vvi g;
  int n, ctr;
  vi num, in, low, root;
  stack <int> bc;
  // TAKE CARE OF MULTIPLE EDGES IN j != par if undir.
  void dfs(int i, int par) {
    num[i] = low[i] = ++ctr;
    bc.push(i); in[i] = 1;
    for (int j : g[i]) {
      if (num[j] == -1) dfs(j, i);
      if (j != par and in[j]) low[i] = min(low[i], low[j]); // FOR DIRECTED REMOVE j != par
    }
    if (low[i] == num[i]) {
      while (bc.top() != i)
        root[bc.top()] = i, in[bc.top()] = 0, bc.pop();
      root[bc.top()] = i, in[bc.top()] = 0, bc.pop();
    }
  }
  BridgeTree() {}
  BridgeTree(vvi& g) : g(g), n(g.size()), ctr(0), num(n, -1), in(n), low(n), root(n) {
    dfs(0, -1);
  }
  vvi getTree() {
    vvi t(n);
    FOR(i, 0, n - 1) {
      for (int j : g[i]) {
        if (i > j)continue;
        if (root[i] != root[j])
          t[root[i]].pb(root[j]),
          t[root[j]].pb(root[i]);
      }
    }
    return t;
  }

};

