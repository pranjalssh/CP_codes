
typedef long double Num;
typedef vector<Num> Vec;
typedef vector<Vec> Mat;

void reduced_row_echelon_form(Mat &A) {
	int n = A.size(), m = A[0].size();
	int lead = 0;
	for(int row = 0; row < n; row ++) {
		if(lead >= m) return;
		int i = row;
		while(A[i][lead] == 0) {
			if(++ i >= n) {
				i = row;
				if(++ lead >= m) return;
			}
		}
		swap(A[i], A[row]);
		Num d = A[row][lead];
		rep(j, m) A[row][j] /= d;
		for(int i = 0; i < n; i ++) if(i != row) {
			Num t = -A[i][lead];
			for(int j = 0; j < m; j ++)
				A[i][j] += t * A[row][j];
		}
	}
}
