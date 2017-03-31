vvi g;
vi st, en, p, w, sw;
vi path, ans;
vi nf, wf;
vvi jump;


void dfs(int u) {
    static int ctr = -1;
    st[u] = ++ctr;
    path.pb(u);
    for (int v : g[u]) {
        if (v == p[u]) continue;
        p[v] = u;
        dfs(v);
    }
    en[u] = ++ctr;
    path.pb(u);
}

void pre(int n) {
    p = st = en = vi(n + 1);
    dfs(1);
    jump = vvi(20, vi(n + 1));
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

int ans_now = 0;

void add_mo(int pos) {
    if (nf[path[pos]] & 1) {
        ans_now -= !(--wf[w[path[pos]]]);
    }
    else {
        ans_now += !(wf[w[path[pos]]]++);
    }
    nf[path[pos]] ^= 1;
}

void remove_mo(int pos) {
    if (nf[path[pos]] & 1) {
        ans_now -= !(--wf[w[path[pos]]]);
    }
    else {
        ans_now += !(wf[w[path[pos]]]++);
    }
    nf[path[pos]] ^= 1;
}


vector<pair<ii, ii>> q;
FOR (i, 0, m - 1) {
    int u, v; cin >> u >> v;
    int LCA = lca(u, v);
    if (st[u] > st[v]) swap(u, v);

    if (LCA == u) {
        q.pb({{st[u], st[v]}, { -1, i}});
    }
    else {
        q.pb({{en[u], st[v]}, {st[LCA], i}});
    }
}


int no = sz(path);
const int K = sqrt(no);

sort(all(q), [&K](const pair<ii, ii>& x, const pair<ii, ii>& y) {
    return x.F.F / K == y.F.F / K ? x.F.S < y.F.S : x.F.F / K < y.F.F / K;
});

int l = 0, r = 0;
ans = vi(m);
nf = vi(no);
wf = vi(sz(sw));

add_mo(0);  // ******** IMPORTANT


for (auto &it : q) {
    int ql = it.F.F, qr = it.F.S;

    while (r < qr) {
        ++r;
        add_mo(r);
    }
    while (r > qr) {
        remove_mo(r);
        --r;
    }
    while (l > ql) {
        --l;
        add_mo(l);
    }
    while (l < ql) {
        remove_mo(l);
        ++l;
    }

    if (~it.S.F) add_mo(it.S.F);
    ans[it.S.S] = ans_now;
    if (~it.S.F) remove_mo(it.S.F);
}

FOR (i, 0, m - 1) cout << ans[i] << "\n";


