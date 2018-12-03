//RBSTに親へのポインタをつけたもの
//#RBST.cpp も参照。そのままコピペで使える関数はそっちにある

unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

template<typename Derived>
struct PNodeBase {
	Derived *left, *right, *parent;
	int size;
	PNodeBase(): left(NULL), right(NULL), parent(NULL), size(1) { }
	inline Derived *update() {
		size = (!left ? 0 : left->size) + 1 + (!right ? 0 : right->size);
		return derived();
	}
	inline void propagate() { }
	inline Derived *linkl(Derived *c) {
		if(left = c) c->parent = derived();
		return derived()->update();
	}
	inline Derived *linkr(Derived *c) {
		if(right = c) c->parent = derived();
		return derived()->update();
	}
	inline Derived *linklr(Derived *l, Derived *r) {
		if(left = l) l->parent = derived();
		if(right = r) r->parent = derived();
		return derived()->update();
	}
	static inline Derived *cut(Derived *t) {
		if(t) t->parent = NULL;
		return t;
	}
private:
	inline Derived *derived() { return static_cast<Derived*>(this); }
};

template<typename Node>
struct PRBSTBase {
	typedef Node *Ref;
	static int size(Ref t) { return !t ? 0 : t->size; }
	static const int MaxHeight = 32 * 4;
	static Ref join(Ref l, Ref r) {
		if(!l) return r;
		if(!r) return l;
		if((int)(xor128() % (l->size + r->size)) < l->size) {
			l->propagate();
			return l->linkr(join(Node::cut(l->right), r));
		}else {
			r->propagate();
			return r->linkl(join(l, Node::cut(r->left)));
		}
	}
	typedef pair<Ref,Ref> RefPair;
	static RefPair split(Ref t, int k) {
		if(!t) return RefPair((Ref)NULL, (Ref)NULL);
		t->propagate();
		int s = size(t->left);
		if(k <= s) {
			RefPair p = split(Node::cut(t->left), k);
			return RefPair(p.first, t->linkl(p.second));
		}else {
			RefPair p = split(Node::cut(t->right), k - s - 1);
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
			return t->linkl(insert(Node::cut(t->left), k, n));
		else
			return t->linkr(insert(Node::cut(t->right), k - s - 1, n));
	}
	static RefPair remove(Ref t, int k) {
		if(!t) return RefPair((Ref)NULL, (Ref)NULL);
		t->propagate();
		int s = size(t->left);
		if(k < s) {
			RefPair p = remove(Node::cut(t->left), k);
			return RefPair(t->linkl(p.first), p.second);
		}else if(k > s) {
			RefPair p = remove(Node::cut(t->right), k - s - 1);
			return RefPair(t->linkr(p.first), p.second);
		}else {
			Ref a = join(Node::cut(t->left), Node::cut(t->right));
			return RefPair(a, t->linklr(NULL, NULL));
		}
	}
	static int rank(Ref t) {
		int k = 0;
		while(t) {
			Ref p = t->parent;
			if(p->right == t)
				k += size(p->left) + 1;
			t = p;
		}
		return k;
	}
	static pair<Ref,int> findRoot(Ref t) {
		if(!t) return make_pair((Ref)NULL, 0);
		int k = size(t->left);
		while(true) {
			Ref p = t->parent;
			if(!p) return make_pair(t, k);
			if(p->right == t)
				k += size(p->left) + 1;
			t = p;
		}
	}
	static Ref leftest(Ref t) {
		if(!t) return NULL;
		while(t->left) t = t->left;
		return t;
	}
	template<typename Func>
	static void modify(Ref t, int k, Func f) {
		t = find(t, k);
		if(!t) return;
		f(t);
		while(t) {
			t->update();
			t = t->parent;
		}
	}
	static void debugcheckparents(Ref t, Ref p) {
		if(!t) return;
		my_assert(t->parent == p);
		debugcheckparents(t->left, t);
		debugcheckparents(t->right, t);
	}
};

