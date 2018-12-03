struct trie {
	struct node {
		int nxt[26];
		bool end;
		node(bool end = false) : end(end) { memset(nxt, 0, sizeof nxt); }
	};
	vector<node> T;
	trie() : T(1) {}
	inline int getNode() { T.emplace_back(); return sz(T) - 1; }
	void insert(const string& s) {
		int n = sz(s);
		int now = 0;
		FOR (i, 0, n - 1) {
			int nxt = T[now].nxt[s[i] - 'a'];
			if (nxt == 0) {
				nxt = getNode();
				T[now].nxt[s[i] - 'a'] = nxt;
			}
			now = nxt;
		}
		T[now].end = true;
	}
};