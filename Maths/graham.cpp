
ii operator-(const ii& x, const ii& y) {
  return ii(x.F - y.F, x.S - y.S);
}

ll dist2(const ii& x, const ii& y) {
  return (x.F-y.F)*1LL*(x.F-y.F) + (x.S-y.S)*1LL*(x.S-y.S);
}
ll cross(const ii& x, const ii& y) {
  return x.F * 1LL* y.S - x.S * 1LL * y.F;
}

vector<ii> Graham(vector<ii> a) {
  if (a.empty()) return {};
  int n = sz(a);
  swap(*a.begin(), *min_element(all(a)));
  sort(a.begin() + 1, a.end(), [&](const ii& x, const ii& y) {
    ll c = cross(x - a[0], y - a[0]);
    return c ? (c > 0) : (dist2(a[0], x) < dist2(a[0], y));
  });

  vector<ii> ans;
  int m = 0;
  FOR (i, 0, n - 1) {
    if (m <= 1 or cross(ans[m-1] - ans[m-2], a[i] - ans[m - 1]) >= 0) ans.push_back(a[i]), ++m; // remove = to remove redundant
    else ans.pop_back(), --i, --m;
  }
  
  return ans;
}
