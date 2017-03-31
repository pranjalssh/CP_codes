struct dis_set
{
  vi p, r;
  dis_set() {}
  dis_set(int n): p(n), r(n) {FOR(i, 0, n - 1)p[i] = i;}

  int get(int i)
  {
    if (p[i] != i)p[i] = get(p[i]);
    return p[i];
  }
  int connect(int i, int j)
  {
    int x = get(i), y = get(j);
    if (x == y) return 0;
    if (r[x] < r[y])p[x] = y;
    else {p[y] = x; if (r[x] == r[y])++r[x];}
    return 1;
  }
};

struct BlockCutTree {
  vector<vector<pair<int, int> > > g;

  int n, m, dfsctr, root, child;
  vi dfs_num, dfs_low, depth, vis, is_art, low;
  vector<ii> e;
  dis_set edges;
  vvi blocks, toArt, inBlock;

  // special :
  vi here, ans, reached;
  vvi VIS;


  void dfs_pre(int u, int p, int e)
  {
    dfs_num[u] = dfs_low[u] = dfsctr++;
    low[u] = depth[u];
    for (auto it : g[u])
    {
      if (dfs_num[it.F] == -1)
      {
        depth[it.F] = depth[u] + 1;
        if (u == root)++child;
        dfs_pre(it.F, u, it.S);
        dfs_low[u] = min(dfs_low[u], dfs_low[it.F]);
        low[u] = min(low[u], low[it.F]);
        if (dfs_low[it.F] >= dfs_num[u])
          is_art[u] = 1;
        if (e != -1)if (low[it.F] < depth[u]) edges.connect(e, it.S);
      }
      else if (p != it.F)
      {
        if (e != -1)if (dfs_num[it.F] < dfs_num[u]) edges.connect(e, it.S);
        low[u] = min(low[u], depth[it.F]);
        dfs_low[u] = min(dfs_low[u], dfs_num[it.F]);
      }
    }
  }

  BlockCutTree (vvii &ig, int m): g(ig), m(m) {   // pair<to, edge_index>
    n = ig.size();

    e = vector<ii>(m);
    FOR (i, 0, n - 1) for (ii it : g[i]) e[it.S] = {i, it.F};

    low = is_art = dfs_low = depth = vis = vi(n);
    dfs_num = vi(n, -1);

    dfsctr = 0;
    root = 0, child = 0;
    edges = dis_set(m);
    dfs_pre(0, -1, -1);
    is_art[root] = child > 1;

    blocks = vvi(m);

    FOR (i, 0, m - 1) {
      blocks[edges.get(i)].pb(e[i].F);
      blocks[edges.get(i)].pb(e[i].S);
    }

    FOR (i, 0, m - 1) {
      sort(all(blocks[i]));
      blocks[i].erase(unique(all(blocks[i])), blocks[i].end());
    }
    toArt.resize(m), inBlock.resize(n);

    FOR (i, 0, m - 1) {
      for (int it : blocks[i]) {
        inBlock[it].pb(i);
        if (is_art[it]) {
          toArt[i].pb(it);
        }
      }
    }

  }

  void dfs(int u, bool isBlock) {
    if (isBlock and reached[u]) {
      ans = here;
      return;
    }
    VIS[u][isBlock] = 1;

    if (isBlock) {
      FOR (i, 0, sz(toArt[u]) - 1) {
        int v = toArt[u][i];
        if (!VIS[v][0]) {
          dfs(v, 0);
        }
      }
    }
    else {
      here.pb(u);
      FOR (i, 0, sz(inBlock[u]) - 1) {
        int v = inBlock[u][i];
        if (!VIS[v][1]) {
          dfs(v, 1);
        }
      }
      here.pop_back();
    }
  }

  void solve(int s, int e) {
    reached = vi(m);
    FOR (i, 0, sz(inBlock[e]) - 1) {
      reached[inBlock[e][i]] = 1;
    }
    VIS = vvi (m, vi(2));

    dfs(inBlock[s][0], 1);

    if (find(all(ans), s) != ans.end())
      ans.erase(find(all(ans), s));

    if (find(all(ans), e) != ans.end())
      ans.erase(find(all(ans), e));

    cout << sz(ans) << "\n";
    sort(all(ans));
    FOR (i, 0, sz(ans) - 1) {
      cout << ans[i] + 1 << "\n";
    }
  }
};