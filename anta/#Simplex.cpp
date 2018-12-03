/*
	線形計画問題を単体法で解く。
	Maximize:
		c * x
	Subject to:
		A * x <= b
		x >= 0
	ここで、(<=)はベクトルの対応する要素が全て(<=)であることを表す
	
	■！注意！
		最大化、MAXIMIZEである！
		誤差・オーバーフローには注意！
		計算量も最悪では指数くらい悪いらしいので注意！
		Unbounded, NoSolutionはいちおう分けられるようになっているが、
		検証していないので違うかも。注意
	
	■引数
	n: 変数の数
	m: 制約式の数
	A: Mat(m, Vec(n))
		A[i][j] で、変数jから制約式iに足し込む倍率を指定する
	b: Vec(m)
		b[j] で制約jの右辺を指定する
	c: Vec(n)
		c[i] で変数iの最終的なスコアへの足し込む倍率を指定する
	
	■制約式の変形
	・目的がminimize(最小化)の場合
		c[i]を全て*=-1
		最後に解を*=-1
	・制約が等値制約(ax + by + cz + ... = α)の場合
		制約式の数を2倍にする(m *= 2)
		(A[j][i] = x)を(A[j][i] = x, A[m/2+j][i] = -x)にする
		(b[j] = x)を(b[j] = x, b[m/2+j] = -x)にする
	・変数が負の値をとりうる場合
		変数の数を2倍にする(n *= 2)
		(A[j][i] = x)を(A[j][i] = x, A[j][n/2+i] = -x)にする
		(c[i] = x)を(c[i] = x, c[n/2+i] = -x)にする
	
	■変数の割り当てを取得したいとき
		一番最後に以下のコード
		Vec v(n+m+1, 0);
		rep(i, m) v[id[n+i]] = D[i][n];
		でvの[0,n)が変数の割り当てになっている
*/

typedef long double Num;
typedef vector<Num> Vec;
typedef vector<Vec> Mat;
const Num Inf = numeric_limits<Num>::infinity();
const Num NoSolution = numeric_limits<Num>::quiet_NaN();
#define IsSolution(x) (x == x)
const Num EPS = 1e-10;

Num simplex(const Mat& A, const Vec& b, const Vec& c) {
	const int m = A.size(), n = A[0].size();
	Mat D(m+1, Vec(n+1)); vi id(n+m+1);
	rer(i, 0, n+m) id[i] = i;
	rep(i, m) {
		rep(j, n) D[i][j] = -A[i][j];
		D[i][n] = b[i];
	}
	rep(j, n) D[m][j] = c[j];
	while(1) {
		int r = m, s = n+m;
		rep(i, m) if(D[i][n] < -EPS && id[n+r] > id[n+i]) r = i;
		rep(j, n) if(D[m][j] > EPS && id[s] > id[j]) s = j;
		if(r == m && s == n + m) break;
		if(id[n + r] < id[s]) {
			s = n + m;
			rep(j, n) if(D[r][j] > EPS && id[s] > id[j]) s = j;
		}else {
			r = m;
			rep(i, m) if(D[i][s] < -EPS && id[n+r] > id[n+i]) r = i;
		}
		if(r == m) { /* Unbounded */ return NoSolution; }
		if(s == n + m) { /* NoSolution */ return NoSolution; }
		swap(id[s], id[n+r]);
		D[r][s] = 1. / D[r][s];
		rer(j, 0, n) if(j != s) D[r][j] *= -D[r][s];
		rer(i, 0, m) if(i != r && abs(D[i][s]) > EPS) {
			rer(j, 0, n) if(j != s) D[i][j] += D[r][j] * D[i][s];
			D[i][s] *= D[r][s];
		}
	}
	return D[m][n];
}
