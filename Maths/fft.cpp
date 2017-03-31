typedef long double Num;
const Num PI = 3.141592653589793238462643383279L;
typedef complex<Num> Complex;

void fft_main(int n, Num theta, Complex a[]) {
	for(int m = n; m >= 2; m >>= 1) {
		int mh = m >> 1;
		Complex thetaI = Complex(0, theta);
		for (int i = 0; i < mh; ++i) {
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
	for(int j = 1; j < n - 1; ++j) {
		for(int k = n >> 1; k > (i ^= k); k >>= 1) ;
		if(j < i) swap(a[i], a[j]);
	}
}
void fft(int n, Complex a[], bool rev) {
	if (rev){
		fft_main(n, -2 * PI / n, a);
		for (int i = 0; i < n; ++i) a[i] /= n;
	}
	else
		fft_main(n, 2 * PI / n, a); 	
}
void convolution(vector<Complex> &v, vector<Complex> &w) {
	int n = 1, vwn = v.size() + w.size() - 1;
	while(n < vwn) n <<= 1;
	v.resize(n), w.resize(n);
	fft(n, &v[0], 0);
	fft(n, &w[0], 0);
	for (int i = 0; i < n; ++i) v[i] *= w[i];
	fft(n, &v[0], 1);
}

void fft2(vector<vector<Complex>> & a, bool rev) {
	int n = a.size();
	int m = a[0].size();
	for (int i = 0; i < n; ++i) {
		fft(m, &a[i][0], rev);
	}
	for (int i = 0; i < m; ++i) {
		vector<Complex> t;
		for (int j = 0; j < n; ++j) t.push_back(a[j][i]);
		fft(n, &t[0], rev);
		for (int j = 0; j < n; ++j) a[j][i] = t[j];
	}
}
void convolution2(vector<vector<Complex>> &v, vector<vector<Complex>> &w) {
	int n = 1, vwn = v.size() + w.size() - 1;
	while(n < vwn) n <<= 1;
	v.resize(n), w.resize(n);
	int m = 1, vwm = v[0].size() + w[0].size() - 1;
	while(m < vwm) m <<= 1;
	for (auto& it : v) it.resize(m);
	for (auto& it : w) it.resize(m);
	fft2(v, 0);
	fft2(w, 0);
	for (int i = 0; i < n; ++i) for (int j = 0; j < m; ++j) v[i][j] *= w[i][j];
	fft2(v, 1);
}




const int MOD = 100003;
vector<int> calc_dfs(const vector<int> &A, int l, int r) {
	if(r - l == 1) {
		vector<int> res(2);
		res[0] = 1;
		res[1] = A[l];
		return res;
	}
	int mid = (l + r) / 2;
	vector<int> L = calc_dfs(A, l, mid), R = calc_dfs(A, mid, r);
	vector<Complex> Lc(all(L)), Rc(all(R));
	convolution(Lc, Rc);
	int n = L.size() + R.size() - 1;
	vector<int> res(n);
	rep(i, n) res[i] = (long long)(Lc[i].real() + .5) % MOD;
	return res;
}