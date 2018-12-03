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

//配列バージョン
struct UnionFind {
	static const int MAXN = 8 * 8;
	typedef char Index;	//適切に書き換えること。符号付きでないといけないので注意
	Index data[MAXN];
	void init(int size_) { memset(data, -1, size_ * sizeof(*data)); }
	bool unionSet(int x, int y) {
		x = root(x); y = root(y);
		if (x != y) {
			if (data[y] < data[x]) swap(x, y);
			data[x] += data[y]; data[y] = x;
		}
		return x != y;
	}
	inline bool findSet(int x, int y) { return root(x) == root(y); }
	inline int root(int x) { return data[x] < 0 ? x : data[x] = root(data[x]); }
	inline int size(int x) { return -data[root(x)]; }
};

