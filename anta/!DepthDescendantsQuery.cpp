//gに親→子の辺のみ(子→親は除くこと)の木を入れておく必要がある
//同じ深さの頂点はBFS-order上で連続している

typedef unsigned int u32;
inline int popcount(u32 x) {
	x = x - ((x >> 1) & 0x55555555); 
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	return ((x + (x >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}
//(k+1)番目の1の立っている位置(最下位から)を返す
//http://graphics.stanford.edu/~seander/bithacks.html#SelectPosFromMSBRank を参考にした
inline int select32(u32 x, int k) {
	u32 a, b, c; int t, s;
	a = (x & 0x55555555) + ((x >> 1) & 0x55555555);
	b = (a & 0x33333333) + ((a >> 2) & 0x33333333);
	c = (b & 0x0f0f0f0f) + ((b >> 4) & 0x0f0f0f0f);
	t = (c & 0xff) + ((c >> 8) & 0xff);
	s = 0;
	s += ((t - k - 1) & 128) >> 3; k -= t & ((t - k - 1) >> 8); //if(k >= t) s += 16, k -= t;
	t = (c >> s) & 0xf;
	s += ((t - k - 1) & 128) >> 4; k -= t & ((t - k - 1) >> 8); //if(k >= t) s += 8, k -= t;
	t = (b >> s) & 0x7;
	s += ((t - k - 1) & 128) >> 5; k -= t & ((t - k - 1) >> 8); //if(k >= t) s += 4, k -= t;
	t = (a >> s) & 0x3;
	s += ((t - k - 1) & 128) >> 6; k -= t & ((t - k - 1) >> 8); //if(k >= t) s += 2, k -= t;
	t = (x >> s) & 0x1;
	s += ((t - k - 1) & 128) >> 7; //if(k >= t) s += 1;
	return s;
}
//※静的なデータ構造
//constructした後setを何回か呼び、その後buildを呼んだ後にrank,selectが行える
struct FullyIndexableDictionary {
	static const int NOTFOUND = -1;
	static const int SELECTT_INTERVAL = 32;	//SELECTT_INTERVAL >= 32
	int length, blockslength, count;
	vector<u32> blocks; vector<int> ranktable, selecttable0, selecttable1;
	FullyIndexableDictionary(int len): length(len) {
		blocks.resize((blockslength = (len + 31) / 32) + 1);
	}
	inline void set(int i) { blocks[i / 32] |= 1 << i % 32; }
	void build() {
		if(length == 0) { count = 0; return; }
		ranktable.assign(blockslength + 1, 0);
		selecttable0.clear(); selecttable1.clear();
		int prev0 = 0, prev1 = 0, count0 = 0, count1 = 0;
		for(int i = 0; i < blockslength; i ++) {
			ranktable[i] = count1;
			count1 += popcount(blocks[i]);
			count0 = 32 * (i + 1) - count1;
			if(prev1 < (count1+SELECTT_INTERVAL-1) / SELECTT_INTERVAL)
				selecttable1.push_back(i), prev1 = (count1+SELECTT_INTERVAL-1) / SELECTT_INTERVAL;
			if(prev0 < (count0+SELECTT_INTERVAL-1) / SELECTT_INTERVAL)
				selecttable0.push_back(i), prev0 = (count0+SELECTT_INTERVAL-1) / SELECTT_INTERVAL;
		}
		ranktable[blockslength] = count1;
		selecttable1.push_back(blockslength-1);
		selecttable0.push_back(blockslength-1);
		count = count1;
	}
	inline bool access(int pos) const { 
		return blocks[pos / 32] >> pos % 32 & 1;
	}
	inline int rank(int pos) const {	//[0..pos)の1の個数
		int block_idx = pos / 32;
		return ranktable[block_idx] + popcount(blocks[block_idx] & (1U << pos % 32)-1);
	}
	inline int rank(bool b, int pos) const { return b ? rank(pos) : pos - rank(pos); }
	inline int rank(bool b, int left, int right) const { return rank(b, right) - rank(b, left); }
	template<bool b>
	int select(int k) const {	//(k+1)番目のbの位置
		if((b ? count : length - count) <= k) return NOTFOUND;
		int selecttable_index = k / SELECTT_INTERVAL;
		int l = (b ? selecttable1 : selecttable0)[selecttable_index],
			u = (b ? selecttable1 : selecttable0)[selecttable_index + 1] + 1;	//ブロックを二分探索
		while(l + 1 < u) {
			int m = (l + u) / 2;
			((b ? ranktable[m] : m * 32 - ranktable[m]) <= k ? l : u) = m;
		}
		return l * 32 + select32(b ? blocks[l] : ~blocks[l], k - (b ? ranktable[l] : 32 * l - ranktable[l]));
	}
	inline int select(bool b, int k) const { return b ? select<true>(k) : select<false>(k); }
	inline int select(bool b, int k, int left) const { return select(b, rank(b, left) + k); }
};

inline unsigned int BITMASK(int i) {
	return (1 << i) - 1;
}
struct WaveletMatrix {
	typedef unsigned int Val;
	static const int NOTFOUND = -1;
	static const int MAX_BITSIZE = 64;
	int length, bitsize; Val maxval;
	vector<FullyIndexableDictionary> dicts;
	vector<int> mids;
	void init(const vector<Val>& data) {
		length = data.size();
		maxval = *max_element(data.begin(), data.end());
		if(1ULL << (8 * sizeof(Val) - 1) <= maxval) bitsize = 8 * sizeof(Val);
		else for(bitsize = 0; Val(1) << bitsize <= maxval; bitsize ++) ;
		dicts.assign(bitsize, length);
		mids.assign(bitsize, 0);
		vector<Val> datacurrent(data), datanext(length);
		for(int bit = 0; bit < bitsize; bit ++) {
			int pos = 0;
			for(int i = 0; i < length; i ++)
				if((datacurrent[i] >> (bitsize - bit - 1) & 1) == 0)
					datanext[pos ++] = datacurrent[i];
			mids[bit] = pos;
			for(int i = 0; i < length; i ++)
				if((datacurrent[i] >> (bitsize - bit - 1) & 1) != 0)
					dicts[bit].set(i), datanext[pos ++] = datacurrent[i];
			dicts[bit].build();
			datacurrent.swap(datanext);
		}
	}
	int rank(Val val, int left, int right) const {
		if(val > maxval) return 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			bool dir = val >> (bitsize - bit - 1) & 1;
			left = dicts[bit].rank(dir, left), right = dicts[bit].rank(dir, right);
			if(dir) left += mids[bit], right += mids[bit];
		}
		return right - left;
	}
	int rank(Val val, int right) const { return rank(val, 0, right); }
	//O(bitsize log length) (FID::selectがlog nで最悪の場合)
	int select(Val val, int k) const {
		if(val > maxval) return NOTFOUND;
		static int lefts[MAX_BITSIZE], rights[MAX_BITSIZE];
		int left = 0, right = length;
		for(int bit = 0; bit < bitsize; bit ++) {
			lefts[bit] = left, rights[bit] = right;
			bool dir = val >> (bitsize - bit - 1) & 1;
			left = dicts[bit].rank(dir, left), right = dicts[bit].rank(dir, right);
			if(dir) left += mids[bit], right += mids[bit];
		}
		for(int bit = bitsize-1; bit >= 0; bit --) {
			k = dicts[bit].select(val >> (bitsize - bit - 1) & 1, k, lefts[bit]);
			if(k == FullyIndexableDictionary::NOTFOUND || k >= rights[bit])
				return NOTFOUND;
			k -= lefts[bit];
		}
		return k;
	}
	int select(Val val, int k, int left) const { return select(val, k + rank(val, left)); }
};

vector<vi> g;

vector<int> preorder;
int preorder_left[100000], preorder_right[100000];

vector<int> bfsorder;
int bfsindex[100000], depth[100000], depthoffset[100001];
int totaldepth;

//gは親への辺は無い
void preorder_traversal(int root) {
	preorder.clear();
	vector<pii> s;
	s.pb(mp(root, 0));
	while(!s.empty()) {
		int i = s.back().first, j = s.back().second;
		s.pop_back();
		if(j == 0) {
			preorder_left[i] = preorder.size();
			preorder.push_back(i);
		}
		if(j == g[i].size()) {
			preorder_right[i] = preorder.size();
		}else {
			s.push_back(mp(i, j+1));
			s.push_back(mp(g[i][j], 0));
		}
	}
}

//gは親への辺は無い
void bfs_traversal(int root) {
	bfsorder.clear();
	vi q, next;
	int d = 0;
	next.push_back(root);
	while(!next.empty()) {
		q.swap(next);
		depthoffset[d] = bfsorder.size();
		while(!q.empty()) {
			int i = q.back(); q.pop_back();
			depth[i] = d;
			bfsindex[i] = bfsorder.size();
			bfsorder.pb(i);
			each(j, g[i]) next.push_back(*j);
		}
		reverse(all(next));
		d ++;
	}
	depthoffset[d] = bfsorder.size();
	totaldepth = d;
}

WaveletMatrix preorder_depth_seq;
void depth_descendants_init() {
	vector<WaveletMatrix::Val> v;
	rep(i, preorder.size())
		v.push_back(depth[preorder[i]]);
	preorder_depth_seq.init(v);
}

pii depth_descendants_query(int v, int d) {
	d += depth[v];
	int left = preorder_left[v], right = preorder_right[v];
	int count = preorder_depth_seq.rank(d, left, right);
	if(count == 0) return mp(-1, -1);
	int t = preorder_depth_seq.rank(d, left);
	int x = preorder[preorder_depth_seq.select(d, t)];
	int y = preorder[preorder_depth_seq.select(d, t + count - 1)];
	return mp(x, y);
}
