
inline int add(int a, int b, int m = inf) {a += b; if (a >= m)a -= m; return a;}
inline int mul(int a, int b, int m = inf) {return (int)(((ll)a * (ll)b) % m);}

const int N = 1e5;
int fac[N + 1];
int inv[N + 1];
int ifac[N + 1];

int sv[N + 1];


int expo(int n, ll p, int mod = inf) {
    int ans = 1;
    while (p) {
        if (p & 1) {
            ans = (ans * 1LL * n) % mod;
        }
        n = (n * 1LL * n) % mod;
        p >>= 1;
    }
    return ans;
}
inline int C(int n, int k) {
    if (k > n)return 0;
    int ans = (fac[n] * 1LL * ifac[k]) % inf;
    return (ans * 1LL * ifac[n - k]) % inf;
}
int main()
{
    boost;

    FOR(i, 1, N)sv[i] = i;

    FOR(i, 2, 1000)if (sv[i] == i)for (int j = i * i; j <= N; j += i)sv[j] = i;

    fac[0] = 1; ifac[0] = 1; inv[1] = 1;
    FOR(i, 1, N)fac[i] = (fac[i - 1] * 1LL * i) % inf;
    FOR(i, 2, N)inv[i] = inf - ((inf / i) * 1LL * inv[inf % i]) % inf;
    FOR(i, 1, N)ifac[i] = (ifac[i - 1] * 1LL * inv[i]) % inf;

    cout << expo(2, 100, inf);

    return 0;
}


// if e > phi(m) n^e mod m= (n ^ (phi(m) + e mod phi(m)) mod m
// for (int x = y; x > 0; x = ( y & (x-1) ) )

// for (int i = 1, la; i <= n; i = la + 1) {
//         la = n / (n / i);
//         // [i ... la]
//     }


// tower of powers
pair<int, int> solve(int l, int r, int m)
{
    if (m == 1)return make_pair(0, 1);
    if (l == r + 1)return make_pair(1, 0);
    if (w[l] == 1)return make_pair(1, 0);
    int t = phi(m);
    pair<int, int> cur = solve(l + 1, r, t), res;
    res.first = fp(w[l], cur.first + cur.second * t, m);
    res.second = cur.second | (fp_min(w[l], cur.first, m) == m);
    return res;
}

template<int MOD>
struct ModInt {
    static const int Mod = MOD;
    unsigned x;
    ModInt() : x(0) {}
    ModInt(signed sig) { int sigt = sig % MOD; if (sigt < 0) sigt += MOD; x = sigt; }
    ModInt(signed long long sig) { int sigt = sig % MOD; if (sigt < 0) sigt += MOD; x = sigt; }
    int get() const { return (int)x; }

    ModInt &operator+=(ModInt that) { if ((x += that.x) >= MOD) x -= MOD; return *this; }
    ModInt &operator-=(ModInt that) { if ((x += MOD - that.x) >= MOD) x -= MOD; return *this; }
    ModInt &operator*=(ModInt that) { x = (unsigned long long)x * that.x % MOD; return *this; }
    ModInt &operator/=(ModInt that) { return *this *= that.inverse(); }
    friend ostream& operator<<(ostream& out, const ModInt& yo) { out << yo.x; return out; }

    ModInt operator+(ModInt that) const { return ModInt(*this) += that; }
    ModInt operator-(ModInt that) const { return ModInt(*this) -= that; }
    ModInt operator*(ModInt that) const { return ModInt(*this) *= that; }
    ModInt operator/(ModInt that) const { return ModInt(*this) /= that; }

    ModInt inverse() const {
        signed a = x, b = MOD, u = 1, v = 0;
        while (b) {
            signed t = a / b;
            a -= t * b; std::swap(a, b);
            u -= t * v; std::swap(u, v);
        }
        if (u < 0) u += Mod;
        ModInt res; res.x = (unsigned)u;
        return res;
    }
};
typedef ModInt<1000000007> mint;