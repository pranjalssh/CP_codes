//GCD。マイナスは未対応
template<typename T>T gcd(T x, T y){if(y==0)return x;else return gcd(y,x%y);}

//マイナスの数はabsしとけばいい
template<typename T>T gcd_positive(T x, T y) { return y == 0 ? x : gcd_positive(y,x%y); }
template<typename T>T gcd(T x, T y) { return gcd_positive(abs(x), abs(y)); }

//LCM
//最大x*yになりうるので、オーバーフローに注意
template<typename T>T lcm(T x, T y){ return x/gcd(x,y)*y; }

//拡張ユークリッド
//a*m + b*n = gcd(m, n) = g の解となる整数a,bの組を見つける
//※未検証
template<typename T>
void extendedEuclid(T m, T n, T& a, T& b, T& g) {
	if(n == 0) {
		a = 1; b = 0; g = m;
	}else {
		extendedEuclid(n, m%n, a, b, g);
		swap(a, b);
		b -= a * (m/n);
	}
}
