//グローバルな実行時に与えられる1つのMod上での計算
//Modはどんな計算もする前に、最初に代入すること！
struct GModInt {
	static int Mod;
	int x;
	GModInt(): x(0) { }
	GModInt(signed sig) { if((x = sig % Mod + Mod) >= Mod) x -= Mod; }
	GModInt(signed long long sig) { if((x = sig % Mod + Mod) >= Mod) x -= Mod; }
	int get() const { return x; }
	
	GModInt &operator+=(GModInt that) { if((x += that.x) >= Mod) x -= Mod; return *this; }
	GModInt &operator-=(GModInt that) { if((x += Mod - that.x) >= Mod) x -= Mod; return *this; }
	GModInt &operator*=(GModInt that) { x = (unsigned long long)x * that.x % Mod; return *this; }
	GModInt &operator/=(GModInt that) { return *this *= that.inverse(); }
	
	GModInt operator+(GModInt that) const { return GModInt(*this) += that; }
	GModInt operator-(GModInt that) const { return GModInt(*this) -= that; }
	GModInt operator*(GModInt that) const { return GModInt(*this) *= that; }
	GModInt operator/(GModInt that) const { return GModInt(*this) /= that; }
	
	//Modが素数であるとか確認すること！
	GModInt inverse() const {
		long long a = x, b = Mod, u = 1, v = 0;
		while(b) {
			long long t = a / b;
			a -= t * b; std::swap(a, b);
			u -= t * v; std::swap(u, v);
		}
		return GModInt(u);
	}
	
	bool operator==(GModInt that) const { return x == that.x; }
	bool operator!=(GModInt that) const { return x != that.x; }
	GModInt operator-() const { GModInt t; t.x = x == 0 ? 0 : Mod - x; return t; }
};
int GModInt::Mod = 0;


typedef GModInt mint;
mint operator^(mint a, unsigned long long k) {
	mint r = 1;
	while(k) {
		if(k & 1) r *= a;
		a *= a;
		k >>= 1;
	}
	return r;
}
