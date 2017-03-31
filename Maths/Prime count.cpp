const int NR = 1e5 + 10;

ll L[NR]; // L[i] sieve for n / i
ll H[NR]; // H[i] sieve for i

void pre(ll n) {

  ll nr = sqrtl(n);

  FOR (i, 1, nr) L[i] = n / i - 1;
  FOR (i, 1, nr) H[i] = i - 1;

  for (ll i = 2; i <= nr; ++i) {
    if (H[i] == H[i - 1]) continue; // if i is composite, continue

    FOR (j, 1, nr) {
      if (i * i > n / j) break;
      L[j] -= (j * i <= nr ? L[j * i] : H[n / (j * i)]) - H[i - 1];
    }

    NFOR (j, nr, 2) {
      if (i * i > j) break;
      H[j] -= H[j / i] - H[i - 1];
    }
  }
}