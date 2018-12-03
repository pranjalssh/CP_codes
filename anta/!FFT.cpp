/*
	値の最大が大きくない((＜2^30)程度)なら、FMTも使える
*/


typedef double Num;	//場合に応じてlong doubleにすること
const Num PI = 3.141592653589793238462643383279L;
typedef complex<Num> Complex;
//nは2のべき乗である必要がある
//aを破壊する
void fft_main(int n, Num theta, Complex a[]) {
	for(int m = n; m >= 2; m >>= 1) {
		int mh = m >> 1;
		Complex thetaI = Complex(0, theta);
		rep(i, mh) {
			Complex w = exp((Num)i*thetaI);
			for(int j = i; j < n; j += m) {
				int k = j + mh;
				Complex x = a[j] - a[k];
				a[j] += a[k];
				a[k] = w * x;
			}
		}
		theta *= 2;
	}
	int i = 0;
	reu(j, 1, n-1) {
		for(int k = n >> 1; k > (i ^= k); k >>= 1) ;
		if(j < i) swap(a[i], a[j]);
	}
}

void fft(int n, Complex a[]) { fft_main(n, 2 * PI / n, a); }
void inverse_fft(int n, Complex a[]) { fft_main(n, -2 * PI / n, a); }

//入力を破壊する
void convolution(vector<Complex> &v, vector<Complex> &w) {
	int n = 1, vwn = v.size() + w.size() - 1;
	while(n < vwn) n <<= 1;
	v.resize(n), w.resize(n);
	fft(n, &v[0]);
	fft(n, &w[0]);
	rep(i, n) v[i] *= w[i];
	inverse_fft(n, &v[0]);
	rep(i, n) v[i] /= n;
}

//http://www.spoj.com/problems/VFMUL/
int main() {
	int n;
	scanf("%d", &n);
	rep(ii, n) {
		static char s[300010], t[300010];
		scanf("%s%s", s, t);
		int sl = strlen(s), tl = strlen(t);
		static const int D = 7;
		static const ll POWD = 10000000LL;
		static vector<Complex> sv, tv;
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
		convolution(sv, tv);
		int rl = (sl + tl + D - 1) / D;
		static vector<ll> r;
		r.assign(rl, 0);
		rep(i, min(r.size(), sv.size())) r[i] = ll(sv[i].real() + .5);
		ll carry = 0;
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