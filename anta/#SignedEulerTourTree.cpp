//全く何も実行してもテストしてもいない！！！！！！！！！！！！！
//link,cut,getRoot,部分木add,部分木minimum,部分木sum,パスsumを実装？
//#PRBST.cppと#RBST.cpp の適切な部分が必要。省略

template<typename Value, typename Sum>
struct SignedMinAddSum : PNodeBase<SignedMinAddSum<Value,Sum> > {
	typedef PNodeBase<SignedMinAddSum<Value,Sum> > Base;
	//各頂点は正負両方に同じ値が入っているとする。
	//部分木に足し込むには正負両方に足し込む。
	//部分木の和を取るには正のみの総和を取る。
	//パスの和を取るには正負をかけて総和を取る。
	bool sign;
	int positiveSize, negativeSize;
	Value value, positiveMinimum;
	Sum positiveSum, negativeSum;
	Value positiveAdd, negativeAdd;
	SignedMinAddSum() { }
	SignedMinAddSum(const Value &value_):
		sign(false),
		positiveSize(1), negativeSize(0),
		value(value_), positiveMinimum(value_),
		positiveSum(value_), negativeSum(0),
		positiveAdd(0), negativeAdd(0) { }
	inline void propagate() {
		if(positiveAdd) {
			if(!sign) {
				value += positiveAdd;
				positiveSum += positiveAdd;
				positiveMinimum += positiveAdd;
			}
			if(Base::left) Base::left->positiveAdd += positiveAdd;
			if(Base::right) Base::right->positiveAdd += positiveAdd;
			positiveAdd = 0;
		}
		if(negativeAdd) {
			if(sign) {
				value += negativeAdd;
				negativeSum += negativeAdd;
			}
			if(Base::left) Base::left->negativeAdd += negativeAdd;
			if(Base::right) Base::right->negativeAdd += negativeAdd;
			negativeAdd = 0;
		}
		return Base::propagate();
	}
	static inline int getPositiveSize(SignedMinAddSum *t) {
		return !t ? 0 : t->positiveSize;
	}
	static inline int getNegativeSize(SignedMinAddSum *t) {
		return !t ? 0 : t->negativeSize;
	}
	static inline Sum getPositiveSum(SignedMinAddSum *t) {
		return !t ? 0 : t->positiveSum + (Sum)t->positiveAdd * t->positiveSize;
	}
	static inline Sum getNegativeSum(SignedMinAddSum *t) {
		return !t ? 0 : t->negativeSum + (Sum)t->negativeAdd * t->negativeSize;
	}
	static inline Value getValue(SignedMinAddSum *t) {
		return !t->sign ? t->value + t->positiveAdd : -t->value - t->negativeAdd;
	}
	static inline Value getPositiveMinimum(SignedMinAddSum *t) {
		return !t ? numeric_limits<Value>::max() : t->positiveMinimum;
	}
	static inline Sum getSignedSum(SignedMinAddSum *t) { return !t ? 0 : t->signedSize; }
	inline SignedMinAddSum *update() {
		positiveSize = getPositiveSize(Base::left) + (!sign ? 1 : 0) + getPositiveSize(Base::right);
		negativeSize = getNegativeSize(Base::left) + (!sign ? 0 : 1) + getNegativeSize(Base::right);
		positiveSum = getPositiveSum(Base::left) + (!sign ? value : 0) + getPositiveSum(Base::right);
		negativeSum = getNegativeSum(Base::left) + (!sign ? 0 : value) + getNegativeSum(Base::right);
		positiveMinimum = min(getPositiveMinimum(Base::left), getPositiveMinimum(Base::right));
		if(!sign) positiveMinimum = min(positiveMinimum, value);
		return Base::update();
	}
	void flipSign() {
		sign = !sign;
		update();
	}
};

//signed euler tourを管理するもの
struct SignedEulerTourTree {
	typedef int Value; typedef long long Sum;
	typedef SignedMinAddSum<Value, Sum> Node;
	typedef PRBSTBase<Node> RBST;
	Node *lefts, *rights;
	EulerTourTree2(): lefts(NULL), rights(NULL) { }
	~EulerTourTree2() { delete[] lefts; delete[] rights; }
	void init(int n) {
		delete[] lefts; delete[] rights;
		lefts = new Node[n]; rights = new Node[n];
		for(int i = 0; i < n; i ++) {
			rights[i].flipSign();
			RBST::join(&lefts[i], &rights[i]);
		}
	}
	Node *link(int p, int c) {	//p=親 に c=子 をlinkする。link後の木を返す
		pair<Node*,int> t = RBST::findRoot(&lefts[p]);
		pair<Node*,int> u = RBST::findRoot(&lefts[c]);
		my_assert(u.second == 0); //cは木の根であってほしい
		RBST::RefPair r = RBST::split(t.first, t.second + 1);	//pの右で切ってそこに差し込む
		return RBST::join(r.first, RBST::join(u.first, r.second));
	}
	Node *cut(int c) {	//cの親への辺を切る。切った木を返す
		pair<Node*,int> t = RBST::findRoot(&lefts[c]);
		pair<Node*,int> u = RBST::findRoot(&rights[c]);
		my_assert(t.first == u.first);
		RBST::RefPair p = RBST::split(t.first, u.second + 1);
		RBST::RefPair q = RBST::split(p.first, t.second);
		RBST::join(q.first, p.second);
		return q.second;
	}
	int getParent(int c) {
		pair<Node*,int> t = RBST::findRoot(&lefts[c]);
		if(t.second == 0) return -1;
		return RBST::find(t.first, t.second - 1) - lefts;
	}
	int findRoot(int x) {
		return RBST::leftest(RBST::findRoot(&lefts[x]).first) - lefts;
	}
	template<typename Func>
	void modify(int i, Func f) {
		pair<Node*,int> t = RBST::findRoot(&lefts[i]);
		pair<Node*,int> u = RBST::findRoot(&rights[i]);
		assert(t.first == u.first);
		RBST::modify(t.first, t.second, f);
		RBST::modify(u.first, u.second, f);
	}
	Node *getNode(Node *s) {
		pair<Node*,int> t = RBST::findRoot(s);
		Node *u = RBST::find(t.first, t.second);	//propagateさせるために
		assert(u == s);
		return u;
	}
	struct SetValue {
		Value x;
		SetValue(Value x_): x(x_) { }
		void operator()(Node *t) { t->value = x; }
	};
	void setValue(int i, Value x) {
		modify(i, SetValue(x));
	}
	Value getValue(int i) {
		return Node::getValue(getNode(&lefts[i]));
	}
	struct GetSubtreeSize {
		void operator()(int &size, Node *t) const {
			size += t->positiveSize;
		}
	};
	struct GetNodeSize {
		void operator()(int &size, Node *t) const {
			size += !t->sign;
		}
	};
	int getSubtreeSize(int i) {
		pair<Node*,int> t = RBST::findRoot(&lefts[i]);
		pair<Node*,int> u = RBST::findRoot(&rights[i]);
		assert(t.first == u.first);
		int size = 0;
		RBST::rangeQuery2(t.first, t.second, u.second+1, size, GetSubtreeSize(), GetNodeSize());
		return size;
	}
	struct AddToSubtree {
		Value x;
		AddToSubtree(Value x_): x(x_) { }
		void operator()(Node *t) const {
			t->positiveAdd += x;
			t->negativeAdd += x;
		}
	};
	struct AddToNode {
		Value x;
		AddToNode(Value x_): x(x_) { }
		void operator()(Node *t) const {
			t->value += x;
		}
	};
	void addToSubtree(int i, Value x) {
		pair<Node*,int> t = RBST::findRoot(&lefts[i]);
		pair<Node*,int> u = RBST::findRoot(&rights[i]);
		assert(t.first == u.first && t.second <= u.second);
		RBST::rangeProcess2(t.first, t.second, u.second+1, AddToSubtree(x), AddToNode(x));
	}
	struct GetSubtreeSum {
		void operator()(Sum &sum, Node *t) const {
			sum += Node::getPositiveSum(t);
		}
	};
	struct GetNodeSum {
		void operator()(Sum &sum, Node *t) const {
			if(!t->sign) sum += Node::getValue(t);
		}
	};
	Sum getSubtreeSum(int i) {
		pair<Node*,int> t = RBST::findRoot(&lefts[i]);
		pair<Node*,int> u = RBST::findRoot(&rights[i]);
		assert(t.first == u.first && t.second <= u.second);
		Sum sum = 0;
		RBST::rangeQuery2(t.first, t.second, u.second+1, sum, GetSubtreeSum(), GetNodeSum());
		return sum;
	}
	struct GetSubtreeSignedSum {
		void operator()(Sum &sum, Node *t) const {
			sum += Node::getPositiveSum(t) - Node::getNegativeSum(t);
		}
	};
	struct GetNodeSignedSum {
		void operator()(Sum &sum, Node *t) const {
			sum += Node::getValue(t);
		}
	};
	Sum getPathSum(int x, int y) {	//xがyの親である必要がある
		pair<Node*,int> t = RBST::findRoot(&lefts[x]);
		pair<Node*,int> u = RBST::findRoot(&lefts[y]);
		assert(t.first == u.first && t.second <= u.second);
		Sum sum = 0;
		RBST::rangeQuery2(t.first, t.second, u.second+1, sum, GetSubtreeSignedSum(), GetNodeSignedSum());
		return sum;
	}
	struct GetSubtreeMinimum {
		void operator()(Value &minimum, Node *t) const {
			minimum = min(minimum, Node::getPositiveMinimum(t));
		}
	};
	struct GetNodeMinimum {
		void operator()(Value &minimum, Node *t) const {
			if(!t->sign) minimum = min(minimum, Node::getValue(t));
		}
	};
	Value getSubtreeMinimum(int i) {
		pair<Node*,int> t = RBST::findRoot(&lefts[i]);
		pair<Node*,int> u = RBST::findRoot(&rights[i]);
		assert(t.first == u.first && t.second <= u.second);
		Value minimum = numeric_limits<Value>::max();
		RBST::rangeQuery2(t.first, t.second, u.second+1, minimum, GetSubtreeMinimum(), GetNodeMinimum());
		return minimum;
	}
};
