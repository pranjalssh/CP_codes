/*
	！！initを最初に呼ぶこと！！
	FFTを整数剰余環でやるもの
	MODに対して、(ω^n = 1 (mod MOD))となるようなωが無ければいけない
	それは計算するのは難しい(無い場合も多い?)から、
		小さいMODが与えられるなら全探索するしかない。
	Omegaは2^kに対してのものを求めたら、それを2乗してけば配列が得られる
	畳み込み(convolution)は、それぞれをFFTした後inner-productしたものを逆FFTすればいい。
	相互相関関数は、片方を反転させればいい。2次元の場合は両方の次元とも反転させる
		1次元は畳み込みで、もう1次元は相互相関関数のようなものもできるかな？
*/

/*
	ω^n ≡ 1 (mod P) (ω, n, P)のリスト
	(31, 2^23, 998244353)	//2^30未満で大きめの素数
	(183, 2^23, 2130706433U)	//2^31未満で大きめの素数 ※旧ver.はこの素数でもオーバーフローしないことは確認したが、新ver.ではだめ
	(848723745, 2^20, 1004535809) 	//自分で作ってみた
	(375, 2^78, 862568572295037916152856577)	//P＞2^89。"c++/find_omega_gmp.cpp"で生成した。"c++/cumsum_test_gen.cpp"で使ってる
*/

template<int MOD>
struct ModInt {
	static const int Mod = MOD;
	int x;
	ModInt(): x(0) { }
	ModInt(signed sig) { if((x = sig % MOD + MOD) >= MOD) x -= MOD; }
	ModInt(signed long long sig) { if((x = sig % MOD + MOD) >= MOD) x -= MOD; }
	int get() const { return x; }
	
	ModInt &operator+=(ModInt that) { if((x += that.x) >= MOD) x -= MOD; return *this; }
	ModInt &operator-=(ModInt that) { if((x += MOD - that.x) >= MOD) x -= MOD; return *this; }
	ModInt &operator*=(ModInt that) { x = (unsigned long long)x * that.x % MOD; return *this; }
	ModInt &operator/=(ModInt that) { return *this *= that.inverse(); }
	
	ModInt operator+(ModInt that) const { return ModInt(*this) += that; }
	ModInt operator-(ModInt that) const { return ModInt(*this) -= that; }
	ModInt operator*(ModInt that) const { return ModInt(*this) *= that; }
	ModInt operator/(ModInt that) const { return ModInt(*this) /= that; }
	
	ModInt inverse() const {
		long long a = x, b = MOD, u = 1, v = 0;
		while(b) {
			long long t = a / b;
			a -= t * b; std::swap(a, b);
			u -= t * v; std::swap(u, v);
		}
		return ModInt(u);
	}
};

typedef ModInt<998244353> fft_mint;
const int OmegaMax = 23;
fft_mint OmegaPrim = 31;
fft_mint Omega[OmegaMax+1];
//必ず呼ぶこと
void fft_init() {
	fft_mint x = OmegaPrim;
	for(int i = OmegaMax; i >= 0; i --) {
		Omega[i] = x;
		x *= x;
	}
}
//aを破壊する
void fft_main(int logn, const bool inv, fft_mint a[]) {
	int n = 1 << logn;
	fft_mint ww = Omega[logn];
	if(inv) ww = ww.inverse();
	for(int m = n, mi = 0; m >= 2; m >>= 1, mi ++) {
		int mh = m >> 1;
		fft_mint w = 1;
		rep(i, mh) {
			for(int j = i; j < n; j += m) {
				int k = j + mh;
				fft_mint x = a[j] - a[k];
				a[j] += a[k];
				a[k] = w * x;
			}
			w *= ww;
		}
		ww *= ww;
	}
	int i = 0;
	reu(j, 1, n-1) {
		for(int k = n >> 1; k > (i ^= k); k >>= 1) ;
		if(j < i) swap(a[i], a[j]);
	}
}

void fft(int logn, fft_mint a[]) { fft_main(logn, false, a); }
void inverse_fft(int logn, fft_mint a[]) {
	fft_main(logn, true, a);
	int n = 1 << logn;
	fft_mint invn = fft_mint(n).inverse();
	rep(i, n) a[i] *= invn;
}

//v, wを破壊し、vに結果を返す
//v, wのサイズは 2^logn, lognはceil(log_2(size(v) + size(w)))必要
void convolution(fft_mint v[], fft_mint w[], int logn) {
	fft(logn, v); fft(logn, w);
	rep(i, 1<<logn) v[i] *= w[i];
	inverse_fft(logn, v);
}


//====================================================================
//旧バージョン


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
const Num MOD = 998244353;
const int OmegaMax = 23;
Num OmegaPrim = 31;	//OmegaPrim^(2^OmegaN) ≡ 1 (mod MOD)
Num Omega[OmegaMax+1];
const int MaxNM = 1<<11;	//2D用

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
				Num x = (a[j] < a[k] ? MOD - a[k] + a[j] : a[j] - a[k]);
				a[j] = ADD(a[j], a[k]);
				a[k] = MUL(w, x);
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

void fft2D(int logn, int logm, Num a[]) {
	int n = 1 << logn, m = 1 << logm;
	rep(i, n) fft(logm, a + (i<<logm));
	rep(j, m) {
		static Num buf[MaxNM];
		rep(i, n) buf[i] = a[i<<logm | j];
		fft(logn, buf);
		rep(i, n) a[i<<logm | j] = buf[i];
	}
}
void inverse_fft2D(int logn, int logm, Num a[]) {
	int n = 1 << logn, m = 1 << logm;
	rep(j, m) {
		static Num buf[MaxNM];
		rep(i, n) buf[i] = a[i<<logm | j];
		inverse_fft(logn, buf);
		rep(i, n) a[i<<logm | j] = buf[i];
	}
	rep(i, n) inverse_fft(logm, a + (i<<logm));
}
};


//JAG春コン2013 F "Point Distance" [http://jag2013spring.contest.atcoder.jp/submissions/78135]
//2次元相互相関
int main() {
	int N, N1;
	scanf("%d", &N1);
	static int C[1024][1024];
	rep(i, N1) rep(j, N1) scanf("%d", &C[i][j]);
	int logN = 0;
	N = 1; while(N < 2*N1) N <<= 1, logN ++;
	int N2 = N*N;
	FMT::init();

	vector<Num> v(N2), w(N2);
	rep(i, N1) rep(j, N1)
		w[((N1-1-i) << logN) + (N1-1-j)] = v[i << logN | j] = C[i][j];
	FMT::fft2D(logN, logN, &v[0]);
	FMT::fft2D(logN, logN, &w[0]);
	rep(i, N2) v[i] = FMT::MUL(v[i], w[i]);
	FMT::inverse_fft2D(logN, logN, &v[0]);
	static ll cnt[1<<21];
	rer(dy, -N1+1, N1-1) rer(dx, -N1+1, N1-1) {
		ll x = 0;
		if(dy == 0 && dx == 0)
			rep(i, N1) rep(j, N1) x += C[i][j] * (C[i][j]-1);
		else x = v[(dy+N1-1)<<logN | (dx+N1-1)];
		if(x) {
//			cout << dy << ", " << dx << ": " << x << endl;
			cnt[dy*dy + dx*dx] += x;
		}
	}
	double distsum = 0, cntsum = 0;
	rep(i, N1*N1*2) {
		distsum += cnt[i] * sqrt((double)i);
		cntsum += cnt[i];
	}
	printf("%.10f\n", distsum / cntsum);
	int cntcnt = 0;
	rep(i, N1*N1*2) if(cnt[i]) {
		if(cntcnt < 10000)
			cout << i << " " << cnt[i] / 2 << endl;
		cntcnt ++;
	}
	return 0;
}



//SPOJ MUL [http://www.spoj.com/files/src/9154672/]
int main() {
	FMT::init_wtable(16);
	int n;
	scanf("%d", &n);
	rep(ii, n) {
		static char s[300010], t[300010];
		scanf("%s%s", s, t);
		int sl = strlen(s), tl = strlen(t);
		static const int D = 3;
		static const int POWD = 1000;
		static vector<Num> sv, tv;
		sv.resize((sl + D - 1) / D), tv.resize((tl + D - 1) / D);
		rep(i, sv.size()) {
			ll x = 0;
			for(int j = min(sl, (i+1)*D)-1; j >= i*D; j --)
				x = x * 10 + (s[sl - 1 - j] - '0');
			sv[i] = x;
		}
		rep(i, tv.size()) {
			ll x = 0;
			for(int j = min(tl, (i+1)*D)-1; j >= i*D; j --)
				x = x * 10 + (t[tl - 1 - j] - '0');
			tv[i] = x;
		}
		FMT::convolution(sv, tv);
		int rl = (sl + tl + D - 1) / D;
		static vector<int> r;
		r.assign(rl, 0);
		rep(i, min(r.size(), sv.size())) r[i] = sv[i];
		int carry = 0;
		rep(i, rl) {
			r[i] += carry;
			carry = r[i] / POWD;
			r[i] %= POWD;
		}
		assert(carry == 0);
		static char o[600010];
		rep(i, rl) {
			ll x = r[i];
			rep(j, D) o[i*D+j] = '0' + x % 10, x /= 10;
		}
		int ol = rl * D;
		while(o[ol-1] == '0' && 1 < ol) ol --;
		rep(i, ol) putchar(o[ol - 1 - i]);
		puts("");
	}
	return 0;
}

//ω^(a 2^k) mod M ＝ 1
//ω^i mod M ≠ 1 (0 ＜ i ＜ a 2^k)
//ω^iの周期が(0, a 2^k)
//周期は必ず((M-1)/x)
//ああ、なるほど。だから(an+1)のでやっていたのね
//というかサイズは2の累乗である必要はないのだから、
//	MODを決めてからサイズを決めればいいのでは。
//つまりサイズが2の累乗でないFFTの実装が求められる、と…
//いや、でも2の累乗じゃないとO(Σp)かかるんだっけ？そしたら難しいな…
//まあ適当な(a 2^k+1)の素数で割らせればいいでしょ

//(a 2^k+1)のMODを決めてから見つけるもの
int main() {
	primeFactorizes();
	vi primes;
	rer(i, 2, PRIMEFACTORIZES_MAXN)
		if(primeFactor[i] == i)
			primes.push_back(i);
	int MOD = 1004535809;	//(MOD-1)は2^jminで割り切れる必要がある
	int jmin = 20;
	int ps[] = {1,2,479,2*479};	//(MOD-1)を2^jminで割ったものの約数
	rep(i, MOD) {
		int x = i;
		rep(kt, sizeof(ps)/sizeof(*ps)) {
			int k = ps[kt];
			int y = powmod(i, k, MOD);
			rep(j, 21) {
				if(y == 1) {
					if(j < jmin) goto aa;
					printf("\n%d^(%d 2^%d) ≡ %d (mod %d); ω = %d\n",
						i, k, j, powmod(i, (1ULL<<j)*k, MOD), MOD,
						powmod(i, k, MOD));
				}
				y = ((ll)y * y) % MOD;
			}
			aa:;
		}
	}
	return 0;
}


/*
	原始n乗根を見つけるやつ
	※全てを列挙するわけではない。小さなωの、指数が2^nの形のものだけを探してる


unsigned powmod(unsigned long long a, unsigned long long k, const unsigned MOD) {
	unsigned long long r = MOD == 1 ? 0 : 1;
	while(k) {
		if(k & 1)
			(r *= a) %= MOD;
		(a *= a) %= MOD;
		k >>= 1;
	}
	return (unsigned)r;
}
bool suspect(int a, int s, int d, int n) {
  ll x = powmod(a, d, n);
  if (x == 1) return true;
  for (int r = 0; r < s; ++r) {
    if (x == n - 1) return true;
    x = x * x % n;
  }
  return false;
}
bool isPrime(int n) {
  if (n <= 1 || (n > 2 && n % 2 == 0)) return false;
  int test[] = {2,7,61,-1};
  int d = n - 1, s = 0;
  while (d % 2 == 0) ++s, d /= 2;
  for (int i = 0; test[i] < n && test[i] != -1; ++i)
    if (!suspect(test[i], s, d, n)) return false;
  return true;
}

	static const int MODbase = 1 << 22;
	for(int MODfact = 1; MODfact < (1 << 30) / MODbase; MODfact ++) {
		int MOD = MODbase * MODfact + 1;
		if(!isPrime(MOD)) continue;
		reu(i, 2, 1000) {
			ll x = i;
			rep(j, 29) {
				if(x == 1) {
					printf("%d^(2^%d) = 1 (mod %d)\n", i, j, MOD);
					goto brk;
				}
				(x *= x) %= MOD;
			}
		}
		brk:;
	}

*/
