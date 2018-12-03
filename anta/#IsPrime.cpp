//すぱそ

ll modmult(ll x, ll y, ll MOD) {
	ll a = x, r = 0;
	while(y) {
		if(y & 1)
			if((r += a) >= MOD) r -= MOD;
		if((a += a) >= MOD) a -= MOD;
		y >>= 1;
	}
	return r;
}
ll powmodll(ll a, unsigned long long k, const ll MOD) {
	a %= MOD;
	ll r = MOD == 1 ? 0 : 1;
	while(k) {
		if(k & 1)
			r = modmult(r, a, MOD);
		a = modmult(a, a, MOD);
		k >>= 1;
	}
	return (unsigned)r;
}

bool suspect(ll a, int s, ll d, ll n) {
	ll x = powmodll(a, d, n);
	if(x == 1) return true;
	for(int r = 0; r < s; ++r) {
		if(x == n - 1) return true;
		x = modmult(x, x, n);
	}
	return false;
}
// {2,7,61,-1}                 is for n < 4759123141 (= 2^32)
// {2,3,5,7,11,13,17,19,23,-1} is for n < 10^16 (at least)
bool isPrime(ll n) {
	if (n <= 1 || (n > 2 && n % 2 == 0)) return false;
	int test[] = {2,3,5,7,11,13,17,19,23,-1};
	ll d = n - 1; int s = 0;
	while (d % 2 == 0) ++ s, d /= 2;
	for(int i = 0; test[i] < n && test[i] != -1; ++i)
		if(!suspect(test[i], s, d, n)) return false;
	return true;
}
