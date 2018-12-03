#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <queue>
#include <cassert>
using namespace std;

//Appendable
typedef unsigned int u32;
inline int popcount(u32 x) {
	x = x - ((x >> 1) & 0x55555555); 
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	return ((x + (x >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}
struct AppendableRankDictionary {
	static const int NOTFOUND = -1;
	int length, blockslength, count;
	vector<u32> blocks; vector<int> ranktable;
	AppendableRankDictionary() {
		count = length = 0;
		blockslength = 1;
		blocks.assign(1, 0);
		ranktable.assign(2, 0);
	}
	void append(bool b) {
		if(b) {
			int block = length / 32;
			blocks[block] |= 1U << length % 32;
			ranktable[block+1] ++;
			count ++;
		}
		if((++ length) % 32 == 0) {
			blockslength ++;
			blocks.push_back(0);
			ranktable.push_back(ranktable.back());
		}
	}
	inline int rank(int pos) const {	//[0..pos)の1の個数
		int block_idx = pos / 32;
		return ranktable[block_idx] + popcount(blocks[block_idx] & (1U << pos % 32)-1);
	}
	inline int rank(bool b, int pos) const { return b ? rank(pos) : pos - rank(pos); }
	inline int rank(bool b, int left, int right) const { return rank(b, right) - rank(b, left); }
};


inline unsigned int BITMASK(int i) {
	return (1 << i) - 1;
}
struct AppendableWaveletTree {
	typedef unsigned int Val;
	static const int NOTFOUND = -1;
	static const Val UNDEFINED = Val(-1);
	vector<vector<AppendableRankDictionary> > dicts;
	Val maxval; int bitsize, length;
	void init(Val maxval_) {
		maxval = maxval_;
		length = 0;
		for(bitsize = 0; Val(1) << bitsize <= maxval; bitsize ++) ;
		dicts.resize(bitsize);
		for(int bit = 0; bit < bitsize; bit ++)
			dicts[bit].assign(1 << bit, AppendableRankDictionary());
	}
	void append(Val v) {
		int node = 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			bool dir = v >> (bitsize - bit - 1) & 1;
			dicts[bit][node].append(dir);
			node = node * 2 + dir;
		}
		length ++;
	}
	void rank_all(Val v, int left, int right, int& out_lt, int& out_gt) const {
		if(v > maxval) { out_lt = right - left; out_gt = 0; return; }
		out_lt = out_gt = 0;
		int node = 0;
		for(int bit = 0; ; bit ++) {
			bool dir = v >> (bitsize - bit - 1) & 1;
			int rightcount = dicts[bit][node].rank(true, right);
			int leftcount = dicts[bit][node].rank(true, left);
			int count = rightcount - leftcount;
			(dir ? out_lt : out_gt) += dir ? (right - left) - count : count;
			if(bit == bitsize-1) return;
			left = dir ? leftcount : left - leftcount;
			right = dir ? rightcount : right - rightcount;
			node = node * 2 + dir;
		}
	}
	inline int rank_lt(Val v, int left, int right) const {
		int tmp_lt, tmp_gt;
		rank_all(v, left, right, tmp_lt, tmp_gt);
		return tmp_lt;
	}
	inline int rangefreq(int left, int right, Val bottom, Val up) {
		return rank_lt(up, left, right) - rank_lt(bottom, left, right);
	}
};

#if 0
//INTERVALを使用したバージョン。INTERVALをやらなくてもメモリは2*nなので問題無いと思う
//でもこっちの方が速いかも？
struct FullyIndexableDictionary {
	static const int INTERVAL = 4;
	static const int NOTFOUND = -1;
	int length, count;
	vector<u32> blocks; vector<int> table;
	FullyIndexableDictionary(int len): length(len) { blocks.resize((len + 31) / 32); }
	void set(int i) { blocks[i / 32] |= 1 << i % 32; }
	void build() {
		count = 0;
		table.assign((blocks.size() + INTERVAL - 1) / INTERVAL, 0);
		for(int i = 0; i < (int)blocks.size(); i ++) {
			if(i % INTERVAL == 0) table[i / INTERVAL] = count;
			count += popcount(blocks[i]);
		}
	}
	int rank(int pos) const {	//[0..pos)の1の個数
		int block_idx = pos / 32;
		int table_idx = block_idx / INTERVAL;
		int r = table[table_idx];
		for(int i = table_idx * INTERVAL; i < block_idx; i ++)
			r += popcount(blocks[i]);
		return r + popcount(blocks[block_idx] & (1U << pos % 32)-1);
	}
	template<bool b> int select(int k) const {	//(k+1)番目のbの位置
		if((b ? count : length - count) < k) return NOTFOUND;
		int l = 0, u = table.size();	//テーブルを二分探索
		while(l + 1 < u) {
			int m = (l + u) / 2;
			((b ? table[m] : INTERVAL * 32 * m - table[m]) <= k ? l : u) = m;
		}
		int r = l * INTERVAL * 32; k -= table[l];
		for(int block_idx = l * INTERVAL; ; block_idx ++) {
			int current = b ? popcount(blocks[block_idx]) : 32 - popcount(blocks[block_idx]);
			if(current >= k) {
				r += select32(b ? blocks[block_idx] : ~blocks[block_idx], k);
				return r;
			}
			k -= current; r += 32;
		}
	}
};
#endif

#if 0

//以下では n = length, m = maxval+1, l = bitsize とする
//m ∈ Θ(2^b), b ∈ Θ(log m) であることに注意
struct WaveletTree {
	typedef unsigned int Val;
	static const int NOTFOUND = -1;
	static const Val UNDEFINED = Val(-1);
	vector<FullyIndexableDictionary> dicts;
	Val maxval; int bitsize, length;
	void init(const vector<Val> data) {
		length = data.size();
		maxval = *max_element(data.begin(), data.end());
		for(bitsize = 0; Val(1) << bitsize <= maxval; bitsize ++) ;
		dicts.resize(bitsize, length);

		/*
//		普通のソートでΘ(b n log n), メモリ4*n ∈ Θ(n)バージョン
		struct ShiftComp {
			int bit;
			ShiftComp(int b): bit(b) {}
			inline bool operator()(Val x, Val y) const {
				return (x >> bit) < (y >> bit);
			}
		};
		vector<Val> tmp(data.begin(), data.end());
		for(int bit = 0; bit < bitsize; bit ++) {
			FullyIndexableDictionary& dict = dicts[bit];
			sort(tmp.begin(), tmp.end(), ShiftComp(bitsize - bit));
			for(int i = 0; i < length; i ++)
				if(tmp[i] >> (bitsize - bit - 1) & 1)
					dict.set(i);
			dict.build();
		}
//		*/
		//バケットソートでΘ(b n + m), メモリ4*2^(b-1) ∈ Θ(m) バージョン
//		/*
		
		for(int bit = 0; bit < bitsize; bit ++) {
			FullyIndexableDictionary& dict = dicts[bit];
			vector<int> poses(1 << bit);
			for(int i = 0; i < length; i ++)
				poses[data[i] >> (bitsize - bit)] ++;
			for(int i = 1; i < 1 << bit; i ++) poses[i] += poses[i-1];
			for(int i = length-1; i >= 0; i --) {
				int x = data[i] >> (bitsize - bit);
				poses[x] --;
				if(data[i] >> (bitsize - bit - 1) & 1)
					dict.set(poses[x]);
			}
			dict.build();
		}
//		*/
	}
	Val access(int pos) const {
		Val val = 0;
		int nodeleft = 0, noderight = length;
		for(int bit = 0; bit < bitsize; bit ++) {
			bool dir = dicts[bit].access(pos);
			val = val << 1 | (dir ? 1 : 0);
			int leftcount = dicts[bit].rank(dir, nodeleft, pos);
			if(dir) nodeleft += dicts[bit].rank(false, nodeleft, noderight);
			else noderight -= dicts[bit].rank(true, nodeleft, noderight);
			pos = nodeleft + leftcount;
		}
		return val;
	}
	int rank(Val v, int pos) const {
		if(v > maxval) return 0;
		int nodeleft = 0, noderight = length;
		for(int bit = 0; bit < bitsize; bit ++) {
			bool dir = v >> (bitsize - bit - 1) & 1;
			pos = dicts[bit].rank(dir, nodeleft, nodeleft + pos);
			if(dir) nodeleft += dicts[bit].rank(false, nodeleft, noderight);
			else noderight -= dicts[bit].rank(true, nodeleft, noderight);
		}
		return pos;
	}
	void rank_all(Val v, int left, int right, int& out_lt, int& out_gt) const {
		if(v > maxval) { out_lt = right - left; out_gt = 0; return; }
		out_lt = out_gt = 0;
		int nodeleft = 0, noderight = length;
		for(int bit = 0; ; bit ++) {
			int count = dicts[bit].rank(true, left, right);
			bool dir = v >> (bitsize - bit - 1) & 1;
			(dir ? out_lt : out_gt) += dir ? (right - left) - count : count;
			if(bit == bitsize-1) return;
			int dircount = dir ? count : (right - left) - count;
			int leftcount = dicts[bit].rank(dir, nodeleft, left);
			if(dir) nodeleft += dicts[bit].rank(false, nodeleft, noderight);
			else noderight -= dicts[bit].rank(true, nodeleft, noderight);
			left = nodeleft + leftcount;
			right = left + dircount;
		}
	}
	inline int rank_lt(Val v, int left, int right) const {
		int tmp_lt, tmp_gt;
		rank_all(v, left, right, tmp_lt, tmp_gt);
		return tmp_lt;
	}
	inline int rank_gt(Val v, int left, int right) const {
		int tmp_lt, tmp_gt;
		rank_all(v, left, right, tmp_lt, tmp_gt);
		return tmp_gt;
	}
	int select(Val v, int k) const {
		if(v > maxval) return NOTFOUND;
		static int nodelefts[33], noderights[33];
		int nodeleft = 0, noderight = length;
		for(int bit = 0; bit < bitsize; bit ++) {
			nodelefts[bit] = nodeleft; noderights[bit] = noderight;
	 		bool dir = v >> (bitsize - bit - 1) & 1;
			if(dir) nodeleft += dicts[bit].rank(false, nodeleft, noderight);
			else noderight -= dicts[bit].rank(true, nodeleft, noderight);
		}
		for(int bit = bitsize-1; bit >= 0; bit --) {
			int index = v >> (bitsize - bit);
			k = dicts[bit].select(v >> (bitsize - bit - 1) & 1, k, nodelefts[bit]);
			if(k == FullyIndexableDictionary::NOTFOUND || k >= noderights[bit])
				return NOTFOUND;
			k -= nodelefts[bit];
		}
		return k;
	}
	inline int select(Val v, int k, int left) const { return select(v, rank(v, left) + k); }

	//quantileを連続で呼び出せば同じ効果が得られる。
	//こっちはインデックスを持ってこれないし。こっちの方がO(k+l)で速い？
	//あと、push_backのところをアルファベットごとに数えるだけのようにすればO(m+l)かな
	//下のrectftopkでも同じことができるが、こっちはpriority_queue必要なくてdfsしているということで
	template<bool maxf> void rangeminmaxk(int left, int right, int k, vector<Val>& out) const {
		return rangeminmaxk_rec<maxf>(out, 0, 0, 0, length, left, right, min(k, length));
	}
	template<bool maxf> void rangeminmaxk_rec(vector<Val>& out, int bit, int val, int nodeleft, int noderight, int left, int right, int k) const {
		int t = dicts[bit].rank(maxf, left, right);
		int nk = min(k, t);
		if(nk != 0) {
			int nval = val << 1 | (maxf ? 1 : 0);
			if(bit == bitsize-1) {
				for(int i = 0; i < nk; i ++)
					out.push_back(nval);
			}else {
				int leftcount = dicts[bit].rank(maxf, nodeleft, left);
				if(maxf) nodeleft += dicts[bit].rank(false, nodeleft, noderight);
				else noderight -= dicts[bit].rank(true, nodeleft, noderight);
				int nleft = nodeleft + leftcount;
				rangeminmaxk_rec<maxf>(out, bit+1, nval, nodeleft, noderight, nleft, nleft + t, nk);
			}
		}
		nk = k - nk;
		if(nk != 0) {
			int nval = val << 1 | (maxf ? 0 : 1);
			if(bit == bitsize-1) {
				for(int i = 0; i < nk; i ++)
					out.push_back(nval);
			}else {
				int leftcount = dicts[bit].rank(!maxf, nodeleft, left);
				if(!maxf) nodeleft += dicts[bit].rank(false, nodeleft, noderight);
				else noderight -= dicts[bit].rank(true, nodeleft, noderight);
				int nleft = nodeleft + leftcount;
				rangeminmaxk_rec<maxf>(out, bit+1, nval, nodeleft, noderight, nleft, nleft + ((right - left) - t), nk);
			}
		}
	}
	struct IdxVal {
		int idx; Val val;
		IdxVal() {}
		IdxVal(int i, Val v): idx(i), val(v) {}
	};
	void quantile(int left, int right, int k, Val& out_val, int& out_k) const {
		if(right - left <= k) { out_val = UNDEFINED; out_k = NOTFOUND; return; }
		Val val = 0;
		int nodeleft = 0, noderight = length;
		for(int bit = 0; ; bit ++) {
			int count = dicts[bit].rank(true, left, right);
			bool dir = k < count;
			Val nval = val << 1 | (dir ? 1 : 0);
			if(!dir) k -= count;
			if(bit == bitsize-1) {
				out_val = nval; out_k = k;
				return;
			}
			int dircount = dir ? count : (right - left) - count;
			int leftcount = dicts[bit].rank(dir, nodeleft, left);
			if(dir) nodeleft += dicts[bit].rank(false, nodeleft, noderight);
			else noderight -= dicts[bit].rank(true, nodeleft, noderight);
			left = nodeleft + leftcount;
			right = left + dircount;
			val = nval;
		}
	}
	inline void quantile(int left, int right, int k, IdxVal& out_idxval) const {
		Val tmp_val; int tmp_k;
		quantile(left, right, k, tmp_val, tmp_k);
		out_idxval.val = tmp_val;
		out_idxval.idx = select(tmp_val, tmp_k, left);
	}
	inline Val quantile(int left, int right, int k) const {
		Val tmp_val; int tmp_k;
		quantile(left, right, k, tmp_val, tmp_k);
		return tmp_val;
	}
	inline Val maximum(int left, int right) const { return quantile(left, right, 0); }
	inline Val minimum(int left, int right) const { return quantile(left, right, right - left - 1); }
	inline int rengefreq(int left, int right, Val bottom, Val up) {
		return rank_lt(up, left, right) - rank_lt(bottom, left, right);
	}
	struct Range {
		int left, right;
		int nodeleft, noderight;
		int bit; Val val;
		Range(int l, int r, int nl, int nr, int b, Val v):
			left(l), right(r), nodeleft(nl), noderight(nr), bit(b), val(v) {}
	};
	template<typename F, typename FOut>
	int rectftopk(const F& f, int left, int right, Val bottom, Val up, int k, FOut& out) const {
		int k0 = k;
		up = min(up, maxval+1);
		priority_queue<Range, vector<Range>, F> q(f);
		q.push(Range(left, right, 0, length, 0, 0));
		while(k && !q.empty()) {
			Range t = q.top(); q.pop();
			if(t.bit == bitsize) {
				f.pushvalues(out, t, k);
			}else {
				int leftcount = dicts[t.bit].rank(false, t.nodeleft, t.left);
				int rangecount = dicts[t.bit].rank(false, t.left, t.right);
				int nodecount = dicts[t.bit].rank(true, t.nodeleft, t.noderight);
				if( rangecount != 0 &&
					bottom <= ((t.val << (bitsize - t.bit)) | ((Val(1) << (bitsize - t.bit - 1)) - 1)))
					q.push(Range(
					t.nodeleft + leftcount,
					t.nodeleft + leftcount + rangecount,
					t.nodeleft,
					t.noderight - nodecount,
					t.bit + 1,
					t.val << 1
					));
				if( (t.right - t.left) - rangecount != 0 &&
					(((t.val << 1 | 1) << (bitsize - t.bit - 1))) < up) {
					int nnodeleft =
						t.nodeleft + ((t.noderight - t.nodeleft) - nodecount);
					int nleft =
						nnodeleft + ((t.left - t.nodeleft) - leftcount);
					q.push(Range(
						nleft,
						nleft + ((t.right - t.left) - rangecount),
						nnodeleft,
						t.noderight,
						t.bit + 1,
						t.val << 1 | 1
						));
				}
			}
		}
		return k0 - k;
	}
	struct ValCount {
		Val val; int count;
		ValCount(Val v, int c): val(v), count(c) {}
		ValCount() {}
	};
	struct FreqList {
		inline bool operator()(const Range& x, const Range& y) const {
			return x.right - x.left < y.right - y.left ||
				x.right - x.left == y.right - y.left && x.val > y.val;
		}
		inline void pushvalues(vector<ValCount>& out, const Range& t, int& k) const {
			out.push_back(ValCount(t.val, t.right - t.left));
			k --;
		}
	};
	inline int topk(int left, int right, Val bottom, Val up, int k, vector<ValCount>& out) const {
		return rectftopk<FreqList,vector<ValCount> >(FreqList(), left, right, bottom, up, k, out);
	}
	template<bool maxf> struct MinMaxList {
		inline bool operator()(const Range& x, const Range& y) const {
			return (x.val < y.val) == maxf;
		}
		inline void pushvalues(vector<ValCount>& out, const Range& t, int& k) const {
			int x = min(k, t.right - t.left);
			out.push_back(ValFreq(t.val, x));
			k -= x;
		}
	};
	template<bool maxf>
	inline int rangeminmaxk_anotherversion(int left, int right, Val bottom, Val up, int k, vector<ValCount>& out) {
		return rectftopk<MinMaxList<maxf>,vector<ValFreq> >(MinMaxList<maxf>(), left, right, bottom, up, k, out);
	}
};
ostream& operator<<(ostream& o, const WaveletTree::IdxVal& idxval) {
	return o << "(" << idxval.idx << ": " << idxval.val << ")";
}
ostream& operator<<(ostream& o, const WaveletTree::ValCount& valcount) {
	return o << "[" << valcount.val << " * " << valcount.count << "]";
}

#endif