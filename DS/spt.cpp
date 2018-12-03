template<class T>
struct Spt {
	vector<T> a;
	vector<vector<int>> dp;
	int n;
	Spt(const vector<T> &a) : a(a), dp(21), n(a.size()) {
		FOR (i, 0, 20) dp[i].resize(n);
		FOR (i, 0, n - 1) dp[0][i] = i;
		FOR (i, 1, 20) {
			FOR (j, 0, n - 1) if (j + (1 << i) <= n) {
				int x = dp[i-1][j];
				int y = dp[i-1][j + (1<<(i-1))];
				dp[i][j] = a[x] < a[y] ? x : y;
			}
		}
	}
	int rmq(int i, int j) {
		int lg = 31 - __builtin_clz(j-i+1);
		int x = dp[lg][i];
		int y = dp[lg][j - (1 << lg) + 1];
		return a[x] < a[y] ? x : y;
	}
};