vi z_function(string s)
{
  int n = s.length();
  vi z(n);
  for (int i = 1, l = 0, r = 0; i < n; ++i)
  {
    if (i <= r)
      z[i] = min (r - i + 1, z[i - l]);
    while (i + z[i] < n && s[z[i]] == s[i + z[i]])
      ++z[i];
    if (i + z[i] - 1 > r)
      l = i, r = i + z[i] - 1;
  }
  return z;
}


struct SuffixArray {
  const int L;
  string s;
  vector<vector<int> > P;
  vector<pair<pair<int, int>, int> > M;

  SuffixArray(const string &s) : L(s.length()), s(s), P(1, vector<int>(L, 0)), M(L) {
    for (int i = 0; i < L; i++) P[0][i] = int(s[i]);
    for (int skip = 1, level = 1; skip < L; skip *= 2, level++) {
      P.push_back(vector<int>(L, 0));
      for (int i = 0; i < L; i++)
        M[i] = make_pair(make_pair(P[level - 1][i], i + skip < L ? P[level - 1][i + skip] : -1000), i);
      sort(M.begin(), M.end());
      for (int i = 0; i < L; i++)
        P[level][M[i].second] = (i > 0 && M[i].first == M[i - 1].first) ? P[level][M[i - 1].second] : i;
    }
  }

  vector<int> GetSuffixArray() { return P.back(); }

  // returns the length of the longest common prefix of s[i...L-1] and s[j...L-1]
  int LongestCommonPrefix(int i, int j) {
    int len = 0;
    if (i == j) return L - i;
    for (int k = P.size() - 1; k >= 0 && i < L && j < L; k--) {
      if (P[k][i] == P[k][j]) {
        i += 1 << k;
        j += 1 << k;
        len += 1 << k;
      }
    }
    return len;
  }
};





vi fail(string s)
{
  int n = s.length();
  vi F(n + 1);
  FOR(i, 2, n)
  {
    int x = F[i - 1];
    while (1)
    {
      if (s[i - 1] == s[x])
      {F[i] = x + 1; break;}
      if (x == 0) {F[i] = 0; break;}
      x = F[x];
    }
  }
  return F;
}

int i = 0;
int j = 0;
bool f = 0;
while (1)
{
  if (j == t.length())break;

  if (s[i] == t[j])
  {
    i++;
    j++;
    if (i == s.length())
    {

    }
  }

  else if (i > 0)i = F[i];

  else j++;
}


// Transform S into T.
// For example, S = "abba", T = "^#a#b#b#a#$".
// ^ and $ signs are sentinels appended to each end to avoid bounds checking
string pre(string s) {
  int n = s.size();
  if (!n)return "^$";
  string temp = "^";
  rep(i, n)
  {
    temp += "#" + s.substr(i, 1);
  }
  temp += "#$";
  return temp;
}

vi manacher(string a) {
  string s = pre(a);
  //cout<<s<<endl;
  int n = s.length();
  vi p(n);
  int c = 0, r = 0;
  FOR(i, 1, n - 2)
  {
    int mir = 2 * c - i;
    p[i] = (r > i) ? min(r - i, p[mir]) : 0;
    while (s[i + p[i] + 1] == s[i - p[i] - 1])p[i]++;
    if (i + p[i] > r)
    {
      c = i;
      r = i + p[i];
    }
  }

//   int len=0,idx=0;
//   FOR(i,1,n-2)
//     if(p[i] > len)idx=i,len=p[i];

//   return s.substr( (idx-1-len)/2 ,len);
  return p;
}

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

