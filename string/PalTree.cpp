struct pal_tree {

  struct node {
    int nxt[26];
    int suff, pos, len, num;
    node() {
      FOR(i, 0, 25)nxt[i] = 0; suff = pos = len = num = 0;
    }
  };

  string s;
  int n;
  vector<node> T;
  int suff;
  int ans;

  int append(int i) {
    int no = suff;

    while (1) {
      int at = i - T[no].len - 1;
      if (at >= 0 and s[at] == s[i]) {
        if (T[no].nxt[s[i] - 'a']) {
          suff = T[no].nxt[s[i] - 'a'];
          return no;
        }
        break;
      }
      no = T[no].suff;
    }

    node now;
    now.pos = i;
    T.push_back(now);

    suff = sz(T) - 1;
    T.back().len = T[no].len + 2;
    T[no].nxt[s[i] - 'a'] = sz(T) - 1;

    if (T.back().len == 1) {
      T.back().suff = 2;
      T.back().num = 1;
      return sz(T) - 1;
    }

    while (1) {
      no = T[no].suff;
      int at = i - T[no].len - 1;

      if (at >= 0 and s[at] == s[i]) {
        T.back().suff = T[no].nxt[s[i] - 'a'];
        break;
      }
    }
    T.back().num = 1 + T[T.back().suff].num;
    return sz(T) - 1;

  }

  pal_tree(string s): s(s), n(s.length()), T(3), suff(2) {
    T[1].suff = T[2].suff = 1;
    T[1].len = -1; T[2].len = 0;
    T[1].pos = -1; T[2].pos = -1;
    ans = 0;
    FOR(i, 0, n - 1)
    append(i),  ans += T[suff].num;
  }
};