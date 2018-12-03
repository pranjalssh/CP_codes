/*
	Knuth Morris Pratt algorithm
	クヌース-モリス-プラット法
	O(n + k)の文字列検索アルゴリズム
	
*/

typedef int Index;
Index *kmp_build(const char *p, int m) {
	Index *fail = new Index[m+1];
	fail[0] = -1;
	for(int i = 1, j = -1; i <= m; i ++) {
		while(j >= 0 && p[j] != p[i-1]) j = fail[j];
		fail[i] = ++ j;
	}
	return fail;
}
int kmp_match(const char *s, int n, const char *p, int m, const Index *fail) {
	int cnt = 0;
	for(int i = 0, k = 0; i < n; i ++) {
		while(k >= 0 && p[k] != s[i]) k = fail[k];
		if(++ k == m) {
			cnt ++;	//s[i-m+1 .. i]
			k = fail[k];
		}
	}
	return cnt;
}

