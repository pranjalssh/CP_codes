//さらに新しいバージョン(inverseを使う)
//Lucas'sに対応(未検証)
//Modが素数であることは確認すること
vector<mint> fact, factinv;
void nCr_computeFactinv(int N) {
	N = min(N, mint::Mod - 1);
	fact.resize(N+1); factinv.resize(N+1);
	fact[0] = 1;
	rer(i, 1, N) fact[i] = fact[i-1] * i;
	factinv[N] = fact[N].inverse();
	for(int i = N; i >= 1; i --) factinv[i-1] = factinv[i] * i;
}
mint nCr(int n, int r) {
	if(n >= mint::Mod)
		return nCr(n % mint::Mod, r % mint::Mod) * nCr(n / mint::Mod, r / mint::Mod);
	return r > n ? 0 : fact[n] * factinv[n-r] * factinv[r];
}
//long long ver. (未検証！)
mint nCr(long long n, long long r) {
	if(n >= mint::Mod)
		return nCr(n % mint::Mod, r % mint::Mod) * nCr(n / mint::Mod, r / mint::Mod);
	return r > n ? 0 : fact[(int)n] * factinv[(int)(n-r)] * factinv[(int)r];
}


//MODが小さいと駄目なので注意！！！！！！！
vector<mint> factinv;
void nCr_smallR_computeFactinv(int N) {
	factinv.resize(N+1);
	mint fact = 1;
	rer(i, 1, N) fact *= i;
	factinv[N] = fact.inverse();
	for(int i = N; i >= 1; i --) factinv[i-1] = factinv[i] * i;
}
mint nCr_smallR(int n, int r) {
	mint x = 1;
	rep(i, r)
		x *= n-i;
	x *= factinv[r];
	return x;
}
//long long ver.
mint nCr_smallR(long long n, int r) {
	mint x = 1;
	rep(i, r)
		x *= n-i;
	x *= factinv[r];
	return x;
}


//下から順に計算する
//MODが小さいと駄目なので注意！！！！！！！
	mint c = 1;
	rep(i, n) {
		mint comb = c * factinv[i];	//C(k,i) = C(k,k-i). product[k-i+1..k] / product[1..i]
		c *= k - i;
	}
	
	mint c = 1;
	rep(i, n) {
		mint comb = c * factinv[i];	//C(k+i-1,i) = C(k+i-1,k-1)
		c *= k + i;
	}

//カタラン数
mint catalan_number(int n) {
	return nCr(2*n, n) - nCr(2*n, n-1);
}

//mod 2。 未検証！
bool nCrMod2(ll n, ll r) {
	return (~n & r) == 0;
}

//新しいバージョン(ModIntを使う)
const int MOD = "REQUIRED!";
typedef ModInt<MOD> mint;

const int nCr_MaxR = "REQUIRED!";
mint inv[nCr_MaxR+2], fact[nCr_MaxR+1], factinv[nCr_MaxR+1];

//必ず最初に呼ぶこと！
void nCr_computeFactinv() {
    inv[1] = 1;
    for(int i=2;i<=nCr_MaxR+1;i++) inv[i] = inv[MOD%i] * (MOD - MOD/i);
	mint x = 1, y = 1;
	rep(i, nCr_MaxR+1) {
		fact[i] = x;
		factinv[i] = y;
		x *= i+1;
		y *= inv[i+1];
	}
}

mint nCr(int n, int r) {
	if(r > n) return 0;
	return fact[n] * factinv[n-r] * factinv[r];
}

//MOD is prime, MaxNR < MOD (じゃないと逆元が取れない)
//nCr_computeFactinv: O(MaxNR log MOD)時間, Θ(MaxNR)空間
//nCr: Θ(1)時間
//※まったくVerifyしてない

const int nCr_MaxNR = "入力！";	//nCr_MaxNR < MOD
int fact[nCr_MaxNR+1], factinv[nCr_MaxNR+1];

ll inverse(ll a) {
	ll b = MOD, u = 1, v = 0;
	while(b) {
		ll t = a / b;
		a -= t * b; swap(a, b);
		u -= t * v; swap(u, v);
	}
	return (u % MOD + MOD) % MOD;
}

void nCr_computeFactinv() {
	ll x = 1;
	rep(i, nCr_MaxNR+1) {
		fact[i] = x;
		factinv[i] = inverse(x);
		(x *= i+1) %= MOD;
	}
}

int nCr(int n, int r) {	//n < nCr_MaxNR, r < nCr_MaxNR
	if(r > n) return 0;
	return int((ll)fact[n] * factinv[n-r] % MOD * factinv[r] % MOD);
}


//MOD is prime, n < MOD, r < MOD (じゃないと逆元が取れない)
//nCr_computeFactinv: O(MaxR log MOD)時間, Θ(MaxR)空間
//nCr: Θ(r)時間

const int nCr_MaxR = "入力！";	//nCr_MaxR < MOD
int factinv[nCr_MaxR+1];

ll inverse(ll a) {
	ll b = MOD, u = 1, v = 0;
	while(b) {
		ll t = a / b;
		a -= t * b; swap(a, b);
		u -= t * v; swap(u, v);
	}
	return (u % MOD + MOD) % MOD;
}

void nCr_computeFactinv() {
	ll x = 1;
	rep(i, nCr_MaxR+1) {
		factinv[i] = inverse(x);
		(x *= i+1) %= MOD;
	}
}

int nCr(int n, int r) {	//n < MOD, r <= nCr_MaxR
	ll x = 1;
	rep(i, r)
		(x *= n-i) %= MOD;
	(x *= factinv[r]) %= MOD;
	return x;
}



//普通のDP
	static const int Combination_N = 1000;
	static mint C[Combination_N+1][Combination_N+1];
	for(int i = 0; i <= Combination_N; i ++) {
		C[i][0] = 1;
		for(int j = 1; j <= i; j ++)
			C[i][j] = C[i-1][j-1] + C[i-1][j];
	}
	