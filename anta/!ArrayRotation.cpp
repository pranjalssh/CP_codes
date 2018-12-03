template<typename T, size_t WW>
void rotate_left_square(T a[][WW], int n) {
	int n1 = n / 2, n2 = n - n1;
	rep(j, n1) rep(i, n2) {
		//a[j][i], a[n-1-i][j], a[n-1-j][n-1-i], a[i][n-1-j]
		T t = a[i][n-1-j];
		a[i][n-1-j] = a[n-1-j][n-1-i];
		a[n-1-j][n-1-i] = a[n-1-i][j];
		a[n-1-i][j] = a[j][i];
		a[j][i] = t;
	}
}
template<typename T, size_t WW>
void rotate_left(T a[][WW], int H, int W) {
	rotate_left_square(a, max(H, W));
	int pad = max(0, H - W);
	if(pad > 0) {
		for(int i = 0; i < W; i ++)
			memcpy(a[i], a[i+pad], H * sizeof(**a));
	}
}