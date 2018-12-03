//#PRBST.cppから適切にコピペする必要がある。
//それか<http://joisc2013-day4.contest.atcoder.jp/submissions/149693>をコピペすればよい。

template<typename Value>
struct MinRefAdd : PNodeBase<MinRefAdd<Value> > {
	typedef PNodeBase<MinRefAdd<Value> > Base;
	Value value, minimum, add; MinRefAdd *minRef;
	MinRefAdd() { }
	//minRefにthisを入れると代入演算子とかで一時オブジェクトを指したままになってしまう…手動でupdateしてねということで
	MinRefAdd(const Value &value_):	
		value(value_), minimum(numeric_limits<Value>::max()), add(0), minRef(NULL) { }
	static Value getValue(const MinRefAdd *t) {
		return t->value + t->add;
	}
	static Value getMinimum(const MinRefAdd *t) {
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
		return Base::propagate();
	}
	inline MinRefAdd *update() {
		Value minL = getMinimum(Base::left), minR = getMinimum(Base::right);
		minimum = value, minRef = this;
		if(minimum > minL) minimum = minL, minRef = Base::left->minRef;
		if(minimum > minR) minimum = minR, minRef = Base::right->minRef;
		return Base::update();
	}
};

//本来のeuler tourの名前のように辺を2回通ることを考える。辺ごとに頭の頂点を記録するとする。根だけは特別に最初に記録する。
//これは実際にはlefts,rightsと考えても同じである。[left,right)の半開区間であるとみなせる。
struct EulerTourTreeLCA {
	typedef MinRefAdd<int> Node;
	typedef PRBSTBase<Node> RBST;
	//各頂点の親への辺に対して、下向きに来た時と上向きに帰る時の記録のノード
	//ただし、根は仮想的な辺が存在しているとする
	Node *downs, *ups; int *parents; int n;
	EulerTourTreeLCA(): downs(NULL), ups(NULL), parents(NULL), n(0) { }
	~EulerTourTreeLCA() { delete[] downs; delete[] ups; delete[] parents; }
	void init(int n_) {
		delete[] downs; delete[] ups; delete[] parents;
		n = n_;
		downs = new Node[n]; ups = new Node[n];
		parents = new int[n];
		for(int i = 0; i < n; i ++) {
			downs[i] = Node(0);
			ups[i] = Node(-1);
			downs[i].update(); ups[i].update();
			RBST::join(&downs[i], &ups[i]);
			parents[i] = -1;
		}
	}
	Node *link(int p, int c) {
		assert(parents[c] == -1);
		parents[c] = p;
		pair<Node*,int> t = RBST::findRoot(&downs[p]);
		pair<Node*,int> u = RBST::findRoot(&downs[c]);
		assert(t.first != u.first);
		Node *s = RBST::find(t.first, t.second);	//propagateさせるために
		int depth = Node::getValue(s) + 1;
		u.first->add += depth;
		RBST::RefPair r = RBST::split(t.first, t.second + 1);
		return RBST::join(r.first, RBST::join(u.first, r.second));
	}
	Node *cut(int c) {
		assert(parents[c] != -1);
		parents[c] = -1;
		pair<Node*,int> t = RBST::findRoot(&downs[c]);
		pair<Node*,int> u = RBST::findRoot(&ups[c]);
		assert(t.first == u.first);
		Node *s = RBST::find(t.first, t.second);	//propagateさせるために
		int depth = Node::getValue(s);
		RBST::RefPair p = RBST::split(t.first, u.second + 1);
		RBST::RefPair q = RBST::split(p.first, t.second);
		RBST::join(q.first, p.second);
		q.second->add -= depth;
		return q.second;
	}
	int getLCA(int x, int y) {
		pair<Node*,int> t = RBST::findRoot(&downs[x]);
		pair<Node*,int> u = RBST::findRoot(&downs[y]);
		if(t.first != u.first) return -1;
		int l = t.second, r = u.second;
		if(l > r) swap(l, r);
		int minDepth = numeric_limits<int>::max();
		Node *minRef = NULL;
		getLCA_rec(t.first, l, r+1, minDepth, minRef);
		if(downs <= minRef && minRef < downs + n) {
			return minRef - downs;
		}else {
			assert(ups <= minRef && minRef < ups + n);
			return parents[minRef - ups];
		}
	}
	void debugout_rec(const Node *t, int add) {
		if(!t) return;
		int nadd = add + t->add;
		debugout_rec(t->left, nadd);
		if(downs <= t && t < downs + n)
			cerr << t - downs + 1;
		else
			cerr << parents[t - ups] + 1;
		cerr << ":" << t->value + nadd << ",";
		debugout_rec(t->right, nadd);
	}
	void debugout(int x) {
		debugout_rec(RBST::findRoot(&downs[x]).first, 0);
		cerr << endl;
	}
private:
	static void getLCA_rec(Node *t, int l, int r, int &minDepth, Node *&minRef) {
		if(l >= r || !t) return;
		int s = t->size;
		if(r <= 0 || s <= l) return;
		if(l <= 0 && s <= r) {
			int d = Node::getMinimum(t);
			if(minDepth > d) {
				minDepth = d;
				minRef = t->minRef;
			}
		}else {
			t->propagate();
			int sl = RBST::size(t->left);
			getLCA_rec(t->left, l, min(r, sl), minDepth, minRef);
			if(l <= sl && sl < r) {
				int d = Node::getValue(t);
				if(minDepth > d) {
					minDepth = d;
					minRef = t;
				}
			}
			getLCA_rec(t->right, max(l, sl + 1) - sl - 1, r - sl - 1, minDepth, minRef);
		}
	}
};

//<http://joisc2013-day4.contest.atcoder.jp/submissions/149693>
int main() {
	int N, Q;
	scanf("%d%d", &N, &Q);
	EulerTourTreeLCA ett; ett.init(N);
	rep(ii, Q) {
		int T;
		scanf("%d", &T);
		if(T == 1) {
			int A, B;
			scanf("%d%d", &A, &B); A --, B --;
			ett.link(B, A);
		}else if(T == 2) {
			int A;
			scanf("%d", &A); A --;
			ett.cut(A);
		}else if(T == 3) {
			int A, B;
			scanf("%d%d", &A, &B); A --, B --;
//			ett.debugout(A);
			int ans = ett.getLCA(A, B);
			printf("%d\n", ans == -1 ? -1 : ans + 1);
		}else assert(0);
	}
	return 0;
}
