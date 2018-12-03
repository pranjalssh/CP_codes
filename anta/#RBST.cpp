unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

template<typename Derived>
struct NodeBase {
	Derived *left, *right;
	int size;
	NodeBase(): left(NULL), right(NULL), size(1) { }
	inline Derived *update() {
		size = (!left ? 0 : left->size) + 1 + (!right ? 0 : right->size);
		return derived();
	}
	//「propagateの前後でget...系が変わらない」というふうにしたいのだけれども、revはgetLeft()とかするのが微妙なのでそうなってない
	inline void propagate() { }
	inline Derived *linkl(Derived *c) {
		left = c;
		return derived()->update();
	}
	inline Derived *linkr(Derived *c) {
		right = c;
		return derived()->update();
	}
	inline Derived *linklr(Derived *l, Derived *r) {
		left = l, right = r;
		return derived()->update();
	}
private:
	inline Derived *derived() { return static_cast<Derived*>(this); }
};

template<typename Node>
struct RBSTBase {
	typedef Node *Ref;
	static int size(Ref t) { return !t ? 0 : t->size; }
	static const int MaxHeight = 32 * 4;
	static Ref join(Ref l, Ref r) {
		if(!l) return r;
		if(!r) return l;
		if((int)(xor128() % (l->size + r->size)) < l->size) {
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
		if(xor128() % (t->size + 1) == 0) {
			RefPair p = split(t, k);
			return n->linklr(p.first, p.second);
		}
		t->propagate();
		int s = size(t->left);
		if(k <= s)
			return t->linkl(insert(t->left, k, n));
		else
			return t->linkr(insert(t->right, k - s - 1, n));
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
	static Ref find(Ref t, int k) {
		if(!t) return NULL;
		t->propagate();
		int s = size(t->left);
		if(k < s) return find(t->left, k);
		else if(k > s) return find(t->right, k - s - 1);
		else return t;
	}
	static Ref leftest(Ref t) {
		if(!t) return NULL;
		while(t->left) t = t->left;
		return t;
	}
	static Ref rightest(Ref t) {
		if(!t) return NULL;
		while(t->right) t = t->right;
		return t;
	}
	//RBSTはTreapと違って(Treapも？)構成し直すと形が変わることに注意
	template<typename Val,typename Func>
	static Val rangeQuery1(Ref &t, int l, int r, Func f) {
		if(l > r) return f((Ref)NULL);
		RefPair p = split(t, r);
		RefPair q = split(p.first, l);
		Val x = f(q.second);
		t = join(join(q.first, q.second), p.second);
		return x;
	}
	//f1はsubtree全体、f2はそのノードのみ。rangeQuery2のほうがrangeQuery1より結構速い(別個に関数書くのがいいかと思う)
	template<typename Val, typename Func1, typename Func2>
	static void rangeQuery2(Ref t, int l, int r, Val &x, const Func1 &f1, const Func2 &f2) {
		if(l > r || !t) return;
		int s = t->size;
		if(r <= 0 || s <= l) return;
		if(l <= 0 && s <= r)
			f1(x, t);
		else {
			t->propagate();
			int sl = size(t->left);
			rangeQuery2(t->left, l, min(r, sl), x, f1, f2);
			if(l <= sl && sl < r) f2(x, t);
			rangeQuery2(t->right, max(l, sl + 1) - sl - 1, r - sl - 1, x, f1, f2);
		}
	}
	template<typename Func>
	static void rangeProcess1(Ref &t, int l, int r, Func f) {
		if(l >= r) { Ref tmp = NULL; f(tmp); return; }
		RefPair p = split(t, r);
		RefPair q = split(p.first, l);
		f(q.second);
		t = join(join(q.first, q.second), p.second);
	}
	template<typename Func1, typename Func2>
	static void rangeProcess2(Ref t, int l, int r, const Func1 &f1, const Func2 &f2) {
		if(l > r || !t) return;
		int s = t->size;
		if(r <= 0 || s <= l) return;
		if(l <= 0 && s <= r)
			f1(t);
		else {
			t->propagate();
			int sl = size(t->left);
			rangeProcess2(t->left, l, min(r, sl), f1, f2);
			if(l <= sl && sl < r) f2(t);
			rangeProcess2(t->right, max(l, sl + 1) - sl - 1, r - sl - 1, f1, f2);
		}
	}
	//propagateで書き換えてしまうことに注意
	template<typename It> static It toList(Ref t, It it) {
		if(t) {
			t->propagate();
			it = toList(t->left, it);
			*it = t; ++ it;
			it = toList(t->right, it);
		}
		return it;
	}
	template<typename It> static Ref fromList(It b, It e) {
		int n = e - b;
		if(n == 0) return NULL;
		It m = b + n / 2;
		Ref l = fromList(b, m);
		Ref r = fromList(m+1, e);
		return (*m).linklr(l, r);
	}
	struct DebugOutIterator : std::iterator<std::output_iterator_tag, Node> {
		struct ToOut {
			ostream &o;
			ToOut(ostream &o_): o(o_) { }
			Ref operator=(Ref t) {
				o << *t;
				return t;
			}
		};
		ostream *o;
		DebugOutIterator(ostream &o_): o(&o_) { }
		DebugOutIterator &operator=(const DebugOutIterator &that) {
			o = that.o;
			return *this;
		}
		DebugOutIterator &operator++() {
			*o << ",";
			return *this;
		}
		ToOut operator*() { return ToOut(*o); }
	};
	static void debugout(Ref t) {
		toList(t, DebugOutIterator(cerr));
		cerr << endl;
	}
};

template<typename Value>
struct MinAddRev : NodeBase<MinAddRev<Value> > {
	typedef NodeBase<MinAddRev<Value> > Base;
	Value value, minimum, add; bool rev;
	MinAddRev(): rev(false) { }
	MinAddRev(const Value &value_): value(value_), minimum(value), add(0), rev(false) { }
	static Value getValue(const MinAddRev *t) {
		return t->value + t->add;
	}
	static Value getMinimum(const MinAddRev *t) {
		return !t ? numeric_limits<Value>::max() : t->minimum + t->add;
	}
	inline void propagate() {
		if(add) {
			value += add;
			minimum += add;
			if(Base::left) Base::left->add += add;
			if(Base::right) Base::right->add += add;
			add = 0;
		}
		if(rev) {
			swap(Base::left, Base::right);
			if(Base::left) Base::left->rev = !Base::left->rev;
			if(Base::right) Base::right->rev = !Base::right->rev;
			rev = false;
		}
		return Base::propagate();
	}
	inline MinAddRev *update() {
		minimum = min(getMinimum(Base::left), min(value, getMinimum(Base::right)));
		return Base::update();
	}
	friend ostream &operator<<(ostream &o, const MinAddRev &t) {
		return o << MinAddRev::getValue(&t);
	}
};

//<http://poj.org/problem?id=3580>
typedef MinAddRev<int> Node;
typedef RBSTBase<Node> RBST;
Node nodes[200000];
char s[10];
int glb_param;

void addToRange(Node *n) {
	if(n) n->add += glb_param;
}
void reverseRange(Node *n) {
	if(n) n->rev = !n->rev;
}
void rotateRange(Node *&n) {
	if(!n) return;
	int size = n->size;
	int mid = size - glb_param;
	RBST::RefPair p = RBST::split(n, mid);
	n = RBST::join(p.second, p.first);
}
void rangeMin1(int &x, Node *n) {
	amin(x, Node::getMinimum(n));
}
void rangeMin2(int &x, Node *n) {
	amin(x, Node::getValue(n));
}
int main() {
	int ns = 0;
	int n;
	scanf("%d", &n);
	rep(i, n) {
		int a;
		scanf("%d", &a);
		nodes[ns ++] = Node(a);
	}
	Node *t = RBST::fromList(nodes, nodes + n);
	int M;
	scanf("%d", &M);
	rep(ii, M) {
		scanf("%s", s);
		if(s[0] == 'A') {	//ADD
			int x, y;
			scanf("%d%d%d", &x, &y, &glb_param); x --;
			RBST::rangeProcess(t, x, y, addToRange);
		}else if(s[0] == 'R' && s[3] == 'E') {	//REVERSE
			int x, y;
			scanf("%d%d", &x, &y); x --;
			RBST::rangeProcess(t, x, y, reverseRange);
		}else if(s[0] == 'R' && s[3] == 'O') {	//REVOLVE
			int x, y;
			scanf("%d%d%d", &x, &y, &glb_param); x --;
			glb_param %= y - x;
			RBST::rangeProcess(t, x, y, rotateRange);
		}else if(s[0] == 'I') {	//INSERT
			int x, P;
			scanf("%d%d", &x, &P);
			Node *p = &nodes[ns ++];
			*p = Node(P);
			t = RBST::insert(t, x, p);
		}else if(s[0] == 'D') {	//DELETE
			int x;
			scanf("%d", &x); x --;
			t = RBST::remove(t, x).first;
		}else if(s[0] == 'M') {	//MIN
			int x, y;
			scanf("%d%d", &x, &y); x --;
			int mini = 0x7fffffff;
			RBST::rangeQuery2(t, x, y, mini, rangeMin1, rangeMin2);
			printf("%d\n", mini);
		}
	}
	return 0;
}
