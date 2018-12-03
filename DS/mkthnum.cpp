#include <bits/stdc++.h>
using namespace std;

#define INF 1000000007

typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<vector<int> > vvi;
typedef pair<int,int> ii;
typedef vector<pair<int,int> > vii;
typedef vector<vector<pair<int,int> > > vvii;

#define all(x) x.begin(), x.end()
#define tr(x,it) for(auto it = x.begin();it!=x.end();++it)
#define ntr(x,it) for(auto it = x.rbegin();it!=x.rend();++it)
#define sz(a) int((a).size()) 
#define pb push_back 
#define MP make_pair
#define fpresent(c,x) ((c).find(x) != (c).end())  // set,map
#define present(c,x) (find(all(c),x) != (c).end())  //vector
#define F first
#define S second
#define FOR(i,a,b) for(int i=a;i<=b;++i)
#define NFOR(i,a,b) for(int i=a;i>=b;--i)




vii a;
vi b;
vi ans;
struct node
{
	int no;
	node * left;
	node * right;
	
	node(int no, node * left, node * right) : no(no),left(left),right(right) {}
	
	node * ghus(int l , int r, int w);
};
node * bc;

node * node::ghus(int l , int r, int w)
{
	if(w>=l&&w<=r)
	{
		if(l==r)
		return new node(this->no+1,bc,bc);
			
		return new node(this->no+1,this->left->ghus(l,(l+r)>>1,w),this->right->ghus(((l+r)>>1)+1,r,w));
	}
	return this; // this fucking shit
}


vector<node *> root;
int query(int l, int r, int k, node * a, node *b)
{
	if(l==r)return r;
	
	int ct=b->left->no-a->left->no;
	
	if(ct>=k) 	return query(l, (l+r)>>1,k,a->left,b->left);
	else return query(((l+r)>>1)+1,r,k-ct,a->right,b->right);
}
int main()
{
    ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    
    int n,m;
    cin>>n>>m;
    
    a.resize(n+1);
    ans.resize(n+1);
    b.resize(n+1);
    FOR(i,1,n)cin>>a[i].F,a[i].S=i;
    
    sort(++a.begin(),a.end());
    
    FOR(i,1,n)
    b[a[i].S]=i,ans[i]=a[i].F;
    
    root.resize(n+1);
    
    root[0]=new node(0,NULL,NULL);
    root[0]->left=root[0]->right=root[0];
    
    bc=root[0];
    
    FOR(i,1,n)
    {
    	root[i]=root[i-1]->ghus(1,n,b[i]);
    }
    while(m--)
    {
    	int l,r,k;
    	cin>>l>>r>>k;
    	cout<<ans[query(1,n,k,root[l-1],root[r])]<<"\n";
    }
	return 0;
}












