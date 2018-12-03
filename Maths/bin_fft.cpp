void bfft(int a[], int l, int r, const vector<vector<ll>> &m = {{0, 1}, {1, 1}}) {
	if (l == r) return;
	int mid = (l + r) >> 1;
	FOR (i, l, mid) {
		int x = a[i];
		int y = a[mid + i - l + 1];
		a[i] = ((m[0][0]*x + m[0][1]*y) % inf + inf) % inf;
		a[mid + i - l + 1] = ((m[1][0]*x + m[1][1]*y) % inf + inf) % inf;
	}
	bfft(a, l, mid);
	bfft(a, mid+ 1, r);
}

void tffb(int a[], int l, int r, const vector<vector<ll>> &m = {{-1, 1}, {1, 0}}) {
	if (l == r) return;
	int mid = (l + r) >> 1;
	FOR (i, l, mid) {
		int x = a[i];
		int y = a[mid + i - l + 1];
		a[i] = ((m[0][0]*x + m[0][1]*y) % inf + inf) % inf;
		a[mid + i - l + 1] = ((m[1][0]*x + m[1][1]*y) % inf + inf) % inf;
	}
	tffb(a, l, mid);
	tffb(a, mid+ 1, r);
}
// 1 1 1 -1 for XOR
// 1 1 1 0 for OR