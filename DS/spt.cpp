
vvi r(1, vi(n, -1));


FOR(i, 0, n - 1)r[0][i] = i;
for (int i = 1; (1 << i) <= n; ++i) {
    r.push_back(vi(n, -1));
    for (int j = 0; j + (1 << i) <= n; ++j) {
        r[i][j] = a[r[i - 1][j]] > a[r[i - 1][j + (1 << (i - 1))]] ? r[i - 1][j] : r[i - 1][j + (1 << (i - 1))];
    }
}

auto rmq = [&](int l, int r) {
    int lg = 31 - __builtin_clz(r - l + 1);
    return a[r[lg][l]] > a[r[lg][r - (1 << lg) + 1]] ? r[lg][l] : r[lg][r - (1 << lg) + 1];
};