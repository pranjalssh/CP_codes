//Verified: <http://www.e-olimp.com/en/solutions/1339472>

//arrangementというのは重複を含まないで要素が0..m-1のordered列
//このarrangementの辞書順での位置を返す(これより辞書順で小さいものの個数)
long long arrangementToRank(const int x[], int n, int m) {
	vector<long long> arrangements(m+1);	//A000522。事前に計算しておくことができる
	arrangements[0] = 1;
	for(int i = 1; i <= m; i ++) arrangements[i] = arrangements[i-1] * i + 1;

	vector<bool> used(m);
	long long r = 0;
	for(int i = 0; i < n; i ++) {
		r ++;	//長さiのprefix
		int ltcnt = 0;
		for(int j = 0; j < x[i]; j ++) if(!used[j])	//Fenwick木が使える
			ltcnt ++;
		r += arrangements[m - i - 1] * ltcnt;
		used[x[i]] = true;
	}
	return r;
}

//xに返す(長さは最大でm必要)。返り値は長さ(存在しない場合-1)
int arrangementFromRank(long long k, int m, int x[]) {
	vector<long long> arrangements(m+1);	//A000522。事前に計算しておくことができる
	arrangements[0] = 1;
	for(int i = 1; i <= m; i ++) arrangements[i] = arrangements[i-1] * i + 1;
	if(k >= arrangements[m]) return -1;

	vector<bool> used(m);
	int i;
	for(i = 0; i < m && k > 0; i ++) {
		k --;
		int a = (int)(k / arrangements[m - i - 1]);
		int b = 0;
		for(int t = a; t >= 0; b ++) t -= !used[b];	//平衡木が使える
		x[i] = b - 1;
		used[b - 1] = true;
		k -= arrangements[m - i - 1] * a;
	}
	return i;
}
