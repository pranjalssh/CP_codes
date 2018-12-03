const int MAXN = 1e6+10;
map <char, int> to[MAXN];
int fail[MAXN], que[MAXN], sz = 1;
bool out[MAXN]; // whether some string is accepted by ith state, use bitset otherwise

void addStr(const string& s) {
    int v = 0;
    rep (i, sz(s)) {
        char c = s[i];
        if (!to[v][c]) {
            to[v][c] = sz++;
        }
        v = to[v][c];
    }
    out[v] = true;
}

void pushLinks() {
    memset(fail, -1, sizeof(fail));
    int st = 0, fi = 0;
    for (char c = 'a'; c <= 'z'; ++c) {
        if (!to[0].count(c)) {
            to[0][c] = 0;
        } else {
            que[fi++] = to[0][c];
            fail[to[0][c]] = 0;
        }
    }
    fail[0] = 0;
    while (st < fi) {
        int v = que[st++];
        for (auto it : to[v]) {
            int u = it.second;
            assert(u != 0);
            if (u == 0) continue;
            char c = it.first;
            int j = fail[v];
            while (j != -1 && !to[j].count(c)) j = fail[j];
            if (j != -1) {
                fail[u] = to[j][c];
                out[u] |= out[fail[u]];
            }
            que[fi++] = u;
        }
    }
}
inline int getNext(int state, char c) {
    if (c < 'a' or c > 'z') {
        return 0;
    }
    while (!to[state].count(c)) {
        state = fail[state];
    }
    return to[state][c];
}
