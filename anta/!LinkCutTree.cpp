/*
	evert+遅延min更新
	遅延更新の例として
	[http://jag2013spring.contest.atcoder.jp/submissions/78113]
*/

typedef ll Weight;
const Weight Inf = INFL;
struct Node {
	Node *parent, *left, *right;
	Weight weight, minassign;
	bool rev;
 
	Node(): parent(NULL), left(NULL), right(NULL),
		weight(Inf), minassign(Inf), rev(false) { update(); }
 
	bool is_root() const {
		return !parent || (parent->left != this && parent->right != this);
	}
	void update() {
	}
	void push() {
		if(rev) {
			if(left) left->rev ^= 1;
			if(right) right->rev ^= 1;
			swap(left, right);
			rev = false;
		}
		if(minassign != Inf) {
			weight = min(weight, minassign);
			if(left) left->minassign = min(left->minassign, minassign);
			if(right) right->minassign = min(right->minassign, minassign);
			minassign = Inf;
		}
	}
	void rotateR() {
		push();
		Node *q = parent, *r = q->parent;
		if(q->left = right) right->parent = q;
		right = q; q->parent = this;
		if(parent = r) {
			if(r->left == q) r->left = this;
			if(r->right == q) r->right = this;
		}
		q->update(); update();
	}
	void rotateL() {
		push();
		Node *q = parent, *r = q->parent;
		if(q->right = left) left->parent = q;
		left = q; q->parent = this;
		if(parent = r) {
			if(r->left == q) r->left = this;
			if(r->right == q) r->right = this;
		}
		q->update(); update();
	}
	void topdown() {
		static vector<Node*> route;
		for(Node *q = this; ; q = q->parent) {
			route.push_back(q);
			if(q->is_root()) break;
		}
		for(int i = route.size()-1; i >= 0; i --) {
			Node *q = route[i];
			q->push();
		}
		route.clear();
	}
	void splay() {
		topdown();
		while(!is_root()) {
			Node *q = parent;
			if(q->is_root()) {
				if(q->left == this) rotateR();
				else rotateL();
			}else {
				Node *r = q->parent;
				if(r->left == q) {
					if(q->left == this) q->rotateR(), rotateR();
					else rotateL(), rotateR();
				}else {
					if(q->right == this) q->rotateL(), rotateL();
					else rotateR(), rotateL();
				}
			}
		}
	}
};

void expose(Node *x) {
	Node *rp = NULL;
	for(Node *p = x; p; p = p->parent) {
		p->splay();
		p->right = rp;
		p->update();
		rp = p;
	}
	x->splay();
}
void evert(Node *x) {	//xをルートノードにする
	expose(x);
	x->right = NULL;
	x->update();
	x->rev ^= true;
}
void cut(Node *c) {
	expose(c);
	Node *p = c->left;
	assert(p);
	c->left = NULL;
	c->update();
	p->parent = NULL;
}
void connect(Node *x, Node *y) {
	evert(x);
	x->parent = y;
}

struct UnionFind {
	vector<int> data;
	UnionFind(int size_) : data(size_, -1) { }
	bool unionSet(int x, int y) {
		x = root(x); y = root(y);
		if (x != y) {
			if (data[y] < data[x]) swap(x, y);
			data[x] += data[y]; data[y] = x;
		}
		return x != y;
	}
	bool findSet(int x, int y) { return root(x) == root(y); }
	int root(int x) { return data[x] < 0 ? x : data[x] = root(data[x]); }
	int size(int x) { return -data[root(x)]; }
};

struct Edge {
	int i, a, b;
	Weight w;
	Edge() {}
	Edge(int i_, int a_, int b_, Weight w_): i(i_), a(a_), b(b_), w(w_) {}
};
bool operator<(const Edge &x, const Edge &y) {
	return x.w < y.w;
}

static vector<Node> *g_nodes, *g_edges;
string showVertex(const pair<const Node*, Weight> &p) {
	const Node *x = p.first;
	stringstream ss;
	if(g_nodes->data() <= x && x < g_nodes->data()+g_nodes->size())
		ss << x - g_nodes->data();
	else {
		ss << '"'<< x-g_edges->data()<<'(';
		if(p.second == INFL) ss << "∞";
		else ss << p.second;
		ss << ')' << '"';
	}
	return ss.str();
}
void flattenNode(Node *x, bool rev, Weight minassign, vector<pair<const Node*,Weight> > &out) {
	if(!x) return;
	rev ^= x->rev;
	minassign = min(minassign, x->minassign);
	flattenNode(!rev ? x->left : x->right, rev, minassign, out);
	out.push_back(mp(x, min(minassign, x->weight)));
	flattenNode(!rev ? x->right : x->left, rev, minassign, out);
}
void visualize() {
	vector<Node> &nodes = *g_nodes, &edges = *g_edges;
	cerr << "http://chart.apis.google.com/chart?cht=gv:dot&chl=digraph{";
	vector<Node*> ps;
	rep(i, nodes.size()) ps.push_back(&nodes[i]);
	rep(i, edges.size()) ps.push_back(&edges[i]);
	rep(i, ps.size()) if(ps[i]->is_root()) {
		vector<pair<const Node*,Weight> > v;
		flattenNode(ps[i], false, Inf, v);
		rep(j, v.size()-1) {
			cerr << showVertex(v[j]) << "->" << showVertex(v[j+1]) << ";";
			cerr << showVertex(v[j+1]) << "->" << showVertex(v[j]) << ";";
		}
		if(ps[i]->parent)
			cerr << showVertex(mp(ps[i]->parent, min(ps[i]->parent->minassign, ps[i]->parent->weight))) << "->" << showVertex(v[0]) << ";";
	}
	cerr << "}" << endl;
}



/*
	evertができるバージョン
	反転できるボトムアップSplay木としても使える
*/


struct Node {
	Node *parent, *left, *right;
	bool rev;

	Node(): parent(NULL), left(NULL), right(NULL),
		rev(false) { update(); }
 
	bool is_root() const {
		return !parent || (parent->left != this && parent->right != this);
	}
	void update() {	//自分の値かleftかrightが変わった時に呼べばいい
	}
	void reverse() {	//ボトムアップなので、一々上まで見てからやる必要があることに注意
		if(left) left->rev ^= 1;
		if(right) right->rev ^= 1;
		swap(left, right);
		rev ^= true;
	}
	void rotateR() {
		Node *q = parent, *r = q->parent;
		if(q->left = right) right->parent = q;
		right = q; q->parent = this;
		if(parent = r) {
			if(r->left == q) r->left = this;
			if(r->right == q) r->right = this;
		}
		q->update(); update();
	}
	void rotateL() {
		Node *q = parent, *r = q->parent;
		if(q->right = left) left->parent = q;
		left = q; q->parent = this;
		if(parent = r) {
			if(r->left == q) r->left = this;
			if(r->right == q) r->right = this;
		}
		q->update(); update();
	}
	void splay() {
		bool revf = false;
		for(Node *q = this; ; q = q->parent) {
			revf ^= q->rev;
			if(q->is_root()) break;
		}
		for(Node *q = this; ; q = q->parent) {
			if(revf) {
				revf ^= q->rev;
				q->reverse();
			}else
				revf ^= q->rev;
			if(q->is_root()) break;
		}
		while(!is_root()) {
			Node *q = parent;
			if(q->is_root()) {
				if(q->left == this) rotateR();
				else rotateL();
			}else {
				Node *r = q->parent;
				if(r->left == q) {
					if(q->left == this) q->rotateR(), rotateR();
					else rotateL(), rotateR();
				}else {
					if(q->right == this) q->rotateL(), rotateL();
					else rotateR(), rotateL();
				}
			}
		}
	}
};

//頂点にアクセスする場合は必ずこれを実行する必要がある
void expose(Node *x) {
	Node *rp = NULL;
	for(Node *p = x; p; p = p->parent) {
		p->splay();
		p->right = rp;
		p->update();
		rp = p;
	}
	x->splay();
}
void evert(Node *x) {	//xをルートノードにする
	expose(x);
	x->right = NULL;
	x->update();
	x->rev ^= true;
}
void cut(Node *c) {
	expose(c);
	Node *p = c->left;
	assert(p);
	c->left = NULL;
	c->update();
	p->parent = NULL;
}
//向きの決まっていないlink
void connect(Node *x, Node *y) {
	evert(x);
	x->parent = y;
}

//	[http://ijpc2012-3.contest.atcoder.jp/submissions/76497]

typedef int Weight;
const Weight MinusInf = -INF;
struct Node {
	Node *parent, *left, *right;
	Weight weight;	//自分の親(1つ左/parent)への辺
	Node *maxnode;
	bool rev;
 
	Node(): parent(NULL), left(NULL), right(NULL),
		weight(MinusInf), rev(false) { update(); }
 
	static Weight get_maxweight(Node *t) { return !t ? MinusInf : t->maxnode->weight; }
	bool is_root() const {
		return !parent || (parent->left != this && parent->right != this);
	}
	void update() {	//自分の値かleftかrightが変わった時に呼べばいい
		maxnode = this;
		if(maxnode->weight < get_maxweight(left))
			maxnode = left->maxnode;
		if(maxnode->weight < get_maxweight(right))
			maxnode = right->maxnode;
	}
	void reverse() {	//ボトムアップなので、一々上まで見てからやる必要があることに注意
		if(left) left->rev ^= 1;
		if(right) right->rev ^= 1;
		swap(left, right);
		rev ^= true;
	}
	void rotateR() {
		Node *q = parent, *r = q->parent;
		if(q->left = right) right->parent = q;
		right = q; q->parent = this;
		if(parent = r) {
			if(r->left == q) r->left = this;
			if(r->right == q) r->right = this;
		}
		q->update(); update();
	}
	void rotateL() {
		Node *q = parent, *r = q->parent;
		if(q->right = left) left->parent = q;
		left = q; q->parent = this;
		if(parent = r) {
			if(r->left == q) r->left = this;
			if(r->right == q) r->right = this;
		}
		q->update(); update();
	}
	void splay() {
		bool revf = false;
		for(Node *q = this; ; q = q->parent) {
			revf ^= q->rev;
			if(q->is_root()) break;
		}
		for(Node *q = this; ; q = q->parent) {
			if(revf) {
				revf ^= q->rev;
				q->reverse();
			}else
				revf ^= q->rev;
			if(q->is_root()) break;
		}
		while(!is_root()) {
			Node *q = parent;
			if(q->is_root()) {
				if(q->left == this) rotateR();
				else rotateL();
			}else {
				Node *r = q->parent;
				if(r->left == q) {
					if(q->left == this) q->rotateR(), rotateR();
					else rotateL(), rotateR();
				}else {
					if(q->right == this) q->rotateL(), rotateL();
					else rotateR(), rotateL();
				}
			}
		}
	}
};
 
static vector<Node> *g_nodes, *g_edges;
string showVertex(const Node *x) {
	stringstream ss;
	if(g_nodes->data() <= x && x < g_nodes->data()+g_nodes->size())
		ss << x - g_nodes->data();
	else
		ss << '"'<< x-g_edges->data()<<'(' << x->weight << ')' << '"';
	return ss.str();
}
void flattenNode(Node *x, bool rev, vector<const Node*> &out) {
	if(!x) return;
	rev ^= x->rev;
	flattenNode(!rev ? x->left : x->right, rev, out);
	out.push_back(x);
	flattenNode(!rev ? x->right : x->left, rev, out);
}
void visualize() {
	vector<Node> &nodes = *g_nodes, &edges = *g_edges;
	cerr << "http://chart.apis.google.com/chart?cht=gv:dot&chl=digraph{";
	vector<Node*> ps;
	rep(i, nodes.size()) ps.push_back(&nodes[i]);
	rep(i, edges.size()) ps.push_back(&edges[i]);
	rep(i, ps.size()) if(ps[i]->is_root()) {
		vector<const Node*> v;
		flattenNode(ps[i], false, v);
		rep(j, v.size()-1) {
			cerr << showVertex(v[j]) << "->" << showVertex(v[j+1]) << ";";
			cerr << showVertex(v[j+1]) << "->" << showVertex(v[j]) << ";";
		}
		if(ps[i]->parent)
			cerr << showVertex(ps[i]->parent) << "->" << showVertex(v[0]) << ";";
	}
	cerr << "}" << endl;
}
 
//頂点にアクセスする場合は必ずこれを実行する必要がある
void expose(Node *x) {
	Node *rp = NULL;
	for(Node *p = x; p; p = p->parent) {
		p->splay();
		p->right = rp;
		p->update();
		rp = p;
	}
	x->splay();
}
void evert(Node *x) {	//xをルートノードにする
	expose(x);
	x->right = NULL;
	x->update();
	x->rev ^= true;
}
/*
//向きの決まっているlink
void link(Node *c, Node *p) {
	expose(c); expose(p);
	assert(c->parent == NULL);
	c->parent = p;
	c->update();
	p->right = c;
	p->update();
}
*/
//こちらも向きが決まっている
void cut(Node *c) {
	expose(c);
	Node *p = c->left;
	assert(p);
	c->left = NULL;
	c->update();
	p->parent = NULL;
}
//向きの決まっていないlink
void connect(Node *x, Node *y) {
	evert(x);
	x->parent = y;
}
struct Query {
	int v, u;	//v < u
	Weight w;
};
bool operator<(const Query &p, const Query &q) {
	return p.u < q.u;
}
 
struct UnionFind {
	vector<int> data;
	UnionFind(int size_) : data(size_, -1) { }
	bool unionSet(int x, int y) {
		x = root(x); y = root(y);
		if (x != y) {
			if (data[y] < data[x]) swap(x, y);
			data[x] += data[y]; data[y] = x;
		}
		return x != y;
	}
	bool findSet(int x, int y) { return root(x) == root(y); }
	int root(int x) { return data[x] < 0 ? x : data[x] = root(data[x]); }
	int size(int x) { return -data[root(x)]; }
};
 
 
 
void construct(int N, int M, int E[][3]) {
	vector<Query> querys(M);
	rep(i, M) querys[i].v = E[i][0], querys[i].u = E[i][1], querys[i].w = E[i][2];
	sort(all(querys));
 
	vector<Node> nodes(N), edges(M);
	g_nodes = &nodes, g_edges = &edges;
	//エッジを1つのノードとして扱うと楽
	//edges[i]はquerys[i].(v/u)を親や子にもつ(evertによってそれは変わることに注意)
 
	UnionFind uf(N);
 
	vector<long long> ans(N, INFL);
	long long sum = 0;
	rep(i, M) {
		int v = querys[i].v, u = querys[i].u;
		Weight w = querys[i].w;
		edges[i].weight = w;
//		cerr << i << ": " << v << ", " << u << ", " << w << endl;
 
		if(uf.findSet(v, u)) {
			evert(&nodes[v]); expose(&nodes[u]);
			Node *a = nodes[u].left->maxnode;
			if(a->weight > w) {
				sum -= a->weight;
				sum += w;
				cut(a);
				evert(&nodes[u]);	//左右の頂点をevertすることによって辺の両側を切れる
				cut(a);
				connect(&edges[i], &nodes[v]);
				connect(&edges[i], &nodes[u]);
			}
		}else {
			connect(&edges[i], &nodes[v]);
			connect(&edges[i], &nodes[u]);
			sum += w;
		}
		uf.unionSet(v, u);
		if(uf.size(u) == u+1)
			ans[u] = min(ans[u], sum);
 
//		visualize();
	}
	reu(i, 1, N) answer(ans[i] == INFL ? -1 : ans[i]);
}



/*
	向きが決まってるバージョン
*/

struct Node {
	Node *parent, *left, *right;
	bool is_root() const {
		return !parent || (parent->left != this && parent->right != this);
	}
	void rotateR() {
		Node *q = parent, *r = q->parent;
		if(q->left = right) right->parent = q;
		right = q; q->parent = this;
		if(parent = r) {
			if(r->left == q) r->left = this;
			if(r->right == q) r->right = this;
		}
	}
	void rotateL() {
		Node *q = parent, *r = q->parent;
		if(q->right = left) left->parent = q;
		left = q; q->parent = this;
		if(parent = r) {
			if(r->left == q) r->left = this;
			if(r->right == q) r->right = this;
		}
	}
	void splay() {
		while(!is_root()) {
			Node *q = parent;
			if(q->is_root()) {
				if(q->left == this) rotateR();
				else rotateL();
			}else {
				Node *r = q->parent;
				if(r->left == q) {
					if(q->left == this) q->rotateR(), rotateR();
					else rotateL(), rotateR();
				}else {
					if(q->right == this) q->rotateL(), rotateL();
					else rotateR(), rotateL();
				}
			}
		}
	}
	Node *head() {
		splay();
		Node *t = this;
		while(t->left) t = t->left;
		t->splay();
		return t;
	}
};
Node *expose(Node *x) {
	Node *rp = NULL;
	for(Node *p = x; p; p = p->parent) {
		p->splay();
		p->right = rp;
		rp = p;
	}
	x->splay();
	return x;
}
void cut(Node *c) {
	expose(c);
	Node *p = c->left;
	c->left = NULL;
	p->parent = NULL;
}
void link(Node *c, Node *p) {
	expose(c); expose(p);
	c->parent = p;
	p->right = c;
}
Node *lca(Node *a, Node *b) {
	expose(a);
	Node *ah = a->head();
	expose(b);
	Node *bh = b->head();
	if(ah != bh) return NULL;
	while(1) {
		a->splay();
		if(!a->parent) return a;
		a = a->parent;
	}
}











