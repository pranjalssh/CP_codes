


struct matrix {
    int a[32][32];
    int n;
    int INF, INF2;
    matrix(int n, int INF) : n(n), INF(INF), INF2(INF*INF) {
        FOR(i, 0, n - 1) FOR (j, 0, n - 1) a[i][j] = i == j;
    }

    matrix(vvi b, int INF) : INF(INF), INF2(INF*INF) {
        n = sz(b);
        FOR(i, 0, sz(b) - 1) FOR(j, 0, sz(b) - 1) a[i][j] = b[i][j];
    }

    int* operator[](int x) {return a[x];}

    matrix operator*(matrix b) {
        matrix ans(n, INF);
        FOR (i, 0, n - 1) {
            FOR (j, 0, n - 1) {
                int x = 0;
                FOR (k, 0, n - 1) {
                    x += a[i][k] * b[k][j];
                    if (x >= INF2) x -= INF2;
                }
                ans[i][j] = x % INF;
            }
        }
        return ans;
    }
    matrix operator+(matrix b) {
        matrix ans(n, INF);
        FOR (i, 0, n - 1) {
            FOR (j, 0, n - 1) {
                ans[i][j] = a[i][j] + b[i][j];
                if (ans[i][j] >= INF) ans[i][j] -= INF;
            }
        }
        return ans;
    }
    matrix operator^(bigint pw) {
        matrix ans(n, INF), now = *this;
        while (pw > 0) {
            if (pw % 2 == 1)
                ans = ans * now;
            now = now * now;
            pw = pw / 2;
        }
        return ans;
    }
};