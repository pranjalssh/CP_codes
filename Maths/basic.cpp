// for (int x = y; x > 0; x = ( y & (x-1) ) ) 
    
inline int add(int a,int b, int m=INF){a+=b;if(a>=m)a-=m;return a;}
inline int mul(int a,int b, int m=INF){return (int)(((ll)a*(ll)b)%m);}


template<int MOD>
struct ModInt {
    static const int Mod = MOD;
    unsigned x;
    ModInt() : x(0) {}
    ModInt(signed sig) { int sigt = sig % MOD; if(sigt < 0) sigt += MOD; x = sigt; }
    ModInt(signed long long sig) { int sigt = sig % MOD; if(sigt < 0) sigt += MOD; x = sigt; }
    int get() const { return (int)x; }
 
    ModInt &operator+=(ModInt that) { if((x += that.x) >= MOD) x -= MOD; return *this; }
    ModInt &operator-=(ModInt that) { if((x += MOD - that.x) >= MOD) x -= MOD; return *this; }
    ModInt &operator*=(ModInt that) { x = (unsigned long long)x * that.x % MOD; return *this; }
    ModInt &operator/=(ModInt that) { return *this *= that.inverse(); }
    friend ostream& operator<<(ostream& out, const ModInt& yo) { out << yo.x; return out; }
 
    ModInt operator+(ModInt that) const { return ModInt(*this) += that; }
    ModInt operator-(ModInt that) const { return ModInt(*this) -= that; }
    ModInt operator*(ModInt that) const { return ModInt(*this) *= that; }
    ModInt operator/(ModInt that) const { return ModInt(*this) /= that; }
 
    ModInt inverse() const {
        signed a = x, b = MOD, u = 1, v = 0;
        while(b) {
            signed t = a / b;
            a -= t * b; std::swap(a, b);
            u -= t * v; std::swap(u, v);
        }
        if(u < 0) u += Mod;
        ModInt res; res.x = (unsigned)u;
        return res;
    }
};
typedef ModInt<1000000007> mint;
typedef ModInt<7340033> mintt;
 

ll Expo(ll n,ll p){
    if(p==0)return 1;
    ll x = Expo(n,p>>1);
    x=(x*x)%INF;
    return ((p&1)?(x*n)%INF:x);
}



const int N = 1e5;
int sv[N+1];
int fac[N+1];
int inv[N+1];
int ifac[N+1];


int expo(int n,int p,int MOD){
	if(p==0)return 1;
	int x = expo(n,p>>1,MOD);
	x=mul(x,x,MOD);
	return (p&1)?mul(x,n,MOD):x;
}
inline int C(int n,int k){
	if(k>n)return 0;
	return mul(fac[n],mul(ifac[k],ifac[n-k]));
}
int main()
{
    boost;
    fac[0]=1;ifac[0]=1;inv[1]=1;FOR(i,1,N)sv[i]=i;

    FOR(i,2,316)if(sv[i]==i)for(int j=i*i;j<=N;j+=i)sv[j]=i;
    FOR(i,1,N)fac[i]=mul(fac[i-1],i);
    FOR(i,2,N)inv[i]=INF - ((INF/i)*1LL*inv[INF%i])%INF;
    FOR(i,1,N)ifac[i]=mul(ifac[i-1],inv[i]);
        
    cout<<expo(2,100,INF);

    return 0;
}
