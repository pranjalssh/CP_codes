//TODO: split

//参考: <http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.109.4875&rep=rep1&type=pdf>
//CGALのMultiset

#include "C:\test\data1\dropbox\Dropbox\backup\implements\Util\MyAssert.hpp"
struct RedBlackTree {
	typedef int Index;
	struct Node {
		Node *left, *right, *parent;
		Index size;
		bool color;	//false: black, true: red

		bool isBlack() const { return !color; }
		bool isRed() const { return color; }
		void colorBlack() { color = false; }
		void colorRed() { color = true; }
	};
	Node *root;
	int blackHeight;

	RedBlackTree(): root(NULL), blackHeight(0) { }
	~RedBlackTree() { destroy(root); }

	struct iterator : public std::iterator<std::bidirectional_iterator_tag, Node> {
		Node *node;
		iterator(): node(NULL) { }
		iterator(Node *n): node(n) { }
		iterator &operator++() { node = successor(node); return *this; }
		iterator &operator--() { node = predecessor(node); return *this; }
		bool operator==(iterator that) const { return node == that.node; }
		bool operator!=(iterator that) const { return node != that.node; }
		Node &operator*() { return *node; }
	};
	iterator begin() { return iterator(leftest(root)); }
	iterator end() { return iterator(); }

	void insert(Index index, Node *n) {
		insert(root, index, n);
	}
	void erase(Index index) {
		erase(iterator(findPos(root, index)));
	}
	void erase(iterator it) {
		erase(it.node);
	}
	bool empty() const { return !root; }
	Index size() const { return getSize(root); }

	static Node *newNode() {
		Node *n = new Node;
		n->left = n->right = n->parent = NULL;
		n->size = 1;
		n->colorRed();
		return n;
	}
	static void deleteNode(Node *node) {
		delete node;
	}
	static void destroy(Node *node) {
		if(!node) return;
		destroy(node->left);
		destroy(node->right);
		deleteNode(node);
	}

	void debugcheck() const {
		my_assert(!root || root->isBlack());
		debugcheck(root, NULL, 0);
	}
	int debugcheck(const Node *node, const Node *parent, int blacks) const {
		if(!node) {
			my_assert(blacks == blackHeight);
			return 0;
		}
		my_assert(node->parent == parent);
		if(node->isRed()) {
			bool left = !node->left || node->left->isBlack();
			bool right = !node->right || node->right->isBlack();
			my_assert(left && right);
		}
		int nblacks = blacks + node->isBlack();
		int size = 1;
		size += debugcheck(node->left, node, nblacks);
		size += debugcheck(node->right, node, nblacks);
		my_assert(node->size == size);
		return size;
	}

	ostream &debugoutnode(ostream &o, Node *parent, bool childindex, Node *node) const {
		if(node) return o << '"' << node << '"';
		else return o << '"' << parent << node << childindex << '"';
	}
	ostream &debugoutdownedge(ostream &o, Node *parent, bool childindex, Node *x, Node *y) const {
		debugoutnode(o, parent, false, x) << " -> ";
		debugoutnode(o, x, childindex, y) << ";" << endl;
		return o;
	}
	ostream &debugoutupedge(ostream &o, Node *parent, bool childindex, Node *x, Node *y) const {
		debugoutnode(o, y, childindex, x) << " -> ";
		debugoutnode(o, parent, false, y) << ";" << endl;
		return o;
	}
	void debugout(ostream &o, Node *parent, bool childindex, Node *node) const {
		if(!node) {
			debugoutnode(o, parent, childindex, node) << " [";
			o << "label=" << '"' << "NIL" << '"' << ",";
			o << "style=filled,color=black,\nfontcolor=white,shape=rect,fontsize=10,fixedsize=true,height=0.2,width=0.45";
			o << "];" << endl;
			return;
		}
		debugoutnode(o, parent, false, node) << " [";
		o << "label=" << '"' << node << '"' << ",";
		o << "style=filled,";
		if(node->isBlack())
			o << "color=black,fontcolor=white";
		else
			o << "color=red,fontcolor=white";
		o << "];" << endl;

		if(node->parent)
			debugoutupedge(o, parent, childindex, node, node->parent);

		debugout(o, node, false, node->left);
		debugoutdownedge(o, parent, false, node, node->left);

		debugout(o, node, true, node->right);
		debugoutdownedge(o, parent, true, node, node->right);
	}
	void debugout(ostream &o) const {
		o << "digraph {" << endl;
		debugout(o, NULL, false, root);
		o << "}" << endl;
	}
	void debugout_e() const {
		debugout(cerr);
	}

	static Node *leftest(Node *node) {
		if(!node) return NULL;
		while(node->left) node = node->left;
		return node;
	}
	static Node *rightest(Node *node) {
		if(!node) return NULL;
		while(node->right) node = node->right;
		return node;
	}
	static Node *successor(Node *node) {
		if(!node) return NULL;
		if(node->right) return leftest(node->right);
		Node *prev = node; node = node->parent;
		while(node && node->right == prev) {
			prev = node;
			node = node->parent;
		}
		return node;
	}
	static Node *predecessor(Node *node) {
		if(!node) return NULL;
		if(node->left) return rightest(node->left);
		Node *prev = node; node = node->parent;
		while(node && node->left == prev) {
			prev = node;
			node = node->parent;
		}
		return node;
	}

	static Index getSize(const Node *node) {
		return !node ? 0 : node->size;
	}
	static bool isBlack(const Node *node) {
		return !node || node->isBlack();
	}
	static bool isRed(const Node *node) {
		return node && node->isRed();
	}

	static pair<Node*,bool> findInsertPos(Node *node, Index index) {
		if(!node) return make_pair((Node*)NULL, false);
		while(true) {
			Index leftSize = getSize(node->left);
			Node *next;
			if(index <= leftSize) {
				if(!(next = node->left))
					return make_pair(node, false);
			}else {
				if(!(next = node->right))
					return make_pair(node, true);
				index -= leftSize + 1;
			}
			node = next;
		}
	}

	void insert(Node *node, Index index, Node *n) {
		pair<Node*,bool> pos = findInsertPos(node, index);
		n->left = n->right = NULL;
		n->colorRed();
		if(pos.first)
			(pos.second ? pos.first->left : pos.first->right) = n;
		link(pos.first, n);
		insert_fixup(n);
		bottomup(n->parent);
	}

	void insert_fixup(Node *n) {
		Node *p = n->parent;
		if(!p) {	//Case 1
			n->colorBlack();
			blackHeight ++;
			root = n;
			return;
		}else if(p->isBlack()) {	//Case 2
			
		}else {	//Case 3
			Node *g = p->parent, *u;
			u = g->left == p ? g->right : g->left;
			if(u && u->isRed()) {
				u->colorBlack(); p->colorBlack();
				g->colorRed();
				update(p); update(g);
				return insert_fixup(g);
			}else {	//Case 4
				Node *t = n;
				if(p->right == n && g->left == p) {
					rotate_left(n);
					t = p;
				}else if(p->left == n && g->right == p) {
					rotate_right(n);
					t = p;
				}
				//Case 5
				Node *q = t->parent, *h = q->parent;
				q->colorBlack();
				h->colorRed();
				if(q->left == t)
					rotate_right(q);
				else
					rotate_left(q);
			}
		}
	}

	static Node *findPos(Node *node, Index index) {
		while(node) {
			Index leftSize = getSize(node->left);
			if(index < leftSize) {
				node = node->left;
			}else if(index == leftSize) {
				break;
			}else {
				node = node->right;
				index -= leftSize + 1;
			}
		}
		return node;
	}
	void erase(Node *n) {
		if(!n) return;
		Node *p = n->parent, *top = p;
		int lr = !p ? 0 : p->left == n ? -1 : 1;
		if(n->left && n->right) {
			Node *t = n->left->size >= n->right->size ? rightest(n->left) : leftest(n->right);
			Node *p = n->parent, *q = t->parent;
			if(p) (p->left == n ? p->left : p->right) = t;
			else root = t;
			if(q != n) {
				(q->left == t ? q->left : q->right) = n;
				swap(n->parent, t->parent);
			}else {
				t->parent = n->parent;
				n->parent = t;
			}
			swap(n->left, t->left);
			if(n->left) n->left->parent = n;
			if(t->left == t) t->left = n;
			else if(t->left) t->left->parent = t;
			swap(n->right, t->right);
			if(n->right) n->right->parent = n;
			if(t->right == t) t->right = n;
			else if(t->right) t->right->parent = t;
			swap(n->color, t->color);
			update(n);
			return erase(n);
		}else {
			Node *c = n->left ? n->left : n->right;
			if(lr == -1) p->left = c;
			else if(lr == 1) p->right = c;
			else root = c;
			link(p, c);
			if(n->isBlack()) {
				if(lr == 0) blackHeight --;
				erase_fixup(c, p);
			}
		}
		deleteNode(n);
		bottomup(top);
	}
	void erase_fixup(Node *n, Node *p) {
		if(isRed(n)) {
			n->colorBlack();
			if(n == root) blackHeight ++;
			return;
		}else if(!p) {
			if(n) blackHeight --;
			return;
		}
		int lr = n == p->left ? -1 : 1;
		Node *s = lr < 0 ? p->right : p->left;
		if(isRed(s)) {
			s->colorBlack();
			p->colorRed();
			if(lr < 0)
				rotate_left(s);
			else
				rotate_right(s);
		}
		Node *t = lr < 0 ? p->right : p->left;
		my_assert(t != NULL);
		if(isBlack(t->left) && isBlack(t->right)) {
			t->colorRed();
			return erase_fixup(p, p->parent);
		}
		if(isBlack(lr < 0 ? t->right : t->left)) {
			t->colorRed();
			if(lr < 0) {
				t->left->colorBlack();
				rotate_right(t->left);
			}else {
				t->right->colorBlack();
				rotate_left(t->right);
			}
		}
		Node *u = lr < 0 ? p->right : p->left;
		u->color = p->color;
		p->colorBlack();
		if(lr < 0) {
			if(u->right) u->right->colorBlack();
			rotate_left(u);
		}else {
			if(u->left) u->left->colorBlack();
			rotate_right(u);
		}
	}

	void merge(RedBlackTree &that) {
		Node *mid1 = rightest(root), *mid2 = leftest(that.root);
		if(!mid2) return;
		if(!mid1) {
			swap(root, that.root);
			swap(blackHeight, that.blackHeight);
			return;
		}
		//真ん中のノードのどちらかを切り出してくる
		Node *a = NULL;
		if(mid1 != root) {
			mid1->parent->right = mid1->left;
			link(mid1->parent, mid1->left);
			if(mid1->isBlack())
				erase_fixup(mid1->left, mid1->parent);
			a = mid1;
			bottomup(mid1->parent);
		}else if(mid2 != that.root) {
			mid2->parent->left = mid2->right;
			link(mid2->parent, mid2->right);
			if(mid2->isBlack())
				that.erase_fixup(mid2->right, mid2->parent);
			a = mid2;
			bottomup(mid2->parent);
		}else {
			root->right = mid2;
			link(root, mid2);
			mid2->colorRed();
			Node *r = mid2->right;
			if(r) {
				insert_fixup(r);
				bottomup(r->parent);
			}
			that.root = NULL;
			that.blackHeight = 0;
			return;
		}
		//blackHeightがつり合うまで降りてく
		Node *node1 = root, *node2 = that.root;
		if(blackHeight <= that.blackHeight) {
			int h = that.blackHeight;
			while(h > blackHeight) {
				if(node2->isBlack()) h --;
				node2 = node2->left;
			}
			if(node2->isRed()) node2 = node2->left;
			my_assert(node2 != NULL);
		}else {
			int h = blackHeight;
			while(h > that.blackHeight) {
				if(node1->isBlack()) h --;
				node1 = node1->right;
			}
			if(node1->isRed()) node1 = node1->right;
			my_assert(node1 != NULL);
		}
		Node *newroot = NULL, *p = NULL;
		if(node1 == root && node2 == that.root) {	//同じ高さだった場合
			newroot = a;
		}else if(node1 == root) {	//右のほうが高かった場合
			p = node2->parent;
			newroot = that.root;
			p->left = a;
		}else {						//左のほうが高かった場合
			p = node1->parent;
			newroot = root;
			p->right = a;
		}
		//切り出してきたaを仲介して繋ぐようにする
		a->parent = p;
		a->colorRed();
		a->left = node1;
		a->right = node2;
		node1->parent = a;
		node2->parent = a;

		if(root != newroot) {
			blackHeight = that.blackHeight;
			root = newroot;
		}

		insert_fixup(a);
		bottomup(node1);

		that.root = NULL;
		that.blackHeight = 0;
	}

	void split(Index index, RedBlackTree &res) {
		
	}

	static inline void link(Node *p, Node *c) {
		if(c) c->parent = p;
		if(p) update(p);
	}
	void rotate_left(Node *n) {
		Node *p = n->parent, *g = p->parent, *l = n->left;
		my_assert(!g || g->left == p || g->right == p);
		my_assert(p->right == n);
		if(g) (g->left == p ? g->left : g->right) = n;
		else root = n;
		link(g, n);
		p->right = l; link(p, l);
		n->left = p; link(n, p);
	}
	void rotate_right(Node *n) {
		Node *p = n->parent, *g = p->parent, *r = n->right;
		my_assert(!g || g->left == p || g->right == p);
		my_assert(p->left == n);
		if(g) (g->left == p ? g->left : g->right) = n;
		else root = n;
		link(g, n);
		p->left = r; link(p, r);
		n->right = p; link(n, p);
	}

	static inline void update(Node *n) {
		n->size = getSize(n->left) + 1 + getSize(n->right);
	}

	static inline void bottomup(Node *n) {
		while(n) {
			update(n);
			n = n->parent;
		}
	}
};

unsigned xor128() {
	static unsigned x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

#include "../Util/TimeIt_Simple.hpp"
int main() {
	double t = -1e99;
	rep(ii, 10000) {
		if(getTime() - t >= 1) {
			cerr << "\r" << ii << "...", cerr.flush();
			t = getTime();
		}
#if 0
		RedBlackTree rbt;
		rep(i, 30) {
			bool insertorerase = !rbt.empty() && xor128() % 10 < 4;
			if(!insertorerase) {
				int x = xor128() % (rbt.size()+1);
				rbt.insert(x, rbt.newNode());
			}else {
				int x = xor128() % rbt.size();
				rbt.erase(x);
			}
			rbt.debugcheck();
//			rbt.debugout_e();
		}
#elif 1
		RedBlackTree rbt1, rbt2;
		rep(i, 30) {
			bool op = xor128() % 10 < 1;
			if(!op) {
				bool b = xor128() % 2 == 0;
				RedBlackTree &rbt = !b ? rbt1 : rbt2;
				int x = xor128() % (rbt.size() + 1);
				rbt.insert(x, rbt.newNode());
			}else {
				bool b = xor128() % 2 == 0;
				RedBlackTree &rbtx = !b ? rbt1 : rbt2;
				RedBlackTree &rbty = !b ? rbt2 : rbt1;
				int xysize = rbtx.size() + rbty.size();
				rbtx.merge(rbty);
				my_assert(rbtx.size() == xysize);
			}
			rbt1.debugcheck();
			rbt2.debugcheck();
		}
#endif
	}
	RedBlackTree().debugout_e();

	return 0;
}
