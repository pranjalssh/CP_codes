//#include "~include_templates.h"

//簡易バージョン。MODを取る時はmintにしよう
//mintだと結構(5倍以上)遅いので注意！
typedef unsigned long long Num;

struct Matrix {
	vector<vector<Num> > v, w;
	Matrix() {}
	Matrix(int n, int m): v(n, vector<Num>(m)) { }
	inline int height() const { return (int)v.size(); }
	inline int width() const { return (int)v[0].size(); }
	inline Num& at(int i, int j) { return v[i][j]; }
	inline const Num& at(int i, int j) const { return v[i][j]; }
	static Matrix identity(int n) {
		Matrix A(n, n);
		rep(i, n) A.at(i, i) = 1;
		return A;
	}
	inline static Matrix identity(const Matrix& A) { return identity(A.height()); }
	Matrix& operator*=(const Matrix& B) {
		int n = height(), m = B.width(), p = B.height();
		assert(p == width());
		w.assign(n, vector<Num>(m, 0));
		rep(i, n) rep(j, m) {
			Num x = 0;
			rep(k, p) x += at(i, k) * B.at(k, j);
			w[i][j] = x;
		}
		v.swap(w);
		return *this;
	}
};
Matrix operator^(const Matrix& t, ll k) {
	Matrix A = t, B = Matrix::identity(t);
	while(k) {
		if(k & 1) B *= A;
		A *= A;
		k >>= 1;
	}
	return B;
}

//配列で高速化ver.
struct Matrix {
	typedef mint Num;
	static const int MaxN = 50;
	int hei, wid;
	Num v[MaxN][MaxN], w[MaxN][MaxN];
	Matrix() {}
	Matrix(int n, int m): hei(n), wid(m) { mset(v, 0); }
	inline int height() const { return hei; }
	inline int width() const { return wid; }
	inline Num& at(int i, int j) { return v[i][j]; }
	inline const Num& at(int i, int j) const { return v[i][j]; }
	static Matrix identity(int n) {
		Matrix A(n, n);
		rep(i, n) A.at(i, i) = 1;
		return A;
	}
	inline static Matrix identity(const Matrix& A) { return identity(A.height()); }
	Matrix& operator*=(const Matrix& B) {
		int n = height(), m = B.width(), p = B.height();
		assert(p == width());
		const unsigned (*b)[MaxN] = reinterpret_cast<const unsigned (*)[MaxN]>(B.v);
		rep(i, n) {
			const unsigned *ai = reinterpret_cast<const unsigned*>(v[i]);
			rep(j, m) {
				unsigned long long x = 0;
				rep(k, p) x += (unsigned long long)ai[k] * b[k][j];
				w[i][j].x = x % mint::Mod;
			}
		}
		memcpy(v, w, sizeof(v));
		return *this;
	}
};

/*
	！MODを変えること！MODが1000000007より大きいならさらに掛け算のところを書き換えること！

	行列が"Toeplitz"であるとはサイズnの正方行列であって、
	A[i][j]が(i-j+n)の関数で表されるものを言う。
	行列がToeplitzであるだけでは乗算の時間計算量は変わらない(ただし空間計算量は変わる)
	また、Toeplitz同士の乗算でもToeplitzになるとは限らない。
	Toeplitzであるなら"Circulant"や"Triangular"であるかを考えてみよう。

	行列が"Circulant"(Circular, 巡回)であるとは、サイズnの正方行列であって、
	A[i][j]が((i-j) mod n)の関数で表されるものを言う。
	任意のkに対してA[i][j] = A[(i+k)%n][(j+k)%n]であることと同値。
	典型的には、円状のボード上の相対的な動きはそうなる。
	乗算・加算のもとで閉じている。
	CirculantならばToeplitzである。
	CirculantならばSymmetricである。
	
	行列が"Upper Triangular Toeplitz"であるとは、サイズnの正方行列であって、
	Toeplitzであって、(i > j -> A[i][j] = 0)となるものを言う。
	ある順序の中での動きはTriangularになる。
	乗算・加算のもとで閉じている。
	"Lower ..."から"Upper ..."の変換は単純に上下を反転させれば良い。

	"Block Matrix"はサイズの合った行列を左右に並べて作った行列。
	各ブロックが乗算と加算のもとで閉じているなら、乗算・加算のもとで閉じている。
	つまり"Circulant"や"Upper Triangular Toeplitz"を並べた"Block Matrix"も「使える」
	ブロックの作り方によって見てわからない形になることがあるので、
	ブロックの作り方((i+h*j)か(j*w+i)か)を二通りためしてみるべき。
	"Block Matrix"で数え上げをしたい時は対角行列の形にしてできる。
	[i][j]を数え上げたいなら[i][s][j][j]みたいな感じで？(下のコード参照)
*/

//#undef assert
//#define assert(_)

typedef unsigned long long Num;
#define MOD 1000000007ULL	//！！！！！！！MODが大きい場合、"*="を書き換えること！

struct Matrix {
	vector<vector<Num> > v, w;
	Matrix() {}
	Matrix(int n, int m): v(n, vector<Num>(m)) { assert(n > 0 && m > 0); }
	inline int height() const { return (int)v.size(); }
	inline int width() const { return (int)v[0].size(); }
	inline Num& at(int i, int j) { assert(0 <= i && i < height() && 0 <= j && j < width()); return v[i][j]; }
	inline const Num& at(int i, int j) const { assert(0 <= i && i < height() && 0 <= j && j < width()); return v[i][j]; }
	static Matrix identity(int n) {
		Matrix A(n, n);
		rep(i, n) A.at(i, i) = 1;
		return A;
	}
	inline static Matrix identity(const Matrix& A) {
		assert(A.height() == A.width()); return identity(A.height()); }
	Matrix& operator*=(const Matrix& B) {
		int n = height(), m = B.width(), p = B.height();
		assert(p == width());
		w.resize(n, vector<Num>(m, 0));
		rep(i, n) rep(j, m) {
			//MOD = 1000000007の場合、1000000012000000036となり、
			//2^64にオーバーフローするのは2^64-1-1000000006^2くらいで、それは最上位4bitが立っているから
			//！MODが違う場合、書き換えること！
			Num x = 0;
			rep(k, p) {
				x += at(i, k) * B.at(k, j);
				if((x >> 60) == 0xf) x %= MOD;
			}
			x %= MOD;
			w[i][j] = x;
		}
		v.swap(w);
		return *this;
	}
	Matrix& operator+=(const Matrix& B) {
		int n = height(), m = width();
		assert(n == B.height() && m == B.width());
		rep(i, n) rep(j, m) {
			at(i, j) += B.at(i, j);
			if(at(i, j) >= MOD) at(i, j) -= MOD;
		}
		return *this;
	}
	void undomult() { v.swap(w); }
};
ostream& operator<<(ostream& o, const Matrix& A) {
	int n = A.height(), m = A.width();
	rep(i, n) {
		o << "["; rep(j, m) o << A.at(i, j) << (j+1 == m ? "]\n" : ",");
	}
	return o;
}

/* ジェネリックなべき乗算 */
template<typename Mat>
Mat operator^(const Mat& t, ll k) {
	Mat A = t, B = Mat::identity(t);
	while(k) {
		if(k & 1) {
			B *= A;
		}
		A *= A;
		k >>= 1;
	}
	return B;
}

struct CirculantMatrix {
	vector<Num> v, w;
	CirculantMatrix() {}
	CirculantMatrix(int n, int m): v(n, 0) { (void)m; assert(n == m); assert(n > 0); }
	CirculantMatrix(const Matrix& A) {
		int n = A.height();
		assert(n > 0);
		assert(n == A.width());
		rep(i, n) rep(j, n) assert(A.at(i, j) == A.at((i+1)%n, (j+1)%n));
		v.assign(n, 0);
		rep(i, n) v[i] = A.at(i, 0);
	}
	inline int size() const { return (int)v.size(); }
	inline int height() const { return size(); }
	inline int width() const { return size(); }
	inline Num& at(int i) { assert(0 <= i && i < size()); return v[i]; }
	inline const Num& at(int i) const { assert(0 <= i && i < size()); return v[i]; }
	inline Num& at(int i, int j) { assert(0 <= i && i < size() && 0 <= j && j < size()); return v[(i - j + size()) % size()]; }
	inline const Num& at(int i, int j) const { assert(0 <= i && i < size() && 0 <= j && j < size()); return v[(i - j + size()) % size()]; }

	inline static CirculantMatrix identity(int n) {
		CirculantMatrix A(n, n);
		A.at(0) = 1;
		return A;
	}
	inline static CirculantMatrix identity(const CirculantMatrix& A) { return identity(A.size()); }
	CirculantMatrix& operator*=(const CirculantMatrix& B) {
		int n = size(); assert(n == B.size()); w.resize(n);
		Num *vv = &v[0], *ww = &w[0]; const Num *zz = &B.v[0];
		rep(j, n) {
			Num x = 0;
			rer(k, 0, j) {
				x += vv[k] * zz[j-k];
				if((x >> 60) == 0xf) x %= MOD;
				//MOD = 1000000007の場合、1000000012000000036となり、
				//2^64にオーバーフローするのは2^64-1-1000000006^2くらいで、それは最上位4bitが立っているから
				//！MODが違う場合、ここと下の部分も書き換えること！
			}
			rer(k, j+1, n-1) {
				x += vv[k] * zz[j-k+n];
				if((x >> 60) == 0xf) x %= MOD;
			}
			x %= MOD;
			ww[j] = x;
		}
		v.swap(w);
		return *this;
	}
	CirculantMatrix& operator+=(const CirculantMatrix& B) {
		int n = size();
		assert(n == B.size());
		rep(i, n) {
			v[i] += B.v[i];
			if(v[i] >= MOD) v[i] -= MOD;
		}
		return *this;
	}
	void undomult() { v.swap(w); }
};

ostream& operator<<(ostream& o, const CirculantMatrix& A) {
	int n = A.size();
	//普通の行列のように表示する
//	rep(i, n) {
//		o << "["; rep(j, n) o << A[(i - j + n) % n] << (j+1 == n ? "]\n" : ",");
//	}
	o << "["; rep(j, n) o << A.v[j] << (j+1 == n ? "]" : ",");
	return o;
}

struct UTToeplitzMatrix {
	vector<Num> v, w;	//A[0][i]ێC[W
	static const Num Zero = 0;
	UTToeplitzMatrix() {}
	UTToeplitzMatrix(int n, int m): v(n, 0) { (void)m; assert(n == m); assert(n > 0); }
	inline int size() const { return (int)v.size(); }
	inline int height() const { return size(); }
	inline int width() const { return size(); }
	inline Num& at(int i) { assert(0 <= i && i < size()); return v[i]; }
	inline const Num& at(int i) const { assert(0 <= i && i < size()); return v[i]; }
	inline Num& at(int i, int j) {
		assert(0 <= i && i < size() && 0 <= j && j < size());
		assert(i <= j);	//0̕ύX邱Ƃ͂łȂ
		return v[j - i];
	}
	inline const Num& at(int i, int j) const {
		assert(0 <= i && i < size() && 0 <= j && j < size());
		if(i > j) return Zero;	//0̕
		return v[j - i];
	}
	static UTToeplitzMatrix identity(int n) {
		UTToeplitzMatrix A(n, n);
		A.at(0) = 1;
		return A;
	}
	inline static UTToeplitzMatrix identity(const UTToeplitzMatrix& A) { return identity(A.size()); }
	UTToeplitzMatrix& operator*=(const UTToeplitzMatrix& B) {
		int n = size(); assert(n == B.size()); w.resize(n);
		Num *vv = &v[0], *ww = &w[0]; const Num *zz = &B.v[0];
		rep(i, n) {
			Num x = 0;
			rer(j, 0, i) {
				x += v[j] * B.v[i - j];
				if((x >> 60) == 0xf) x %= MOD;
			}
			x %= MOD;
			w[i] = x;
		}
		v.swap(w);
		return *this;
	}
	UTToeplitzMatrix& operator+=(const UTToeplitzMatrix& B) {
		int n = size();
		assert(n == B.size());
		rep(i, n) {
			v[i] += B.v[i];
			if(v[i] >= MOD) v[i] -= MOD;
		}
		return *this;
	}
	void undomult() { v.swap(w); }
};

struct BlockMatrix {
	typedef UTToeplitzMatrix Block;
	vector<vector<Block> > blocks, blocks2;
	vector<int> heights, widths;
	BlockMatrix(const vector<int>& hs, const vector<int>& ws)
		: heights(hs), widths(ws) {
		int n = heights.size(), m = widths.size();
		assert(n > 0 && m > 0);
		blocks.resize(n);
		rep(i, n) rep(j, m)
			blocks[i].push_back(Block(heights[i], widths[j]));
	}
	inline int height() const { return heights.size(); }
	inline int width() const { return widths.size(); }
	inline int blockHeight(int i) const { assert(0 <= i && i < height()); return blocks[i][0].height(); }
	inline int blockWidth(int j) const { assert(0 <= j && j < width()); return blocks[0][j].width(); }
	inline Block& at(int i, int j) { assert(0 <= i && i < height() && 0 <= j && j < width()); return blocks[i][j]; }
	inline const Block& at(int i, int j) const { assert(0 <= i && i < height() && 0 <= j && j < width()); return blocks[i][j]; }
	static BlockMatrix identity(int n, int m) {
		vector<int> v;
		rep(i, n) v.push_back(m);
		BlockMatrix A(v, v);
		rep(i, n) A.at(i, i) = Block::identity(m);
		return A;
	}
	inline static BlockMatrix identity(const BlockMatrix& A) {
		int n = A.height(); assert(n == A.width());
		int m = A.heights[0];
		assert(count(A.heights.begin(), A.heights.end(), m) == n);
		assert(count(A.widths.begin(), A.widths.end(), m) == n);
		return identity(n, m);
	}
	BlockMatrix& operator*=(const BlockMatrix& B) {
		int n = height(), m = B.width(), p = B.height();
		assert(p == width());
		blocks2.resize(n, vector<Block>(m, Block()));
		rep(i, n) rep(j, m) {
			Block x = Block(blockHeight(i), B.blockWidth(j));
			rep(k, p) {
				at(i, k) *= B.at(k, j);
				x += at(i, k);
				at(i, k).undomult();
			}
			blocks2[i][j] = x;
		}
		blocks.swap(blocks2);
		return *this;
	}
};
ostream& operator<<(ostream& o, const BlockMatrix& A) {
	int n = A.height(), m = A.width();
	rep(i, n) rep(i2, A.blockHeight(i)){
		o << "[";
		rep(j, m) rep(j2, A.blockWidth(j))
			o << A.at(i, j).at(i2, j2) << (j+1 == m && j2+1 == A.blockWidth(j) ? "]\n" : ",");
	}
	return o;
}

/*
	"Autumn Fest 2012" J "Ninja of Train"
	Problem: http://autumn_fest.contest.atcoder.jp/tasks/autumn_fest_10
	Submittion: http://autumn_fest.contest.atcoder.jp/submissions/66536
*/
int main() {
	int h, t;
	scanf("%d%d", &h, &t);
	int s = (int)sqrt((double)h)+1;
	BlockMatrix V(vi(1, h), vi(s+1, h));
	V.at(0, 0) = BlockMatrix::Block::identity(h);
	vector<int> hs; rep(i, s+1) hs.push_back(h);
	BlockMatrix A(hs, hs);
	rep(i, h) rep(j, s) {
		if(j == 0) {
			A.at(0, s).at(i, i) = 1;
			rer(k, 1, h*2) {
				int t = (int)sqrt((double)k);
				int u = i+k-t;
				if(!(0 <= u && u < h)) continue;
				A.at(0, t-1).at(i, u) = 1;
			}
		}else {
			A.at(j, j-1).at(i, i) = 1;
		}
	}

	A.at(s, s) = BlockMatrix::Block::identity(h);
	BlockMatrix B = A ^ t+1;
	BlockMatrix C = V; C *= B;
	ll r = 0;
	rep(i, h) (r += C.at(0, s).at(i, h-1)) %= MOD;
	cout << r << endl;
	return 0;
}





/*
#################################################################
	三角ToeplitzをFMTでやるやつ
	詳細は!FMT.cppを
*/

unsigned inverse(ll a, const unsigned MOD) {
	ll b = MOD, u = 1, v = 0;
	while(b) {
		ll t = a / b;
		a -= t * b; swap(a, b);
		u -= t * v; swap(u, v);
	}
	return (u % MOD + MOD) % MOD;
}

typedef unsigned int Num;
namespace FMT {
const Num MOD = 1004535809;
const int OmegaMax = 20;
Num OmegaPrim = 848723745;	//OmegaPrim^(2^OmegaN) ≡ 1 (mod MOD)
Num Omega[OmegaMax+1];

inline Num ADD(Num x, Num y) {
	if((x += y) >= MOD) x -= MOD;
	return x;
}
inline Num MUL(Num x, Num y) {
	return (unsigned long long)x * y % MOD;
}
void init() {
	Num x = OmegaPrim;
	for(int i = OmegaMax; i >= 0; i --) {
		Omega[i] = x;
		x = MUL(x, x);
	}
}
//aを破壊する
void fft_main(int logn, const bool inv, Num a[]) {
	int n = 1 << logn;
	Num ww = Omega[logn];
	if(inv) ww = inverse(ww, MOD);
	for(int m = n, mi = 0; m >= 2; m >>= 1, mi ++) {
		int mh = m >> 1;
		Num w = 1;
		rep(i, mh) {
			for(int j = i; j < n; j += m) {
				int k = j + mh;
				Num x = (a[j] < a[k] ? MOD : 0) + a[j] - a[k];
				if((a[j] += a[k]) >= MOD) a[j] -= MOD;
				a[k] = (int)((ll)w * x % MOD);
			}
			w = MUL(w, ww);
		}
		ww = MUL(ww, ww);
	}
	int i = 0;
	reu(j, 1, n-1) {
		for(int k = n >> 1; k > (i ^= k); k >>= 1) ;
		if(j < i) swap(a[i], a[j]);
	}
}

void fft(int logn, Num a[]) { fft_main(logn, false, a); }
void inverse_fft(int logn, Num a[]) {
	fft_main(logn, true, a);
	int n = 1 << logn;
	Num invn = inverse(n, MOD);
	rep(i, n) a[i] = MUL(a[i], invn);
}

//v, wを破壊し、vに結果を返す
//v, wのサイズは 2^logn, lognはceil(log_2(size(v) + size(w)))必要
void convolution(Num v[], Num w[], int logn) {
	fft(logn, v); fft(logn, w);
	rep(i, 1<<logn) v[i] = MUL(v[i], w[i]);
	inverse_fft(logn, v);
}
};

struct LTToeplitz {
	int log2n, size_;
	vector<Num> v;
	static const Num Zero = 0;
	LTToeplitz() {}
	static int calclog2n(int n) {
		int r = 1;
		while((1 << r) < 2 * n) r ++;
		return r;
	}
	LTToeplitz(int n): size_(n), log2n(calclog2n(n)), v(1<<log2n, 0) { }
	inline int size() const { return size_; }
	inline Num& at(int i) { return v[i]; }	//(i, j) -> i - j
	static LTToeplitz identity(int n) {
		LTToeplitz A(n);
		A.at(0) = 1;
		return A;
	}
	LTToeplitz& operator*=(const LTToeplitz& B) {
		assert(size() == B.size());
		static vector<Num> tmp;
		tmp = B.v;
		FMT::convolution(&v[0], &tmp[0], log2n);
		fill(v.begin()+size_, v.begin()+2*size_, 0);
		return *this;
	}
};

LTToeplitz operator^(const LTToeplitz& t, ll k) {
	LTToeplitz A = t, B = LTToeplitz::identity(t.size());
	while(k) {
		if(k & 1) {
			B *= A;
		}
		A *= A;
		k >>= 1;
	}
	return B;
}

ostream& operator<<(ostream& o, const LTToeplitz& A) {
	int n = A.size();
	o << "["; rep(j, n) o << A.v[j] << (j+1 == n ? "]" : ",");
	return o;
}
