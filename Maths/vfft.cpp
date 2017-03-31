
namespace FFT
{
#define fore(i, a, b) for(int i = (int)(a); i <= (int)(b); ++i)
#define forn(i, n) for(int i = 0; i < (int)(n); ++i)
typedef long double ld;
typedef long long LL;
LL power(LL a, LL p, LL mod)
{
    LL ret = 1;
    while (p)
    {
        if (p & 1)ret = (ret * a) % mod;
        a = (a * a) % mod;
        p /= 2;
    }
    return ret;
}
const int MOD = 13313;
struct base
{
    typedef double T;
    T re, im;
    base() : re(0), im(0) {}
    base(T re) : re(re), im(0) {}
    base(T re, T im) : re(re), im(im) {}
    base operator + (const base& o) const {return base(re + o.re, im + o.im);}
    base operator - (const base& o) const {return base(re - o.re, im - o.im);}
    base operator * (const base& o) const
    {
        return base(re * o.re - im * o.im, re * o.im + im * o.re);
    }
    base operator * (ld k) const
    {
        return base(re * k, im * k) ;
    }
    base conj() const
    {
        return base(re, -im);
    }
};
const int N = 21;
const int MAXN = (1 << N);
base w[MAXN];
base f1[MAXN];
int rev[MAXN];
void build_rev(int k)
{
    static int rk = -1;
    if ( k == rk )return ;
    rk = k;
    fore(i, 1, (1 << k) )
    {
        int j = rev[i - 1], t = k - 1;
        while ( t >= 0 && ((j >> t) & 1) )
        {
            j ^= 1 << t;
            --t;
        }
        if ( t >= 0 )
        {
            j ^= 1 << t;
            --t;
        }
        rev[i] = j;
    }
}
void fft(base *a, int k)
{
    build_rev(k);
    int n = 1 << k;
    forn(i, n) if ( rev[i] > i ) swap(a[i], a[rev[i]]);
    for (int l = 2, ll = 1; l <= n; l += l, ll += ll)
    {
        if ( w[ll].re == 0 && w[ll].im == 0 )
        {
            ld angle = M_PI / ll;
            base ww( cosl(angle), sinl(angle) );
            if ( ll > 1 ) for (int j = 0; j < ll; ++j)
                {
                    if ( j & 1 ) w[ll + j] = w[(ll + j) / 2] * ww;
                    else w[ll + j] = w[(ll + j) / 2];
                }
            else w[ll] = base(1, 0);
        }
        for (int i = 0; i < n; i += l) forn(j, ll)
        {
            base v = a[i + j], u = a[i + j + ll] * w[ll + j];
            a[i + j] = v + u;
            a[i + j + ll] = v - u;
        }
    }
}
vi mul(const vi& a, const vi& b)
{
    int k = 1;
    while ( (1 << k) < (sz(a) + sz(b)) ) ++k;
    int n = (1 << k);
    forn(i, n) f1[i] = base(0, 0);
    forn(i, a.size()) f1[i] = f1[i] + base(a[i], 0);
    forn(i, b.size()) f1[i] = f1[i] + base(0, b[i]);
    fft(f1, k);
    forn(i, 1 + n / 2)
    {
        base p = f1[i] + f1[(n - i) % n].conj();
        base _q = f1[(n - i) % n] - f1[i].conj();
        base q(_q.im, _q.re);
        f1[i] = (p * q) * 0.25;
        if ( i > 0 ) f1[(n - i)] = f1[i].conj();
    }
    forn(i, n) f1[i] = f1[i].conj();
    fft(f1, k);
    vi r(sz(a) + sz(b));
    forn(i, sz(r) )
    {
        r[i] = ll (f1[i].re / n + 0.5) % MOD;
    }
    return r;
}
void addmod(int& x, int y, int MOD) {
    (x += y) >= MOD && (x -= MOD);
}

int mulmod(int x, int y, int MOD) {
    return x * 1ll * y % MOD;
}

vi multmod(const vi& a, const vi& b) {
    // a = a0 + sqrt(mod) * a1
    // a = a0 + base * a1
    int base = sqrtl(MOD);

    vi a0(sz(a)), a1(sz(a));
    forn(i, sz(a)) {
        a0[i] = a[i] % base;
        a1[i] = a[i] / base;
        assert(a[i] == a0[i] + base * a1[i]);
    }

    vi b0(sz(b)), b1(sz(b));
    forn(i, sz(b)) {
        b0[i] = b[i] % base;
        b1[i] = b[i] / base;
        assert(b[i] == b0[i] + base * b1[i]);
    }

    vi a01 = a0;
    forn(i, sz(a)) {
        addmod(a01[i], a1[i], MOD);
    }

    vi b01 = b0;
    forn(i, sz(b)) {
        addmod(b01[i], b1[i], MOD);
    }


    vi C = mul(a01, b01);  // 1

    vi a0b0 = mul(a0, b0); // 2
    vi a1b1 = mul(a1, b1); // 3


    vi mid = C;
    forn(i, sz(mid)) {
        addmod(mid[i], -a0b0[i] + MOD, MOD);
        addmod(mid[i], -a1b1[i] + MOD, MOD);
    }

    vi res = a0b0;
    forn(i, sz(res)) {
        addmod(res[i], mulmod(base, mid[i], MOD), MOD);
    }

    base = mulmod(base, base, MOD);
    forn(i, sz(res)) {
        addmod(res[i], mulmod(base, a1b1[i], MOD), MOD);
    }

    return res;
}
}