template<typename Key, typename Val> class Treap {
	static unsigned xor128() {
		static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
		unsigned t = x ^ x<<11;
		x = y; y = z; z = w;
		return w = w ^ w>>19 ^ t ^ t>>8;
	}

	struct Node {
		Node *left, *right;
		unsigned priority;
		Key key; Val val;
		Node(const Key &k, const Val &v):
			left(NULL), right(NULL), priority(xor128()),
			key(k), val(v) {}
	};
	Node *nodes; int nNodes;

	template<bool b> static inline Node *link(Node *x, Node *c) {
		if(b) {
			x->right = c;
		}else {
			x->left = c;
		}
		return x;
	}
	template<bool b> static inline Node *rotate(Node *t) {
		Node *s = b ? t->left : t->right;
		link<!b>(t, b ? s->right : s->left);
		link<b>(s, t);
		return s;
	}
	Node *insert(Node *t, const Key &k, const Val &v) {
		if(!t) return new(&nodes[nNodes++]) Node(k, v);
		if(k <= t->key) {
			t = link<false>(t, insert(t->left, k, v));
			if(t->priority > t->left->priority) t = rotate<true>(t);
		}else {
			t = link<true>(t, insert(t->right, k, v));
			if(t->priority > t->right->priority) t = rotate<false>(t);
		}
		return t;
	}

	static Val *find(Node *t, const Key &k) {
		if(!t) return NULL;
		if(k <  t->key) return find(t->left, k);
		if(k == t->key) return &t->val;
		                return find(t->right, k);
	}

	Node *root;
public:
	Treap(int n):
	  nodes((Node*)malloc(n * sizeof(Node))), nNodes(0), root(NULL) { }
	~Treap() { free(nodes); }

	void insert(const Key &k, const Val &v) { root = insert(root, k, v); }
	Val *find(const Key &k) const { return find(root, k); }
};