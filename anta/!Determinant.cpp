//最小全域木の数え上げ: <http://arc018.contest.atcoder.jp/submissions/136003>
//	<http://arc018.contest.atcoder.jp/submissions/136143>と比べて遅いのはdeterminantが悪いんだろうか、改良の余地ありか
//mint ver.
typedef mint Num;
typedef vector<Num> Vec;
typedef vector<Vec> Mat;
 
Num determinant(Mat A) {
	if(A.empty()) return 1;
	int n = A.size(), m = A[0].size();
	Num det = 1;
	vector<char> used(n, 0);
	rep(j, m) {
		int i = 0;
		while(i < n && (used[i] || A[i][j] == 0))
			i ++;
		if(i == n) continue;
		det *= A[i][j];
		if(i != j) det = -det;
		used[i] = true;
		rep(k, n) if(!used[k]) {
			Num coef = A[k][j] / A[i][j];
			rep(l, m)
				A[k][l] -= A[i][l] * coef;
		}
	}
	return det;
}



typedef long double Num;
typedef vector<Num> Vec;
typedef vector<Vec> Mat;

Num determinant(Mat A) {
	int n = A.size();
	Num det = 1;
	for(int i = 0; i < n; i ++) {
		int pivot = i;
		for(int j = i+1; j < n; j ++)
			if(abs(A[pivot][i]) < abs(A[j][i])) pivot = j;
		swap(A[i], A[pivot]);
		det *= A[i][i];
		if(i != pivot) det = -det;
		if(det == 0) break;
		Num r = Num(1) / A[i][i];
		for(int j = i+1; j < n; j ++) {
			Num u = A[j][i] * r;
			for(int k = n-1; k >= i; k --)
				A[j][k] -= u * A[i][k];
		}
	}
	return det;
}
