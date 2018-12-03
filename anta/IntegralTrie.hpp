
template<typename Val, typename Int = unsigned, unsigned BitSize = sizeof(Int)*8>
class IntegralTrie {
	enum { BlockSize = 5	//2^BlockSize <= 32
		, FirstStage = (BitSize-1) / BlockSize };
	
	template<unsigned Stage> struct Node {
		unsigned mask;
		Node<Stage-1> *next[1 << BlockSize];
		Node<Stage>() { mask = 0; }
	};
	template<> struct Node<0> {
		unsigned mask;
		Val values[1 << BlockSize];	//sizeof(Val)‚ª¬‚³‚¢‚à‚Ì‚Æl‚¦‚Ä‚¢‚é
		Node<0>() { mask = 0; }
	};
	template<unsigned Stage> inline Node<Stage> *newNode() {
		return new Node<Stage>();
	}

	template<unsigned Stage> inline Val *getRef(Node<Stage> *node, Int x) {
		Int h = x & (1 << BlockSize)-1;
		return getRef<Stage-1>((node->mask >> h & 1)
			? node->next[h]
			: (node->mask |= 1 << h, node->next[h] = newNode<Stage-1>())
			, x >> BlockSize);
	}
	template<> inline Val *getRef<0>(Node<0> *node, Int x) {
		node->mask |= 1 << x;
		return &node->values[x];
	}

	template<unsigned Stage> inline const Val *get(const Node<Stage> *node, Int x) const {
		Int h = x & (1 << BlockSize)-1;
		if(node->mask >> h & 1) return get<Stage-1>(node->next[h], x >> BlockSize);
		else return NULL;
	}
	template<> inline const Val *get<0>(const Node<0> *node, Int x) const {
		if(node->mask >> x & 1) return &node->values[x];
		else return NULL;
	}

	template<unsigned Stage> inline void destruct(Node<Stage> *node) {
		Node<Stage-1> **p = node->next;
		for(unsigned m = node->mask; m; ++ p, m >>= 1)
			if(m & 1) destruct<Stage-1>(*p);
		delete node;
	}
	template<> inline void destruct<0>(Node<0> *node) {
		delete node;
	}

	Node<FirstStage> *root;
public:
	IntegralTrie(): root(newNode<FirstStage>()) { }
	~IntegralTrie() { destruct<FirstStage>(root); }

	Val *getRef(Int x) { return getRef<FirstStage>(root, x); }
	const Val *get(Int x) const { return get<FirstStage>(root, x); }
};

