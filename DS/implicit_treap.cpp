#include <bits/stdc++.h>
using namespace std;

#define F first
#define S second
#define FOR(i,a,b) for (int i = (a); i <= (b); ++i)
#define NFOR(i,a,b) for(int i = (a); i >= (b); --i)
#define all(x) (x).begin(), (x).end()
#define sz(x) int(x.size())
typedef long long ll; typedef pair <int, int> ii; typedef vector <int> vi; const int inf = 1e9 + 7;
#define pr(...) dbs(#__VA_ARGS__, __VA_ARGS__)
template <class T> void dbs(string str, T t) {cout << str << " : " << t << "\n";}
template <class T, class... S> void dbs(string str, T t, S... s) {int idx = str.find(','); cout << str.substr(0, idx) << " : " << t << ","; dbs(str.substr(idx + 1), s...);}
template <class S, class T>ostream& operator <<(ostream& os, const pair<S, T>& p) {return os << "(" << p.first << ", " << p.second << ")";}
template <class T>ostream& operator <<(ostream& os, const vector<T>& p) {os << "[ "; for (auto& it : p) os << it << " "; return os << "]";}
template <class T>ostream& operator <<(ostream& os, const set<T>& p) {os << "[ "; for (auto& it : p) os << it << " "; return os << "]";}
template <class S, class T>ostream& operator <<(ostream& os, const map<S, T>& p) {os << "[ "; for (auto& it : p) os << it << " "; return os << "]";}
template <class T> void prc(T a, T b) {cout << "["; for (T i = a; i != b; ++i) {if (i != a) cout << ", "; cout << *i;} cout << "]\n";}
#define T int

const int N = 1e5;
struct node {
	T val;
	int lazy;
	vector<int> f;
	int prior, size;
	node *l, *r;
	node():f(10){}
};
node _arr[2 * N];
int ctr;

int fac[N+1], inv[N+1], ifac[N+1];

struct implicit_treap {
	node * root;
	implicit_treap() : root(0) {}

	int size(node * t) {
		return t ? t->size : 0;
	}
	void upd_size(node * t) {
		if (t)t->size = size(t->l) + 1 + size(t->r);
	}

	void lazy(node * t) {
		if (!t || !t->lazy)return;
		if (t->l)t->l->lazy ^= 1; //propagate lazy
		if (t->r)t->r->lazy ^= 1;
		if (!t->l) t->l = t->r, t->r = 0;
		else if (!t->r) t->r = t->l, t->l = 0;
		else swap(t->l, t->r);
		t->lazy = 0;
	}
	void reset(node * t) {
		if (t)t->f.assign(10, 0), t->f[t->val] = 1; //no need to reset lazy coz when we call this lazy would itself be propagated
	}
	void combine(node * &t, node * l, node * r) { //combining two ranges of segtree
		if (!l || !r)return void(t = l ? l : r);
		FOR (i, 0, 9) t->f[i] = (l ? l->f[i] : 0) + (r ? r->f[i] : 0);
	}
	void operation(node * t) { //operation of segtree
		if (!t)return;
		reset(t);//reset the value of current node assuming it now represents a single element of the array
		lazy(t->l); lazy(t->r); //imp:propagate lazy before combining t->l,t->r;
		combine(t, t->l, t);
		combine(t, t, t->r);
	}
	void split(node * t, node * &l, node * &r, int pos, int offset = 0) {
		if (!t)return void(l = r = NULL);
		lazy(t);
		int curr_pos = offset + size(t->l);
		if (curr_pos <= pos) //element at pos goes to left subtree(l)
			split(t->r, t->r, r, pos, curr_pos + 1), l = t;
		else
			split(t->l, l, t->l, pos, offset), r = t;
		upd_size(t);
		operation(t);
	}
	void merge(node * &t, node * l, node * r) { //l->leftarray,r->rightarray,t->resulting array
		lazy(l); lazy(r);
		if (!l || !r) t = l ? l : r;
		else if (l->prior > r->prior)merge(l->r, l->r, r), t = l;
		else    merge(r->l, l, r->l), t = r;
		upd_size(t);
		operation(t);
	}
	node * init(int val) {
		node * ret = &_arr[ctr++];
		ret->prior = rand(); ret->size = 1;
		ret->val = val;
		ret->f[val] = 1; ret->lazy = 0;
		return ret;
	}
	ll range_query(node * t, int l, int r) { //[l,r]
		node * L, * mid, * R;
		split(t, L, mid, l - 1);
		split(mid, t, R, r - l); //note: r-l!!
		int ans = 0; int count = 0;

		FOR (i, 0, 9) if (t->f[i] & 1) count++;
		if (count <= 1) {
			ans = fac[(r-l+1)/2];
			FOR (i, 0, 9) ans = (ans * 1LL * ifac[t->f[i] >> 1]) % inf;
		}
		merge(mid, L, t);
		merge(t, mid, R);
		return ans;
	}
	void range_update(node * t, int l, int r) { //[l,r]
		node * L, * mid, * R;
		split(t, L, mid, l - 1);
		split(mid, t, R, r - l); //note: r-l!!
		t->lazy ^= 1; //lazy_update
		merge(mid, L, t);
		merge(t, mid, R);
	}
};

#undef T




int main()
{
	ios::sync_with_stdio(0); cin.tie(0);
	srand(time(NULL));

	fac[0] = ifac[0] = fac[1] = ifac[1] = 1;
	inv[1] = 1;
	FOR (i, 2, N) {
		inv[i] = (-(inf / i) * 1LL * inv[inf % i]) % inf + inf;
		fac[i] = (i * 1LL * fac[i - 1]) % inf;
		ifac[i] = (inv[i] * 1LL * ifac[i - 1]) % inf;
	}

	implicit_treap t;
	int n, m; cin >> n >> m;
	string s; cin >> s;

	FOR (i, 0, n - 1) {
		node * temp;
		t.merge(temp, t.root, t.init(s[i] - 'a'));
		t.root = temp;
	}


	while (m--) {
		int type, l, r; 
		cin >> type >> l >> r;
		--l, --r;
		if (type == 1) {
			t.range_update(t.root, l, r);
		}
		else {
			cout << t.range_query(t.root, l, r) << "\n";
		}
	}

	return 0;
}
