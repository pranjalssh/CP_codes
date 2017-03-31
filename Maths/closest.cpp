function <ll (int, int)> f = [&](int l, int r) -> ll {
  if (l == r) return inf2;
  int mid = (l + r) / 2;
  ll ans = min(f(l, mid), f(mid + 1, r));

  merge(a.begin() + l, a.begin() + mid + 1, 
  a.begin() + mid + 1, a.begin() + r + 1, temp.begin());
  copy(temp.begin(), temp.begin() + r - l + 1, a.begin() + l);
  
  FOR (i, l, r) FOR (j, i + 1, min(n - 1, i + 6))
    ans = min(ans, dist(a[i], a[j]));
  
  return ans;
};
