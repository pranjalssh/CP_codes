#include <bits/stdc++.h>
using namespace std;

typedef long double Num;
typedef complex<Num> Complex;

void FFT(Complex P[], int n, int oper) {
  for (int i = 1, j = 0; i < n - 1; i++) {
    for (int s = n; j ^= s >>= 1, ~j & s;);
    if (i < j) swap(P[i], P[j]);
  }
  for (int d = 0; (1 << d) < n; d++) {
    int m = 1 << d, m2 = m * 2;
    Complex unit_p0(cos(-2*M_PI/m2*oper), sin(-2*M_PI/m2*oper));
    for (int i = 0; i < n; i += m2) {
      Complex unit(1);
      for (int j = 0; j < m; j++) {
        Complex &P1 = P[i + j + m], &P2 = P[i + j];
        Complex t = unit * P1;
        P1 = P2 - t; P2 = P2 + t;
        unit = unit * unit_p0;
      }
    }
  }
}
vector<int> mul(const vector<int> &a, const vector<int> &b) {
  vector<int> ret(max(0, (int) a.size() + (int) b.size() - 1), 0);
  int len = 1; while (len < (int)ret.size()) len <<= 1;
  static Complex A[100000], B[100000], C[100000];
  for (int i = 0; i < len; i++) A[i] = i < (int)a.size() ? a[i] : 0;
  for (int i = 0; i < len; i++) B[i] = i < (int)b.size() ? b[i] : 0;
  FFT(A, len, 1); FFT(B, len, 1);
  for (int i = 0; i < len; i++) C[i] = A[i] * B[i];
  FFT(C, len, -1); for (int i = 0; i < (int)ret.size(); i++) ret[i] = (long long)((C[i]/Complex(len,0)).real() + 0.5);
  return ret;
}

// online FFT

void f(int l, int r) {
	if (l == r) {
		c2[l] += a[l]*b[0];
		a[l+1] = c2[l];
		return;
	}
	int mid = (l + r) >> 1;
	f(l, mid);
	FOR (i, l, mid) FOR (j, mid+1, r) c2[j] += a[i]*b[j-i];
	f(mid+1, r);
}

{
	FOR (i, 0, N-1) b[i] = i+2;
	a[0] = 1;

	// naive
	FOR (i, 0, N-1) {
		FOR (j, 0, i) c0[i] += a[j]*b[i-j];
		a[i+1] = c0[i];
	}

	// optimized
	FOR (i, 0, N-1) {
		c1[i] += a[i]*b[0];
		c1[i+1] += a[i]*b[1];
		if (i) {
			int K = (1 << __builtin_ctz(i));
			for (int k = 2; k <= K; k*=2) {
				// do fft here
				FOR (j, i-k, i-1) {
					FOR (J, k, k+k-1) {
						c1[j+J] += a[j]*b[J];
					}
				}
			}
		}
		a[i+1] = c1[i];
	}
}

const int MOD = 100003;
vector<int> calc_dfs(const vector<int> &A, int l, int r) {
	if(r - l == 1) {
		vector<int> res(2);
		res[0] = 1;
		res[1] = A[l];
		return res;
	}
	int mid = (l + r) / 2;
	vector<int> L = calc_dfs(A, l, mid), R = calc_dfs(A, mid, r);
	vector<Complex> Lc(all(L)), Rc(all(R));
	convolution(Lc, Rc);
	int n = L.size() + R.size() - 1;
	vector<int> res(n);
	rep(i, n) res[i] = (long long)(Lc[i].real() + .5) % MOD;
	return res;
}