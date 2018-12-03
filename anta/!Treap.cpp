/*
	Treap
	以下のものは、区間への加算, 区間の反転, 区間のmin を実装している
	POJ 3580 http://poj.org/problem?id=3580 でVerifyした。
	評価遅延は、その遅延した状態で他の値が計算出来ないといけない。
	区間への加算は、sumに対して、単にadd*sizeを足すだけという感じに。
	遅延評価や情報を付け加える時は、それがそのように出来るか？をしっかり考えよう
	他の関数:
		sumのlowerbound
		ソート済みの列に対するlowerbound, insert, erase
*/

typedef int Val;
struct Node {
	int priority;
	Node *ch[2];
	//ここに欲しい情報(値のsum, 値のmin, etc.)
	//splitのためにデフォルトでsizeがある
	Val val; int size; Val mini;
	//遅延評価をするもの
	Val add; bool rev;
	//情報の初期値を設定する
	Node(Val v, int p)
		: priority(p)
		, val(v), size(1), mini(v), add(0), rev(false) { ch[0] = ch[1] = NULL; }
};
//NULL用に関数を作っておくと便利(Nodeのコンストラクタと同じく初期値に注意)
//addをvalやminiにも足している点に注意。こんなふうにできないと遅延評価はできない
Val val(Node *x) { assert(x); return x->val + x->add; }
int size(Node *x) { return !x ? 0 : x->size; }
Val mini(Node *x) { return !x ? INF : x->mini + x->add; }

Node *update(Node *x) {
	//ここで情報の更新を行う
	if(!x) return x;
	x->size = size(x->ch[0]) + 1 + size(x->ch[1]);
	x->mini = min(min(mini(x->ch[0]), mini(x->ch[1])), val(x));
	if(x->add) {
		x->val += x->add;
		if(x->ch[0]) x->ch[0]->add += x->add;
		if(x->ch[1]) x->ch[1]->add += x->add;
		x->add = 0;
	}
	if(x->rev) {
		swap(x->ch[0], x->ch[1]);
		if(x->ch[0]) x->ch[0]->rev ^= 1;
		if(x->ch[1]) x->ch[1]->rev ^= 1;
		x->rev = false;
	}
	return x;
}
Node *link(Node *x, Node *c, bool b) {
	x->ch[b] = c; return update(x);
}
Node *merge(Node *x, Node *y) {
	x = update(x); y = update(y);
	if(!x || !y) return x ? x : y;
	if(x->priority < y->priority) {
		return link(x, merge(x->ch[1], y), 1);
	}else {
		return link(y, merge(x, y->ch[0]), 0);
	}
}
//サイズによってsplitする。　([0,k) , [k,n))
typedef pair<Node*, Node*> NPair;
NPair split(Node *t, int k) {
	if(!update(t)) return mp((Node*)NULL, (Node*)NULL);
	if(size(t->ch[0]) < k) {
		NPair u = split(t->ch[1], k - size(t->ch[0]) - 1);
		return mp(link(t, u.first, 1), u.second);
	}else {
		NPair u = split(t->ch[0], k);
		return mp(u.first, link(t, u.second, 0));
	}
}
Node *singleton(Val val) { return update(new Node(val, rand())); }

//Sumに対するlowerbound
int lowerbound(Node *t, Val x) {
	if(!update(t)) return 0;
	if(sum(t->ch[0]) > x) {
		return lowerbound(t->ch[0], x);
	}else if(sum(t->ch[0]) + t->val > x) {
		return size(t->ch[0]);
	}else {
		return size(t->ch[0]) + 1 + lowerbound(t->ch[1], x - sum(t->ch[0]) - t->val);
	}
}

//minimumだけのバージョン
typedef int Val;
struct Node {
	int priority;
	Node *ch[2];
	//ここに欲しい情報(値のsum, 値のmin, etc.)
	//splitのためにデフォルトでsizeがある
	Val val; int size; Val mini;
	//情報の初期値を設定する
	Node(Val v, int p)
		: priority(p)
		, val(v), size(1), mini(v) { ch[0] = ch[1] = NULL; }
};
//NULL用に関数を作っておくと便利(Nodeのコンストラクタと同じく初期値に注意)
//addをvalやminiにも足している点に注意。こんなふうにできないと遅延評価はできない
Val val(Node *x) { assert(x); return x->val; }
int size(Node *x) { return !x ? 0 : x->size; }
Val mini(Node *x) { return !x ? INF : x->mini; }

Node *update(Node *x) {
	//ここで情報の更新を行う
	if(!x) return x;
	x->size = size(x->ch[0]) + 1 + size(x->ch[1]);
	x->mini = min(min(mini(x->ch[0]), mini(x->ch[1])), val(x));
	return x;
}

//ソート済みの列に対するlowerbound
int lowerbound(Node *t, Val x) {
	if(!update(t)) return 0;
	if(x <= val(t)) {
		return lowerbound(t->ch[0], x);
	}else {
		return size(t->ch[0]) + 1 + lowerbound(t->ch[1], x);
	}
}

//ソート済みの列に対するinsert
Node *insert(Node *t, Val x) {
	int k = lowerbound(t, x);
	NPair p = split(t, k);
	return merge(p.first, merge(singleton(x), p.second));
}

//ソート済みの列に対するerase
Node *erase(Node *t, Val x) {
	int k = lowerbound(t, x);
	NPair p = split(t, k);
	NPair q = split(p.second, 1);
	assert(val(q.first) == x);	//存在しない時にどうするかは場合によって書き換えること
	delete q.first;
	return merge(p.first, q.second);
}

//デバッグ用
ostream& operator<<(ostream& o, Node *t) {
	int n = size(t);
	o << "[";
	rep(i, n) {
		pair<Node*, Node*> p = split(t, i);
		pair<Node*, Node*> q = split(p.second, 1);
		o << val(q.first);
		if(i+1 != n) o << ", ";
		t = merge(p.first, merge(q.first, q.second));
	}
	return o << "]";
}

/*
	高速？なもの
	eraseは重いのでマイナスのinsertで済ませたらよさそう
*/

Node *link(Node *x, Node *c, const bool b) {
//	(b ? x->ch1 : x->ch0) = c; return update(x);
	if(b) {
		x->ch1 = c;
		if(c) {
			x->size = size(x->ch0) + 1 + x->ch1->size;
			x->sum = sum(x->ch0) + x->val + x->ch1->sum;
			x->dels = dels(x->ch0) + x->del + x->ch1->dels;
		}else {
			x->size = size(x->ch0) + 1;
			x->sum = sum(x->ch0) + x->val;
			x->dels = dels(x->ch0) + x->del;
		}
	}else {
		x->ch0 = c;
		if(c) {
			x->size = x->ch0->size + 1 + size(x->ch1);
			x->sum = x->ch0->sum + x->val + sum(x->ch1);
			x->dels = x->ch0->dels + x->del + dels(x->ch1);
		}else {
			x->size = 1 + size(x->ch1);
			x->sum = x->val + sum(x->ch1);
			x->dels = x->del + dels(x->ch1);
		}
	}
	return x;
}
Node *singleton(Key key, Val val, bool del) {
	return new(&mem_heap[mem_index++])Node(key, val, del, xor128());
}

inline Node *rotate(Node *t, const bool b) {
	Node *s = b ? t->ch0 : t->ch1;
	link(t, b ? s->ch1 : s->ch0, !b);
	link(s, t, b);
	return s;
}
Node *insert(Node *t, Key x, Val s, bool del) {
	if(!t) return singleton(x, s, del);
	if(keyValOrd(x,s) <= keyValOrd(t->key,t->val)) {
		t = link(t, insert(t->ch0, x, s, del), 0);
		if(t->priority > t->ch0->priority) t = rotate(t, 1);
	}else {
		t = link(t, insert(t->ch1, x, s, del), 1);
		if(t->priority > t->ch1->priority) t = rotate(t, 0);
	}
	return t;
}
//lowerboundを一度にやる
int greaterequalsum(Node *t, Key x, Val s, int& out_size) {
	Sum r = 0;
	int ord = keyValOrd(x,s);
	while(t) {
		if(ord <= keyValOrd(t->key,t->val)) {
			out_size += 1 + size(t->ch1);
			out_size -= (t->del + dels(t->ch1)) * 2;
			r += t->val + sum(t->ch1);
			t = t->ch0;
		}else {
			t = t->ch1;
		}
	}
	return r;
}


//Treap永続化バージョン (参照関係が木の形になっていないと駄目)
typedef int Key; typedef int Val;
typedef ll Sum;
struct NodeT;
typedef const NodeT* Node;
struct NodeT {
	unsigned p;
	Node l, r;
	Key k; Val v; int s; Sum u;
};

unsigned char heap_mem[300000*2*20*sizeof(NodeT)];
int heap_index = 0;
Node newNode(Key k, Val v, Node l, Node r, int s, Sum u) {
	NodeT *t = (NodeT*)&heap_mem[heap_index]; heap_index += sizeof(NodeT);
	t->p = xor128();
	t->k = k, t->v = v, t->l = l, t->r = r, t->s = s, t->u = u;
	return t;
}

int size(Node x) { return !x ? 0 : x->s; }
Sum sum(Node x) { return !x ? 0 : x->u; }

Node link(Node x, Node c, const bool b) {
	if(b) {
		if(c)
			return newNode(x->k, x->v, x->l, c, size(x->l) + 1 + c->s, sum(x->l) + x->v + c->u);
		else
			return newNode(x->k, x->v, x->l, NULL, size(x->l) + 1, sum(x->l) + x->v);
	}else {
		if(c)
			return newNode(x->k, x->v, c, x->r, c->s + 1 + size(x->r), c->u + x->v + sum(x->r));
		else
			return newNode(x->k, x->v, NULL, x->r, 1 + size(x->r), x->v + sum(x->r));
	}
}
Node rotate(Node t, const bool b) {
	Node c = b ? t->l : t->r;
	t = link(t, b ? c->l : c->r, !b);
	return link(c, t, b);
}

Node singleton(Key k, Val v) {
	return newNode(k, v, NULL, NULL, 1, v);
}

Node insert(Node t, Key k, Val v) {
	if(!t) return singleton(k, v);
	if(k <= t->k) {
		t = link(t, insert(t->l, k, v), 0);
		if(t->p > t->l->p) t = rotate(t, 1);
	}else {
		t = link(t, insert(t->r, k, v), 1);
		if(t->p > t->r->p) t = rotate(t, 0);
	}
	return t;
}

Sum gesum(Node t, Key k, int &out_size) {
	Sum u = 0;
	out_size = 0;
	while(t) {
		if(k <= t->k) {
			u += t->v + sum(t->r);
			out_size += 1 + size(t->r);
			t = t->l;
		}else {
			t = t->r;
		}
	}
	return u;
}

void getlist_rec(vector<Node>& out_v, Node t) {
	if(!t) return;
	getlist_rec(out_v, t->l);
	out_v.push_back(t);
	getlist_rec(out_v, t->r);
}

//未検証
struct Node {
	typedef int Val;
	typedef pair<Node*, Node*> NPair;

	Node *parent, *left, *right;
	unsigned priority;
	
	unsigned size;	//部分木(自分も含む)のサイズ

	Node(): parent(0), left(0), right(0), priority(xor128()) { update(); }

	static int size_(Node *t) { return !t ? 0 : t->size; }
	void update() {	//左右の子から受け取るイメージ。左右の子や自分の値を変更した後に呼ぶ必要がある。
		size = size_(left) + 1 + size_(right);
	}

	static Node *merge(Node *x, Node *y) {
		if(!x || !y) return x ? x : y;
		if(x->priority < y->priority) {
			x->right = merge(x->right, y); (x->right->parent = x)->update();
			return x;
		}else {
			y->left = merge(x, y->left); (y->left->parent = y)->update();
			return y;
		}
	}

	static NPair split(Node *t, int k) {
		if(!t) return NPair((Node*)NULL, (Node*)NULL);
		if(k <= size_(t->left)) {
			NPair u = split(t->left, k);
			if(t->left = u.second) (u.second->parent = t)->update();
			return NPair(u.first, t);
		}else {
			NPair u = split(t->right, k - size_(t->left) - 1);
			if(t->right = u.first) (u.first->parent = t)->update();
			return NPair(t, u.second);
		}
	}

	static int getPosition(Node *t) {
		if(!t->parent) return size_(t->left);
		if(t == t->parent->left)
			return getPosition(t->parent) - size_(t->right) - 1;
		else
			return getPosition(t->parent) + size_(t->left) + 1;
	}

	unsigned xor128() {
		static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
		unsigned t = x ^ (x << 11);
		x = y; y = z; z = w;
		return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
	}
};



//書きかけ


unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

struct TestVal {
	int size, x;
	TestVal(): size(0) { }
	TestVal(int x_): size(1), x(x_) { }
	inline void update(TestVal x, TestVal y) {
		size = x.size + y.size;
	}
	bool operator<(const TestVal &that) const {
		return x < that.x;
	}
	bool operator==(const TestVal &that) const {
		return x == that.x;
	}
};
ostream &operator<<(ostream &o, const TestVal &that) {
	return o << that.x;
}

struct Treap {
	static const int MaxDepth = 64;
	typedef TestVal Val;
	static const Val ZeroVal;
	struct Node {
		int size; unsigned priority;
		bool merged, propagated;
		Node *left, *right;
		Val val;
		Node(const Val &x, unsigned p):
			size(1), priority(p), merged(true), propagated(true), left(0), right(0), val(x) { }
	};
	Node *root;
	Treap(): root(NULL) { }
	static inline const Val &val(Node *x) {
		return x ? x->val : ZeroVal;
	}
	static inline Node *singleton(const Val &x) {
		return new Node(x, xor128());
	}
	void insert(int i, const Val &x) { root = insertAt(root, i, x); }
	void erase(int i, const Val &x) { root = eraseAt(root, i, x); }
private:
	static inline int size(Node *x) { return !x ? 0 : x->size; }
	static Node *insertAt(Node *t, int i, const Val &x) {
		Node *nodes[MaxDepth]; bool dirs[MaxDepth]; int k = 0;
		for(k = 0; nodes[k] = t; k ++) {
			int x = size(t->left);
			if(dirs[k] = i <= x) t = t->left;
			else t = t->right, i -= x;
		}
		nodes[k] = singleton(x);
		while(k --) {
			link(nodes[k], nodes[k+1], dirs[k]);
			if(nodes[k]->priority > nodes[k+1]->priority)
				nodes[k] = rotate(nodes[k], !dirs[k]);
		}
		return nodes[0];
	}
	static Node *eraseAt(Node *t, int i, const Val &x) {
		Node *nodes[MaxDepth]; bool dirs[MaxDepth]; int k = 0;

	}
	static inline void update(Node *x) {
		x->size = size(x->left) + size(x->right);
		x->val.update(val(x->left), val(x->right));
	}
	static inline void link(Node *x, Node *c, const bool b) {
		(b ? x->right : x->left) = c;
		update(x);
	}
	static inline Node *rotate(Node *t, const bool b) {
		Node *s = b ? t->left : t->right;
		link(t, b ? s->right : s->left, !b);
		link(s, t, b);
		return s;
	}
	static void enumerate_all(Node *t, vector<Node *> &out_v) {
		enumerate_all(t->left, out_v);
		out_v.push_back(t);
		enumerate_all(t->right, out_v);
	}
	static void debug_out(const Node *t, ostream &o) {
		if(!t) return;
		debug_out(t->left, o);
		o << t->val << " ";
		debug_out(t->right, o);
	}
	friend ostream &operator<<(ostream &o, const Treap &t) {
		t.debug_out(t.root, o); return o;
	}
};
const Treap::Val Treap::ZeroVal = Val();

int main() {
	Treap t;
	t.insert(0); cerr << t << endl;
	t.insert(2); cerr << t << endl;
	t.insert(1); cerr << t << endl;
	t.insert(2); cerr << t << endl;
	t.insert(-1); cerr << t << endl;
	t.insert(1000); cerr << t << endl;
	t.insert(2); cerr << t << endl;
	t.erase(1); cerr << t << endl;
	t.erase(1); cerr << t << endl;
	t.erase(2); cerr << t << endl;
	t.erase(2); cerr << t << endl;
	t.erase(100000); cerr << t << endl;
	t.erase(-2); cerr << t << endl;
	return 0;
}

//きちんと書いてみたもの？
struct BasicPNode {
	static unsigned xor128_x, xor128_y, xor128_z, xor128_w;
	static unsigned xor128() {
		unsigned t = xor128_x ^ (xor128_x << 11);
		xor128_x = xor128_y; xor128_y = xor128_z; xor128_z = xor128_w;
		return xor128_w = xor128_w ^ (xor128_w >> 19) ^ (t ^ (t >> 8));
	}
	BasicPNode *left, *right;
	int size;
	unsigned priority;
	BasicPNode(): left(NULL), right(NULL), size(1), priority(xor128()) { }
	inline BasicPNode *update() {
		size = (!left ? 0 : left->size) + 1 + (!right ? 0 : right->size);
		return this;
	}
	inline void propagate() { }
	inline BasicPNode *linkl(BasicPNode *c) {
		left = c;
		return update();
	}
	inline BasicPNode *linkr(BasicPNode *c) {
		right = c;
		return update();
	}
	inline BasicPNode *linklr(BasicPNode *l, BasicPNode *r) {
		left = l, right = r;
		return update();
	}
};
unsigned BasicPNode::xor128_x = 123456789, BasicPNode::xor128_y = 362436069, BasicPNode::xor128_z = 521288629, BasicPNode::xor128_w = 88675123;

struct Treap {
	typedef BasicPNode *Ref;
	static int size(Ref t) { return !t ? 0 : t->size; }
	static const int MaxHeight = 32 * 4;
	static Ref join(Ref l, Ref r) {
		if(!l) return r;
		if(!r) return l;
		if(l->priority < r->priority) {
			l->propagate();
			return l->linkr(join(l->right, r));
		}else {
			r->propagate();
			return r->linkl(join(l, r->left));
		}
	}
	typedef pair<Ref,Ref> RefPair;
	static RefPair split(Ref t, int k) {
		if(!t) return RefPair((Ref)NULL, (Ref)NULL);
		t->propagate();
		int s = size(t->left);
		if(k <= s) {
			RefPair p = split(t->left, k);
			return RefPair(p.first, t->linkl(p.second));
		}else {
			RefPair p = split(t->right, k - s - 1);
			return RefPair(t->linkr(p.first), p.second);
		}
	}
	static Ref insert(Ref t, int k, Ref n) {
		if(!t) return n;
		t->propagate();
		int s = size(t->left);
		if(k <= s) {
			t->linkl(insert(t->left, k, n));
			if(t->priority > t->left->priority) t = rotateR(t);
		}else {
			t->linkr(insert(t->right, k - s - 1, n));
			if(t->priority > t->right->priority) t = rotateL(t);
		}
		return t;
	}
	static RefPair remove(Ref t, int k) {
		if(!t) return RefPair((Ref)NULL, (Ref)NULL);
		t->propagate();
		int s = size(t->left);
		if(k < s) {
			RefPair p = remove(t->left, k);
			return RefPair(t->linkl(p.first), p.second);
		}else if(k > s) {
			RefPair p = remove(t->right, k - s - 1);
			return RefPair(t->linkr(p.first), p.second);
		}else {
			Ref a = join(t->left, t->right);
			return RefPair(a, t->linklr(NULL, NULL));
		}
	}
	static inline Ref rotateR(Ref t) {
		Ref c = t->left;
		t->linkl(c->right);
		return c->linkr(t);
	}
	static inline Ref rotateL(Ref t) {
		Ref c = t->right;
		t->linkr(c->left);
		return c->linkl(t);
	}
};
