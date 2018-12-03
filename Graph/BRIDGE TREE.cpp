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
      if (j != par and in[j]) low[i] = min(low[i], low[j]); // FOR DIRECTED REMOVE j != par
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
  vector<vi> getTree() {
    vector<vi> t(n);
    FOR(i, 0, n - 1) {
      for (int j : g[i]) {
        if (i > j)continue;
        if (root[i] != root[j])
          t[root[i]].push_back(root[j]),
          t[root[j]].push_back(root[i]);
      }
    }
    return t;
  }

};

