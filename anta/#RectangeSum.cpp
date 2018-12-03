/*
	チェックしてないので注意
	サイズが大きいので、"必ず"
	スタック上でなくグローバルに定義すること！
	すぐにオーバーフローする(intの場合、例えば50*50に全部1000000を入れたら駄目？)
	なので、そういう場合long longを使うこと。
*/

template<typename T, int MaxH, int MaxW>
struct RectangeSum {
	T sum[MaxH+1][MaxW+1];
	template<typename U, unsigned W>
	void init(int h, int w, U a[][W]){
		rer(y, 0, h) rer(x, 0, w) sum[y][x] = (y == 0 || x == 0) ? 0 :
			sum[y-1][x] + sum[y][x-1] - sum[y-1][x-1] + a[y-1][x-1];
	}
	//[l, r), [t, b)
	inline T get(int t, int l, int b, int r) {
		return sum[b][r] - sum[b][l] - sum[t][r] + sum[t][l];
	}
};
