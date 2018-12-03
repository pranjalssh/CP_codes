/*
	ガウスの消去法。変数の個数と式の個数が一致した連立方程式を解ける。
	O(n^3)。ただし行列の乗算より重い。
	！少しのケースしかVerifyされていない！
	(A x^T = b^T) を解く。^Tはそれが縦であるということ。
		Aが左にあることに注意。[i][j]はjからiに足し込む。
	(a xn = b xm)という形の時は(a xn - b xm = 0) とする。
	同様に(a x + b = c)は(a x = c - b)とする。
	■行列累乗で計算できる漸化式の極限を求める
	◆geometric seriesの無限和という考え方がわかりやすい！公式が行列にも適用できる。
	確率や期待値で極限を取りたいことがある。それは連立方程式で表現できる。
	・その行列をマイナスにする
	・A[k][k] += 1	//↑と合わせて、(a xn + b xm = xp)を表している
	・極限を取りたい状態は係数1だけ？
	・b[極限を取りたい状態] = 1
	・x[最初の状態]が答え
	※わかってない
	※DPを何回もすればいいことも多い
	※状態数が大きい時にはO(n^3)はできないので注意
	※要求精度によっては行列累乗のほうが速いこともある(行列の乗算1回は速い)
	
*/

//配列バージョン(vectorバージョンより150%くらい速い)
typedef double Num;
const int VecSize = ここを書き換えること;
struct Vec {
	Num a[VecSize];
	Num &operator[](int i) { return a[i]; }
	Vec() {mset(a, 0);}
};
struct Mat {
	Num a[VecSize][VecSize];
	Num *operator[](int i) { return a[i]; }
	Mat() {mset(a, 0);}
};

Vec gaussian_elimination(Mat A, Vec b) {
	const int n = VecSize, m = VecSize;
	int pi = 0, pj = 0;
	while(pi < n && pj < m) {
		reu(i, pi+1, n)
			if(fabs(A[i][pj]) > fabs(A[pi][pj])) {
				rep(k, m) swap(A[i][k], A[pi][k]);
				swap(b[i], b[pi]);
			}
		if(fabs(A[pi][pj]) > 1e-7) {	//！注意！
			Num d = A[pi][pj];
			rep(j, m) A[pi][j] /= d;
			b[pi] /= d;
			reu(i, pi+1, n) {
				Num k = A[i][pj];
				rep(j, m) A[i][j] -= k * A[pi][j];
				b[i] -= k * b[pi];
			}
			pi ++;
		}
		pj ++;
	}
	reu(i, pi, n) if(fabs(b[i]) > 0) return Vec();	//Inconsistent
	if(pi < m || pj < m) return Vec();	//Ambiguous
	for(int j = m-1; j >= 0; j --)
		rep(i, j)
			b[i] -= b[j] * A[i][j];
	return b;
}

//vectorバージョン
typedef double Num;
typedef vector<Num> Vec;
typedef vector<Vec> Mat;

Vec gaussian_elimination(Mat A, Vec b) {
	const int n = A.size(), m = A[0].size();
	int pi = 0, pj = 0;
	while(pi < n && pj < m) {
		reu(i, pi+1, n)
			if(abs(A[i][pj]) > abs(A[pi][pj]))
				A[i].swap(A[pi]), swap(b[i], b[pi]);
		if(abs(A[pi][pj]) > 0) {
			Num d = A[pi][pj];
			rep(j, m) A[pi][j] /= d;
			b[pi] /= d;
			reu(i, pi+1, n) {
				Num k = A[i][pj];
				rep(j, m) A[i][j] -= k * A[pi][j];
				b[i] -= k * b[pi];
			}
			pi ++;
		}
		pj ++;
	}
	reu(i, pi, n) if(abs(b[i]) > 0) return Vec();	//Inconsistent
	if(pi < m || pj < m) return Vec();	//Ambiguous
	for(int j = m-1; j >= 0; j --)
		rep(i, j)
			b[i] -= b[j] * A[i][j];
	return b;
}

//おまけ
void print_equations(Mat &A, Vec &b) {
	int n = VecSize, m = VecSize;
	rep(i, n) {
		vector<Num> a; vi v;
		rep(j, m) if(A[i][j] != 0) a.push_back(A[i][j]), v.push_back(j);
		if(a.empty()) {
			if(b[i] == 0) continue;
			cout << "  0";
		}else {
			rep(j, a.size()) {
				cout.width(4);
				if(j == 0) {
					if(a[j] == 1) cout << " ";
					else cout << a[j] << "*";
				} else if(abs(a[j]) == 1) cout << " ";
				else cout << abs(a[j]) << "*";
				cout << "x";
				cout.width(2), cout.flags(cout.left);
				cout << v[j] << (j+1 == a.size() ? "" : (a[j+1] < 0 ? " -" : " +"));
				cout.flags(cout.right);
			}
		}
		cout << " = "; cout.width(3), cout << b[i] << endl;
	}
}

//mod2バージョン
//<https://judge.npca.jp/submissions/detail/4658>
bool gaussian_elimination(vector<vector<bool> > A, vector<bool> b) {
	const int n = A.size(), m = A[0].size();
	vector<bool> used(n);
	rep(j, m) {
		int i = 0;
		while(i < n && (used[i] || !A[i][j]))
			i ++;
		if(i == n) continue;
		used[i] = true;
		rep(k, n) if(!used[k] && A[k][j]) {
			rep(l, m)
				A[k][l] = A[k][l] ^ A[i][l];
			b[k] = b[k] ^ b[i];
		}
	}
	rep(i, n) if(!used[i] && b[i]) return false;
	return true;
}


