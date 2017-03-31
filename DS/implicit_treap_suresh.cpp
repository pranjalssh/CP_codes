#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
// #include <sys/resource.h>

using namespace std;
using namespace __gnu_pbds;

template<class T, class U>
ostream& operator<<(ostream& out, const pair<T, U>& a) {out << "[" << a.first << " " << a.second << "]"; return out;}
template<class T>
ostream& operator<<(ostream& out, const vector<T>& a) {out << "[ "; for (auto &it : a)out << it << " "; out << "]"; return out;}
template<class T>
ostream& operator<<(ostream& out, const set<T>& a) {out << "[ "; for (auto &it : a)out << it << " "; out << "]"; return out;}
template<class T>
ostream& operator<<(ostream& out, const multiset<T>& a) {out << "[ "; for (auto &it : a)out << it << " "; out << "]"; return out;}
template<class T, class U>
ostream& operator<<(ostream& out, const map<T, U>& a) {for (auto &it : a)out << it.first << " -> " << it.second << " | "; return out;}
template<class T, class U>
ostream& operator<<(ostream& out, const multimap<T, U>& a) {for (auto &it : a)out << it.first << " -> " << it.second << " | "; return out;}


#define cerr cout

#define pra(a,n) cerr<<#a<<" : ";for(int i=0;i<n;++i)cerr<<a[i]<<" ";cerr<<endl;
#define praa(a,n,m) cerr<<#a<<" : "<<endl;for(int i=0;i<n;++i){for(int j=0;j<m;++j)cerr<<a[i][j]<<" ";cerr<<endl;}
#define pr(...) __f(#__VA_ARGS__, __VA_ARGS__)
template <typename Arg1>
void __f(const char* name, Arg1&& arg1) {
	cerr << name << " : " << arg1 << std::endl;
}
template <typename Arg1, typename... Args>
void __f(const char* names, Arg1&& arg1, Args&&... args) {
	const char* comma = strchr(names + 1, ','); cerr.write(names, comma - names) << " : " << arg1 << " | "; __f(comma + 1, args...);
}
#undef cerr

#define inc_stack_limit const rlim_t kStackSize = 64 * 1024 * 1024;struct rlimit rl;rl.rlim_cur = kStackSize;setrlimit(RLIMIT_STACK, &rl);
#define all(x) (x).begin(), (x).end()
#define nall(x) (x).rbegin(), (x).rend()
#define sz(a) int((a).size())
#define pb push_back
#define MP make_pair
#define F first
#define S second
#define FOR(i,a,b) for(int i=(a);i<=(b);++i)
#define NFOR(i,a,b) for(int i=(a);i>=(b);--i)
#define TCASE int __T;cin>>__T;FOR(Tc,1,__T)


typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
typedef long long ll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<vll> vvll;
typedef vector<vector<int> > vvi;
typedef pair<int, int> ii;
typedef vector<pair<int, int> > vii;
typedef vector<vector<pair<int, int> > > vvii;

const ll inf2 = 1e18;
const int inf = 1e9 + 7;

inline int add(int a, int b, int m = inf) {if ((a += b) >= m)a -= m; return a;}
inline int sub(int a, int b, int m = inf) {if ((a -= b) < 0)a += m; return a;}
inline int mul(int a, int b, int m = inf) {return (int)(((ll)a * (ll)b) % m);}


const int N = 1e5 + 100;

struct node;
void push(node * x);

struct node {
	int cnt, prior;
	ll sum, lazy, val;
	node * l, * r;
	inline void update() {
		if (l) push(l);
		if (r) push(r);
		cnt = 1 + (l ? l->cnt : 0) + (r ? r->cnt : 0);
		sum = val + (l ? l->sum : 0) + (r ? r->sum : 0);
	}
} mem[N];
typedef node * pNode;
int memPtr;
inline pNode newNode() {
	return mem + memPtr++;
}

inline int size(pNode x) {
	return x ? x->cnt : 0;
}

inline ll sum(pNode x) {
	return x ? x->sum : 0;
}

void push(node * x) {
	if (not x) return;
	x-> sum += x->lazy * x->cnt;
	x-> val += x->lazy;
	if (x->l) x->l->lazy += x->lazy;
	if (x->r) x->r->lazy += x->lazy;
	x->lazy = 0;
}

void split(pNode x, pNode & l, pNode & r, int key, int add = 0) {
	if(!x) {
		l = r = NULL;
		return;
	}
	push(x);
	int curKey = add + size(x->l);
	if(key <= curKey) {
		split(x->l, l, x->l, key, add);
		r = x;
	} else {
		split(x->r, x->r, r, key, curKey + 1);
		l = x;
	}
	x->update();
}

void merge(pNode & x, pNode l, pNode r) {
	push(l), push(r);
	if(!l or !r)
		x = l ? l : r;
	else if(l->prior > r->prior) {
		merge(l->r, l->r, r);
		x = l;
	} else {
		merge(r->l, l, r->l);
		x = r;
	}
	if(x) x->update();
}

inline void insert(pNode & T, int pos, int v) {
	node * cur = newNode();
	cur->val = cur->sum = v;
	cur->lazy = 0;
	cur->prior = rand();
	cur->cnt = 1;
	cur->l = cur->r = NULL;
	pNode l=0, r=0;
	split(T, l, r, pos);
	merge(l, l, cur);
	merge(T, l, r);
}

inline void update(pNode & T, int pos, int v) {
	pNode l=0, m=0, r=0;
	split(T, l, m, pos);
	split(m, m, r, 1);
	if(m) {
		m->sum = m->val = v;
	}
	merge(T, l, m);
	merge(T, T, r);
}

inline ll getSum(pNode & T, int ql, int qr) {
	pNode l, m, r;
	split(T, l, m, ql);
	split(m, m, r, qr - ql + 1);
	push(m);
	ll ret = sum(m);
	merge(T, l, m);
	merge(T, T, r);
	return ret;
}

inline void rangeUpdate(pNode & T, int ql, int qr, ll val) {
	pNode l, m, r;
	split(T, l, m, ql);
	split(m, m, r, qr - ql + 1);
	m->lazy += val;
	push(m);
	merge(T, l, m);
	merge(T, T, r);
}

void print(pNode T) {
	if (not T) return;
	pr(T->val, T->cnt, T->sum, T->lazy);
	print(T->l);
	print(T->r);
	return;
}

int main() 
{
	ios_base::sync_with_stdio(false);    cin.tie(NULL);

	srand(time(0));

	TCASE {
		memPtr = 0;
		int n; cin >> n;
		pNode T = 0;
		FOR (i, 0, n - 1) insert(T, i, 0);
		// print(T);
		TCASE {
			int c, p, q; cin >> c >> p >> q;
			if (c == 0) {
				ll v; cin >> v;
				rangeUpdate(T, p - 1, q - 1, v);

			} else {
				cout << getSum(T, p - 1, q - 1) << "\n";
			}
		}
	}

	return 0;    
}

