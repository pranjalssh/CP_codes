#include <bits/stdc++.h>
using namespace std;

#define INF 1000000007

typedef long long ll;typedef unsigned long long ull;typedef vector<int> vi;typedef vector<ll> vll;typedef vector<vector<int> > vvi;typedef pair<int,int> ii;typedef vector<pair<int,int> > vii;typedef vector<vector<pair<int,int> > > vvii;

#define all(x) (x).begin(), (x).end()
#define nall(x) (x).rbegin(), (x).rend()
#define tr(x,it) for(vi::iterator it = (x).begin();it!=(x).end();++it)
#define ntr(x,it) for(auto it = (x).rbegin();it!=(x).rend();++it)
#define ufy(v) sort(all(v));(v).erase(unique(all((v))),(v).end())
#define sz(a) int((a).size())
#define boost ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0)
#define pb push_back
#define MP make_pair
#define clr clear
#define rz resize
#define C(a,b) memset(a,b,sizeof(a))
#define ia(a,n) FOR(i,0,n-1)cin>>a[i]
#define ia1(a,n) FOR(i,1,n)cin>>a[i]
#define fpresent(c,x) ((c).find(x) != (c).end())  // set,map
#define present(c,x) (find(all(c),x) != (c).end())  //vector
#define F first
#define S second
#define FOR(i,a,b) for(int i=a;i<=b;++i)
#define NFOR(i,a,b) for(int i=a;i>=b;--i)
#define rep(i,n) FOR(i,0,n-1)
#define TCASE int __T;cin>>__T;FOR(Tc,1,__T)
inline int add(int a,int b, int m=INF){a+=b;if(a>=m)a-=m;return a;}
inline int mul(int a,int b, int m=INF){return (int)(((ll)a*(ll)b)%m);}

#define mid ((l+r)>>1)
#define N 100010
vvi g;
int chead[N];
int clen[N];
int cpos[N];
vi t[N];
int n;
int p[N];
int s[20][N];
int lvl[N];
int sub[N];
ii e[N];
void dfs(int i)
{
    sub[i]=1;
  tr(g[i],it)
  {
    if(p[i]==*it)continue;
    p[*it]=i;
    lvl[*it]=lvl[i]+1;
    dfs(*it);
    sub[i]+=sub[*it];
  }
}
void pre()
{
  lvl[0]=0;
  p[0]=-1;
  dfs(0);
  FOR(i,0,n-1)
  for(int j=0;(1<<j)<n;++j)
  s[j][i]=-1;

  FOR(i,0,n-1)s[0][i]=p[i];

  for(int j=1;(1<<j)<n;++j)
  {
    FOR(i,0,n-1)if(s[j-1][i]!=-1)s[j][i]=s[j-1][s[j-1][i]];
  }

}
int lg2[N];
inline int lca(int u, int v)
{
  if(lvl[u]<lvl[v])swap(u,v);

  int LOG=lg2[lvl[u]];

  NFOR(i,LOG,0)
  if(lvl[u]-(1<<i)>=lvl[v])
  u=s[i][u];

  if(u==v)return u;

  NFOR(i,LOG,0)
  if(s[i][u]!=-1&&s[i][u]!=s[i][v])
  u=s[i][u],v=s[i][v];

  return p[u];
}

int decompose(int u,int head)
{
  cpos[u]=clen[head]+1;
  clen[head]++;
  chead[u]=head;

  ii mx=MP(-1,-1);
  tr(g[u],it)
  if(*it!=p[u])mx=max(mx,MP(sub[*it],*it));

  if(mx.F+1)
  decompose(mx.S,head);
  tr(g[u],it) if(*it!=p[u] and *it!=mx.S)decompose(*it,*it);
}

int qry(int num,int no,int l,int r,int u,int v)
{
    if(r<u or l>v)return 0;
    if(l>=u and r<=v)return t[num][no];
    return max(qry(num,2*no,l,mid,u,v),qry(num,2*no+1,mid+1,r,u,v));
}
void upd(int num,int no,int l,int r,int u,int val)
{
    if(l==r){t[num][no]=val;return;}
    if(u<=mid)upd(num,2*no,l,mid,u,val);
    else upd(num,2*no+1,mid+1,r,u,val);
    t[num][no]=max(t[num][2*no],t[num][2*no+1]);
}
int query(int u,int v)
{
    if(chead[u]==chead[v])
        return qry(chead[u],1,1,clen[chead[u]],cpos[v]+1,cpos[u]);
    return max(qry(chead[u],1,1,clen[chead[u]],1,cpos[u]),query(p[chead[u]],v));
}
int w[N];
int main()
{
    boost;
    lg2[1]=0;
    FOR(i,1,10000)lg2[i]=lg2[i-1]+((i&(i-1))==0);
    TCASE
    {

        cin>>n;
        assert(n>1);
        g.clr();
        FOR(i,0,n-1)p[i]=chead[i]=cpos[i]=-1,sub[i]=clen[i]=0,t[i].clr();

        g.rz(n);

        FOR(i,0,n-2){int a,b,c;cin>>a>>b>>c;g[a-1].pb(b-1);g[b-1].pb(a-1),e[i]=MP(a-1,sz(g[a-1])-1);w[i]=c;}

        pre();
        decompose(0,0);

        FOR(i,0,n-2)
        {
            int x=e[i].F;
            int y=g[e[i].F][e[i].S];
            if(p[x]!=y)swap(x,y);
            if(t[chead[x]].size()==0)t[chead[x]].rz(clen[chead[x]]*4+10);
            upd(chead[x],1,1,clen[chead[x]],cpos[x],w[i]);
        }

        while(1)
        {
            string ss;
            cin>>ss;
            if(ss=="DONE")break;
            if(ss=="CHANGE")
            {
                int i,tt;
                cin>>i>>tt;
                i--;
                int x=e[i].F;
                int y=g[e[i].F][e[i].S];
                if(p[x]!=y)swap(x,y);

                upd(chead[x],1,1,clen[chead[x]],cpos[x],tt);

            }
            else
            {
                int a,b;
                cin>>a>>b;
                --a,--b;

                int x=lca(a,b);
                cout<<max(query(a,x),query(b,x))<<"\n";
            }
        }
        cout<<"\n";

    }
  return 0;
}
