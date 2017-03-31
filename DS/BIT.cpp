template<typename T>
struct BIT1	// Point update,query
{
	vector<T> t;int n;BIT1(){n=0;}
	BIT1(int sz){ n=sz; t=vector<T>(sz+1,0);}
	void upd(int i, T val){ while(i<=n) t[i]+=val, i+=i&-i;}
	T qry(int i){T ans=0; while(i>0) ans+=t[i],i-=i&-i; return ans;}
};

struct BIT2	// Range update,query
{
	vector<XXX> t;XXX n;BIT1 y,z;
	BIT2(XXX sz){n=sz;t.clr();t.rz(sz+1);y.clr(sz);z.clr(sz);}
	void updr(int l, int r, XXX val){y.upd(l,val); y.upd(r+1,-1*val); z.upd(l,l*val); z.upd(r+1,-1*(r+1)*val);}
	XXX qryr(int l,int r){return (r+1)*(y.qry(r)-y.qry(l-1)) + (r-l+1)*(y.qry(l-1)) - (z.qry(r)-z.qry(l-1));
};
