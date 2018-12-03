//！！！！！なんかバグってるっぽい？？？？？！！！！！
//！！！！！！！バグってる！！！！！！！！！
/*
//高速化？: https://judge.npca.jp/submissions/detail/3821
//本当に高速化したいなら(j,文字)で全部テーブル作るのがいいと思う

typedef int Index;
const int AlphaSize = 26;
typedef pair<Index*, Index*> BM;

BM bm_build(const char *p, int m) {
	Index *next = new Index[m];
	for(int i = 0; i < m; i ++) next[i] = 2 * m - i - 1;
	Index *g = new Index[m];
	int j = m;
	for(int i = m-1; i >= 0; i --, j --) {
		g[i] = j;
		while(j < m && p[j] != p[i]) {
			next[j] = min(next[j], (Index)(m - i - 1));
			j = g[j];
		}
	}
	for(int i = 0; i < m; i ++) {
		next[i] = min(next[i], (Index)(j + m - i));
		if(i >= j) j = g[j];
	}
	delete[] g;

	Index *skip = new Index[AlphaSize];
	for(int i = 0; i < m; i ++) skip[p[i]] = m - i - 1;

	return BM(skip, next);
}
int bm_match(const char *s, int n, const char *p, int m, const BM bm) {
	const Index *skip = bm.first, *next = bm.second;
	int cnt = 0;
	for(int i = m-1; i < n; ) {
		int j = m-1;
		while(j >= 0 && s[i] == p[j]) i --, j --;
		if(j < 0) {
			cnt ++;	//s[i+1..i+m]
			i += m + 1;
		}else i += max(skip[s[i]], next[j]);
	}
	return cnt;
}
*/
