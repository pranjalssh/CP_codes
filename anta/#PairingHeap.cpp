template<typename T> struct PairingHeap {
	struct Node {
		T key;
		Node *head, *next;
		Node(T k): key(k), head(0), next(0) {}
		Node(): head(0), next(0) {}
	} *root;
	Node *merge(Node *x, Node *y) {
		if(!x || !y) return x ? x : y;
		if(x->key > y->key) swap(x, y);
		y->next = x->head; x->head = y;
		return x;
	}
	Node *mergeList(Node *x) {
		Node t;
		while(x) {
			Node *a = x, *b = 0;
			x = x->next; a->next = 0;
			if(x) b = x, x = x->next, b->next = 0;
			a = merge(a, b); a->next = t.next;
			t.next = a;
		}
		while(t.next) {
			Node *u = t.next;
			t.next = t.next->next;
			x = merge(u, x);
		}
		return x;
	}
	const T& top() { return root->key; }
	void pop()     { root = mergeList(root->head); }
	void push(T k) { root = merge(root, new Node(k)); }
	bool empty()   { return root == NULL; }
	PairingHeap(): root(0) {}
};

int main() {
	PairingHeap<int> p;
	rep(i, 1000000) p.push(rand()*(RAND_MAX+1)+rand());
	rep(i, 1000) {
		cout << p.top() << endl;
		p.pop();
	}
	return 0;
}
