#if 0
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <map>
#include <queue>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstring>
#include <cctype>
#include <cassert>
#define rep(i,n) for(int (i)=0;(i)<(int)(n);++(i))
#define rer(i,l,u) for(int (i)=(int)(l);(i)<=(int)(u);++(i))
#define reu(i,l,u) for(int (i)=(int)(l);(i)<(int)(u);++(i))
#if defined(_MSC_VER)
#define aut(r,v) auto r = (v)
#else
#define aut(r,v) typeof(v) r = (v)
#endif
#define each(it,o) for(aut(it, (o).begin()); it != (o).end(); ++ it)
#define all(o) (o).begin(), (o).end()
#define pb(x) push_back(x)
#define mp(x,y) make_pair((x),(y))
#define mset(m,v) memset(m,v,sizeof(m))
#define INF 0x3f3f3f3f
#define INFL 0x3f3f3f3f3f3f3f3fLL
#define EPS 1e-9
using namespace std;
typedef vector<int> vi; typedef pair<int,int> pii; typedef vector<pair<int,int> > vpii;
typedef long long ll; typedef vector<long long> vl; typedef pair<long long,long long> pll; typedef vector<pair<long long,long long> > vpll;
typedef vector<string> vs; typedef long double ld;

void __cdecl _wassertp(_In_z_ const wchar_t * _Message, _In_z_ const wchar_t *_File, _In_ unsigned _Line) {
	wcout << "Assert failed: " << _Message << " at " << _Line << endl;
}
#undef assert
#define assert(_Expression) (void)( (!!(_Expression)) || (_wassertp(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )

//http://www.haskell.org/ghc/docs/latest/html/libraries/containers-0.5.0.0/Data-Map-Lazy.html
//を参考に書いてみるテスト
//※mergeはO(n+m), splitはO(log n)
typedef unsigned Key; typedef unsigned Val;
struct Bin;
typedef const Bin* Node;
struct Bin {
	int s;
	Key k; Val x;
	Node l, r;
	Bin(){}
	Bin(int s_, Key k_, Val x_, Node l_, Node r_):
		s(s_), k(k_), x(x_), l(l_), r(r_) {}
};

Bin heap[30000000];
int heap_index = 0;
Node new_Node(int s, Key k, Val x, Node l, Node r) {
	return new(&heap[heap_index ++])Bin(s, k, x, l, r);
}

//基本プロパティ
int size(Node t) {
	if(t == NULL) return 0;
	else return t->s;
}

//バランス
const int delta = 3;
const int ratio = 2;
Node balanceL(Key k, Val x, Node l, Node r) {
	if(r == NULL) {
		if(l == NULL) return new_Node(1, k, x, NULL, NULL);
		if(l->l == NULL && l->r == NULL) return new_Node(2, k, x, l, NULL);
		if(l->l == NULL) return new_Node(3, l->r->k, l->r->x, new_Node(1, l->k, l->x, NULL, NULL), new_Node(1, k, x, NULL, NULL));
		if(l->r == NULL) return new_Node(3, l->k, l->x, l->l, new_Node(1, k, x, NULL, NULL));
		if(l->r->s < ratio*l->l->s) return new_Node(1+l->s, l->k, l->x, l->l, new_Node(1+l->r->s, k, x, l->r, NULL));
		else return new_Node(1+l->s, l->r->k, l->r->x, new_Node(1+l->l->s+size(l->r->l), l->k, l->x, l->l, l->r->l), new_Node(1+size(l->r->r), k, x, l->r->r, NULL));
	}else {
		if(l == NULL) return new_Node(1+r->s, k, x, NULL, r);
		else if(l->s > delta*r->s) {
			assert(l->l != NULL && l->r != NULL);
			if(l->r->s < ratio*l->l->s) return new_Node(1+l->s+r->s, l->k, l->x, l->l, new_Node(1+r->s+l->r->s, k, x, l->r, r));
			else return new_Node(1+l->s+r->s, l->r->k, l->r->x, new_Node(1+l->l->s+size(l->r->l), l->k, l->x, l->l, l->r->l), new_Node(1+r->s+size(l->r->r), k, x, l->r->r, r));
		}else return new_Node(1+l->s+r->s, k, x, l, r);
	}
}
Node balanceR(Key k, Val x, Node l, Node r) {
	if(l == NULL) {
		if(r == NULL) return new_Node(1, k, x, NULL, NULL);
		if(r->l == NULL && r->r == NULL) return new_Node(2, k, x, NULL, r);
		if(r->l == NULL) return new_Node(3, r->k, r->x, new_Node(1, k, x, NULL, NULL), r->r);
		if(r->r == NULL) return new_Node(3, r->l->k, r->l->x, new_Node(1, k, x, NULL, NULL), new_Node(1, r->k, r->x, NULL, NULL));
		if(r->l->s < ratio*r->r->s) return new_Node(1+r->s, r->k, r->x, new_Node(1+r->l->s, k, x, NULL, r->l), r->r);
		else return new_Node(1+r->s, r->l->k, r->l->x, new_Node(1+size(r->l->l), k, x, NULL, r->l->l), new_Node(1+r->r->s+size(r->l->r), r->k, r->x, r->l->r, r->r));
	}else {
		if(r == NULL) return new_Node(1+l->s, k, x, l, NULL);
		if(r->s > delta*l->s) {
			assert(r->l != NULL && r->r != NULL);
			if(r->l->s < ratio*r->r->s) return new_Node(1+l->s+r->s, r->k, r->x, new_Node(1+l->s+r->l->s, k, x, l, r->l), r->r);
			else return new_Node(1+l->s+r->s, r->l->k, r->l->x, new_Node(1+l->s+size(r->l->l), k, x, l, r->l->l), new_Node(1+r->r->s+size(r->l->r), r->k, r->x, r->l->r, r->r));
		}else return new_Node(1+l->s+r->s, k, x, l, r);
	}
}

//glueに使われる
Node eraseFindMin(Node t, Key &out_k, Val &out_x) {
	assert(t != NULL);
	if(t->l == NULL) {
		out_k = t->k; out_x = t->x;
		return t->r;
	}else {
		Node lp = eraseFindMin(t->l, out_k, out_x);
		return balanceR(t->k, t->x, lp, t->r);
	}
}

Node eraseFindMax(Node t, Key &out_k, Val &out_x) {
	assert(t != NULL);
	if(t->r == NULL) {
		out_k = t->k; out_x = t->x;
		return t->l;
	}else {
		Node rp = eraseFindMax(t->r, out_k, out_x);
		return balanceL(t->k, t->x, t->l, rp);
	}
}

//eraseに使われる
Node glue(Node l, Node r) {
	if(l == NULL) return r;
	if(r == NULL) return l;
	if(l->s > r->s) {
		Key km; Val m;
		Node lp = eraseFindMax(l, km, m);
		return balanceR(km, m, lp, r);
	}else {
		Key km; Val m;
		Node rp = eraseFindMin(r, km, m);
		return balanceR(km, m, l, rp);
	}
}


Node empty() { return NULL; }
Node singleton(Key k, Val x) {
	return new_Node(1, k, x, NULL, NULL);
}
//1つのkeyに複数の値を許す
Node insert(Key kx, Val x, Node t) {
	if(t == NULL) return singleton(kx, x);
	if(kx <= t->k)
		return balanceL(t->k, t->x, insert(kx, x, t->l), t->r);
	else
		return balanceR(t->k, t->x, t->l, insert(kx, x, t->r));
}
//1つのkeyに複数の値がある場合はどれでも
Node erase(Key k, Node t) {
	if(t == NULL) return NULL;
	if(k < t->k) return balanceR(t->k, t->x, erase(k, t->l), t->r);
	if(!(k == t->k)) return balanceL(t->k, t->x, t->l, erase(k, t->r));
	return glue(t->l, t->r);
}

bool lookup(Key k, Node t, Val &out_x) {
	if(t == NULL) return false;
	if(k < t->k) return lookup(k, t->l, out_x);
	if(!(k == t->k)) return lookup(k, t->r, out_x);
	out_x = t->x;
	return true;
}

//lowerbound
int lowerbound(Key k, Node t) {
	if(t == NULL) return 0;
	if(k <= t->k) return lowerbound(k, t->l);
	else return t->s + 1 + lowerbound(k, t->r);
}

//デバッグ用
bool balanced(Node t) {
	if(t == NULL) return true;
	return (
		size(t->l) + size(t->r) <= 1 ||
		size(t->l) <= delta*size(t->r) && size(t->r) <= delta*size(t->l))
		&& balanced(t->l) && balanced(t->r);
}
int realsize(Node t) {
	if(t == NULL) return 0;
	int n = realsize(t->l), m = realsize(t->r);
	if(n == -1 || m == -1) {
		return -1;
	}
	if(n + m + 1 != t->s) {
		return -1;
	}
	return t->s;
}
bool validsize(Node t) {
	return realsize(t) == size(t);
}

unsigned xor128() {
	static unsigned x = 123456789;
	static unsigned y = 362436069;
	static unsigned z = 521288629;
	static unsigned w = 88675123;
	unsigned t;
 
	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

struct Timer {
	const char* name;
	clock_t cl;
	Timer(const char* s): name(s) { cl = clock(); }
	~Timer() {
		fprintf(stderr, "%s: %dmsec.\n", name, clock() - cl);
	}
};

int main() {
	const int N = 300000, M = 100000;
	Node t = empty();
	{ Timer timer("inserts random");
	rep(i, N) {
		t = insert(xor128() % M, xor128() % M, t);
	}
	}
	cout << "memory: " << (heap_index * sizeof(Bin)) / 1024. / 1024 << "MBytes" << endl;
	while(getchar()!='\n');
	{ Timer timer("erases ramom");
	rep(i, N) {
		t = erase(xor128() % M, t);
	}
	}
	cout << "memory: " << (heap_index * sizeof(Bin)) / 1024. / 1024 << "MBytes" << endl;
	while(getchar()!='\n');
	t = empty();
	{ Timer timer("inserts sequencial");
	rep(i, N) {
		t = insert(i, xor128() % M, t);
	}
	}
	cout << "memory: " << (heap_index * sizeof(Bin)) / 1024. / 1024 << "MBytes" << endl;
	while(getchar()!='\n');
	{ Timer timer("erases sequencial");
	rep(i, N) {
		t = erase(i, t);
	}
	}
	cout << "memory: " << (heap_index * sizeof(Bin)) / 1024. / 1024 << "MBytes" << endl;
	while(getchar()!='\n');
	t = empty();
	{ Timer timer("inserts random");
	rep(i, N) {
		t = insert(xor128() % M, xor128() % M, t);
	}
	}
	cout << "memory: " << (heap_index * sizeof(Bin)) / 1024. / 1024 << "MBytes" << endl;
	while(getchar()!='\n');
	{ Timer timer("lowerbound random");
	int x = 0;
	rep(i, N) {
		x += lowerbound(xor128() % M, t);
	}
	if(x == 0) cerr << "!" << endl;
	}
	while(getchar()!='\n');
	{ Timer timer("lowerbound sequencial");
	int x = 0;
	rep(i, N) {
		x += lowerbound(i, t);
	}
	if(x == 0) cerr << "!" << endl;
	}
	while(getchar()!='\n');
	/*
	const int N = 30000, M = 3000000, K = 1000, V = 1000;
	Node t = empty();
	multimap<Key,Val> a;
	vector<Key> ks;
	rep(i, N) {
		Key k = xor128() % K;
		Val x = xor128() % V;
//		if(a.find(k) != a.end()) continue;
		t = insert(k, x, t);
		a.insert(make_pair(k, x));
		ks.push_back(k);
		if(i < 100 || i % 10000 == 0) {
			assert(balanced(t));
			assert(validsize(t));
			assert(size(t) == a.size());
		}
	}
	rep(i, M) {
		Key k = xor128() % N;
		auto ai = a.find(k);
		if(ai != a.end()) {
			Val x;
			assert(lookup(k, t, x));
			for(; ai != a.end() && ai->first == k && ai->second != x; ai ++) ;
			assert(ai != a.end() && ai->first == k && ai->second == x);
			t = erase(k, t);
			a.erase(ai);
		}else {
			Val x;
			assert(!lookup(k, t, x));
		}
		if(i >= M - 100 || i % 10000 == 0) {
			assert(balanced(t));
			assert(validsize(t));
			assert(size(t) == a.size());
		}
	}
	cout << "OK!!!!!" << endl;
	*/
	return 0;
}
#endif




//////////////////////////
//なにか。とちゅう

struct PersistentBalancedTree {
	typedef Bit Val;
	typedef Val::Measured Measured;
	static const int HeapSize = 1024*1024*1024/2/4/5;

	struct NodeT;
	typedef const NodeT *Node;
	struct NodeT {
		Node left, right;
		int size;
		Val val;
		Measured measured;

		NodeT(Node l, Node r, int s, const Val &v, const Measured &m):
			left(l), right(r), size(s), val(v), measured(m) {}

		static int size_(Node t) {
			return !t ? 0 : t->size;
		}
		static Measured measure_(Node t) {
			return !t ? Measured::zero() : t->measured;
		}
	};
	static NodeT heap[HeapSize]; static int heapindex;

	static Node node(const Val &val, Node left, Node right) {
		return new(&heap[heapindex++])
			NodeT(left, right, NodeT::size_(left) + 1 + NodeT::size_(right), val,
				NodeT::measure_(left) + val.measure() + NodeT::measure_(right));
	}
	static Node balanceL(const Val &x, Node l, Node r) {
		const int delta = 3, ratio = 2;
		if(r == NULL) {
			if(l == NULL) return node(x, NULL, NULL);
			Node ll = l->left, lr = l->right;
			if(ll == NULL && lr == NULL) return node(x, l, NULL);
			if(ll == NULL) return node(lr->val, node(l->val, NULL, NULL), node(x, NULL, NULL));
			if(lr == NULL) return node(l->val, ll, node(x, NULL, NULL));
			if(lr->size < ratio*ll->size) return node(l->val, ll, node(x, lr, NULL));
			return node(lr->val, node(l->val, ll, lr->left), node(x, lr->right, NULL));
		}else {
			if(l == NULL) return node(x, NULL, r);
			if(l->size <= delta*r->size) return node(x, l, r);
			Node ll = l->left, lr = l->right;
			if(lr->size < ratio*ll->size) return node(l->val, ll, node(x, lr, r));
			return node(lr->val, node(l->val, ll, lr->left), node(x, lr->right, r));
		}
	}
	static Node balanceR(const Val &x, Node l, Node r) {
		const int delta = 3, ratio = 2;
		if(l == NULL) {
			if(r == NULL) return node(x, NULL, NULL);
			Node rl = r->left, rr = r->right;
			if(rl == NULL && rr == NULL) return node(x, NULL, r);
			if(rl == NULL) return node(r->val, node(x, NULL, NULL), rr);
			if(rr == NULL) return node(rl->val, node(x, NULL, NULL), node(r->val, NULL, NULL));
			if(rl->size < ratio*rr->size) return node(r->val, node(x, NULL, rl), rr);
			return node(rl->val, node(x, NULL, rl->left), node(r->val, rl->right, rr));
		}else {
			if(r == NULL) return node(x, l, NULL);
			if(r->size <= delta*l->size) return node(x, l, r);
			Node rl = r->left, rr = r->right;
			if(rl->size < ratio*rr->size) return node(r->val, node(x, l, rl), rr);
			return node(rl->val, node(x, l, rl->left), node(x, rl->right, rr));
		}
	}
	static Node eraseLeftmost(Node t, Val &out_val) {
		if(t->left == NULL) {
			out_val = t->val;
			return t->right;
		}else {
			Node lp = eraseLeftmost(t->left, out_val);
			return balanceR(t->val, lp, t->right);
		}
	}
	static Node eraseRightmost(Node t, Val &out_val) {
		if(t->right == NULL) {
			out_val = t->val;
			return t->left;
		}else {
			Node rp = eraseRightmost(t->right, out_val);
			return balanceL(t->val, t->left, rp);
		}
	}
	static Node merge(Node l, Node r) {
		if(l == NULL) return r;
		if(r == NULL) return l;
		if(l->size > r->size) {
			Val m;
			Node lp = eraseRightmost(l, m);
			return balanceR(m, lp, r);
		}else {
			Val m;
			Node rp = eraseLeftmost(r, m);
			return balanceL(m, l, rp);
		}
	}
	static pair<Node, Node> split(Node t, int k) {
		if(t == NULL) return make_pair(NULL, NULL);
		int s = NodeT::size_(t->left);
		if(k < s) {
			pair<Node, Node> p = split(t->left, k);
			return balanceR(t->val, p.first, t->right);
		}
		if(k > s) return balanceL(t->val, t->left, erase(t->right, k-s-1));
		return merge(t->left, t->right);
	}
	static Node insert(Node t, int k, const Val &x) {
		if(t == NULL) return singleton(x);
		int s = NodeT::size_(t->left);
		if(k <= s)
			return balanceL(t->val, insert(t->left, k, x), t->right);
		else
			return balanceR(t->val, t->left, insert(t->right, k-s-1, x));
	}
	static Node erase(Node t, int k) {
		if(t == NULL) return NULL;
		int s = NodeT::size_(t->left);
		if(k < s) return balanceR(t->val, erase(t->left, k), t->right);
		if(k > s) return balanceL(t->val, t->left, erase(t->right, k-s-1));
		return merge(t->left, t->right);
	}

	static Node empty() { return NULL; }
	static Node singleton(const Val &x) {
		return node(x, NULL, NULL);
	}
};
