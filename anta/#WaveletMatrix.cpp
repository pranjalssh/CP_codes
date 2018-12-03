//高速化？: http://www.codechef.com/viewsolution/1790313

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <queue>
#include <cassert>
using namespace std;

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
	//O(log n)は重いよねえ。expect O(1) (たぶん) ならできるけど最悪ケースがなあ
	//あるいはメモリ4*32*n bytes でO(1)でもできるけど…ハッシュテーブルでうまくやればうまくできるか？
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

/*
	WaveletMatrix。たしかにこっちの方が「常に」使うべきな気がする
*/
inline unsigned int BITMASK(int i) {
	return (1 << i) - 1;
}
//※メモリ, 時間はだいたい支配的な部分のみ書く
//メモリ: (length * bitsize / 8) bytes
struct WaveletMatrix {
	typedef unsigned long long Val;
	static const int NOTFOUND = -1;
	static const Val UNDEFINED = Val(-1);
	static const int MAX_BITSIZE = 64;
	int length, bitsize; Val maxval;
	vector<FullyIndexableDictionary> dicts;
	vector<int> mids;
	//追加メモリ: (2 * length * sizeof Val) bytes
	//時間: bitsize * length * 大きめ
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
	Val access(int pos) const {
		Val val = 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			bool dir = dicts[bit].access(pos);
			val = val << 1 | (dir ? 1 : 0);
			pos = dicts[bit].rank(dir, pos);
			if(dir) pos += mids[bit];
		}
		return val;
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
	int rank_all(Val val, int left, int right, int& out_lt, int& out_gt) const {
		if(val > maxval) { out_lt = right - left; out_gt = 0; return 0; }
		out_lt = out_gt = 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			bool dir = val >> (bitsize - bit - 1) & 1;
			int leftcount = dicts[bit].rank(dir, left), rightcount = dicts[bit].rank(dir, right);
			(dir ? out_lt : out_gt) += (right - left) - (rightcount - leftcount);
			left = leftcount, right = rightcount;
			if(dir) left += mids[bit], right += mids[bit];
		}
		return right - left;
	}
	inline int rank_lt(Val val, int left, int right) const {
		int tmp_lt, tmp_gt;
		rank_all(val, left, right, tmp_lt, tmp_gt);
		return tmp_lt;
	}
	inline int rangefreq(int left, int right, Val bottom, Val up) {
		return rank_lt(up, left, right) - rank_lt(bottom, left, right);
	}
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
	void quantile(int left, int right, int k, Val& out_val, int& out_k) const {
		if(right - left <= k) { out_val = UNDEFINED; out_k = NOTFOUND; return; }
		Val val = 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			int count = dicts[bit].rank(true, left, right);
			bool dir = k < count;
			val = val << 1 | (dir ? 1 : 0);
			if(!dir) k -= count;
			left = dicts[bit].rank(dir, left), right = dicts[bit].rank(dir, right);
			if(dir) left += mids[bit], right += mids[bit];
		}
		out_val = val; out_k = k;
	}
	struct IdxVal {
		int idx; Val val;
		IdxVal() {}
		IdxVal(int i, Val v): idx(i), val(v) {}
	};
	inline Val quantile(int left, int right, int k) const {
		Val tmp_val; int tmp_k;
		quantile(left, right, k, tmp_val, tmp_k);
		return tmp_val;
	}
	inline IdxVal quantile_idxval(int left, int right, int k) const {
		Val tmp_val; int tmp_k;
		quantile(left, right, k, tmp_val, tmp_k);
		return IdxVal(select(tmp_val, tmp_k, left), tmp_val);
	}
	inline Val maximum(int left, int right) const { return quantile(left, right, 0); }
	inline Val minimum(int left, int right) const { return quantile(left, right, right - left - 1); }
	//区間がかぶってるとkとかがintより大きくなるよね
	void quantile_ranges(const vector<int>& lefts0, const vector<int>& rights0, int k, Val& out_val, int& out_k) const {
		int n = lefts0.size();
		int width = 0;
		for(int i = 0; i < n; i ++) width += rights0[i] - lefts0[i];
		if(width <= k) { out_val = UNDEFINED; out_k = NOTFOUND; return; }
		static vector<int> lefts, rights;	//自動変数だとメモリ確保の時間…と思ったけどこれだとどうだろう？
		lefts.assign(lefts0.begin(), lefts0.end());
		rights.assign(rights0.begin(), rights0.end());
		Val val = 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			int count = 0;
			for(int i = 0; i < n; i ++) {
				count += dicts[bit].rank(true, lefts[i], rights[i]);
			}
			bool dir = k < count;
			val = val << 1 | (dir ? 1 : 0);
			if(!dir) k -= count;
			for(int i = 0; i < n; i ++) {
				lefts[i] = dicts[bit].rank(dir, lefts[i]);
				rights[i] = dicts[bit].rank(dir, rights[i]);
				if(dir) lefts[i] += mids[bit], rights[i] += mids[bit];
			}
		}
		out_val = val; out_k = k;
	}
	//区間がかぶってるとバグる
	//区間がソートされていないとバグる
	inline IdxVal quantile_ranges_idxval(const vector<int>& lefts, const vector<int>& rights, int k) const {
		int n = lefts.size();
		Val tmp_val; int tmp_k;
		quantile_ranges(lefts, rights, k, tmp_val, tmp_k);
		for(int i = 0; i < n; i ++) {
			if(tmp_k < rights[i] - lefts[i]) {
				return IdxVal(select(tmp_val, tmp_k, lefts[i]), tmp_val);
			}
			tmp_k -= rights[i] - lefts[i];
		}
		return IdxVal(NOTFOUND, UNDEFINED);
	}
	struct Range {
		int left, right;
		int bit; Val val;
		Range(int l, int r, int b, Val v):
			left(l), right(r), bit(b), val(v) {}
	};
	//O(bitsize min(length, maxval) log min(length, maxval))?
	//priority_queueではやはり最悪ケースが…
	//でもランダムで適当(バラけすぎとかが無い)なデータに対しては結構速い
	template<typename F, typename FOut>
	int rectbfsk(const F& f, int left, int right, Val bottom, Val up, int k, FOut& out) const {
		int k0 = k;
		up = min(up, maxval+1);
		priority_queue<Range, vector<Range>, F> q(f);
		q.push(Range(left, right, 0, 0));
		while(k && !q.empty()) {
			Range t = q.top(); q.pop();
			if(t.bit == bitsize) {
				f.pushvalues(out, t, k);
			}else {
				int leftcount = dicts[t.bit].rank(false, t.left);
				int rightcount = dicts[t.bit].rank(false, t.right);
				if( rightcount - leftcount != 0 &&
					bottom <= ((t.val << (bitsize - t.bit)) | BITMASK(bitsize - t.bit - 1)))
					q.push(Range(leftcount, rightcount, t.bit + 1, t.val << 1));
				if( (t.right - t.left) - (rightcount - leftcount) != 0 &&
					(((t.val << 1 | 1) << (bitsize - t.bit - 1))) < up) {
					q.push(Range(
						(t.left - leftcount) + mids[t.bit], (t.right - rightcount)  + mids[t.bit],
						t.bit + 1, t.val << 1 | 1));
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
				(x.right - x.left == y.right - y.left && x.val > y.val);
		}
		inline void pushvalues(vector<ValCount>& out, const Range& t, int& k) const {
			out.push_back(ValCount(t.val, t.right - t.left));
			k --;
		}
	};
	inline int topk(int left, int right, Val bottom, Val up, int k, vector<ValCount>& out) const {
		return rectbfsk<FreqList,vector<ValCount> >(FreqList(), left, right, bottom, up, k, out);
	}
	template<typename F, typename FOut>
	struct DfsInfo {
		const F& f;
		FOut& out;
		Val bottom, up;
		DfsInfo(const F& f_, FOut& o, Val b, Val u): f(f_), out(o), bottom(b), up(u) {}
	};
	//O(min(k bitsize, min(length, maxval)))?
	//minじゃなくてもう少しなめらかな関数で上界得られそうだけど…kつの中で幅に入らない分は共有されるイメージ
	//でもO(k bitsize)はやはり心強いな。k=1ならO(bitsize)となるわけだし
	template<typename F, typename FOut>
	void rectdfsk_dfs(const DfsInfo<F,FOut>& info, int bit, Val val, int left, int right, int& k) const {
		if(bit == bitsize) {
			info.f.pushvalues(info.out, val, right - left, k);
			return;
		}
		int leftcount = dicts[bit].rank(left);
		int rightcount = dicts[bit].rank(right);
		if(F::MAXF) {
			if(k > 0 && rightcount - leftcount != 0 &&
				(((val << 1 | 1) << (bitsize - bit - 1)) < info.up))
				rectdfsk_dfs<F,FOut>(info, bit+1, val << 1 | 1, leftcount + mids[bit], rightcount + mids[bit], k)
			if( k > 0 && (right - left) - (rightcount - leftcount) != 0 &&
				(info.bottom <= ((val << (bitsize - bit)) | BITMASK(bitsize - bit - 1))))
				rectdfsk_dfs<F,FOut>(info, bit+1, val << 1, left - leftcount, right - rightcount, k)
		}else {
			if( k > 0 && (right - left) - (rightcount - leftcount) != 0 &&
				(info.bottom <= ((val << (bitsize - bit)) | BITMASK(bitsize - bit - 1))))
				rectdfsk_dfs<F,FOut>(info, bit+1, val << 1, left - leftcount, right - rightcount, k)
			if(k > 0 && rightcount - leftcount != 0 &&
				(((val << 1 | 1) << (bitsize - bit - 1)) < info.up))
				rectdfsk_dfs<F,FOut>(info, bit+1, val << 1 | 1, leftcount + mids[bit], rightcount + mids[bit], k)
		}
	}
	template<bool maxf>
	struct MinMaxList {
		enum { MAXF = maxf };
		inline void pushvalues(vector<ValCount>& out, Val val, int count, int& k) const {
			out.push_back(ValCount(val, min(k, count)));
			k -= min(k, count);
		}
	};
	template<typename F, typename FOut>
	int rectdfsk(const F& f, int left, int right, Val bottom, Val up, int k, FOut& out) const {
		rectdfsk_dfs<F,FOut>(DfsInfo<F,FOut>(f, out, bottom, up), 0, 0, left, right, k);
		return k;
	}
	template<bool maxf>
	int rectminmaxk(int left, int right, Val bottom, Val up, int k, vector<ValCount>& out) const {
		return rectdfsk<MinMaxList<maxf>,vector<ValCount> >(MinMaxList<maxf>(), left, right, bottom, up, k, out);
	}
};
ostream& operator<<(ostream& o, const WaveletMatrix::IdxVal& idxval) {
	return o << "(" << idxval.idx << ": " << idxval.val << ")";
}

#include <cstdlib>
#include <ctime>
u32 xor128() {
	static u32 x = 123456789;
	static u32 y = 362436069;
	static u32 z = 521288629;
	static u32 w = 88675123;
	u32 t;
 
	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}
int main() {
	WaveletMatrix::Val a[] = {0,7,2,1,4,3,6,7,2,5,0,4,7,2,6,3};
	vector<WaveletMatrix::Val> v(a, a+sizeof(a)/sizeof(*a));
	WaveletMatrix w; w.init(v);
	vector<int> lefts(2), rights(2);
	lefts[1] = 0; rights[1] = 5;
	lefts[0] = 8; rights[0] = 14;
	vector<pair<WaveletMatrix::Val,int> > t;
	for(size_t i = 0; i < lefts.size(); i ++) {
		for(int j = lefts[i]; j < rights[i]; j ++) {
			t.push_back(make_pair(a[j], - j));
		}
	}
	sort(t.begin(), t.end(), greater<pair<WaveletMatrix::Val,int> >());
	for(size_t i = 0; i < t.size(); i ++) {
		cout << i << ": " << WaveletMatrix::IdxVal(- t[i].second, t[i].first) << endl;
	}
	cout << "=== quantile_ranges ===" << endl;
	for(size_t k = 0; k < t.size()+1; k ++) {
		cout << k << ": " << w.quantile_ranges_idxval(lefts, rights, k) << endl;
	}
	while(getchar()!='\n');
	return 0;
}



/*
	Rankのみバージョン(+rangefreq)
*/
typedef unsigned int u32;
inline int popcount(u32 x) {
	x = x - ((x >> 1) & 0x55555555); 
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	return ((x + (x >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}

struct RankDictionary {
	static const int NOTFOUND = -1;
	int length, blockslength, count;
	vector<u32> blocks; vector<int> ranktable;
	RankDictionary(int len): length(len) {
		blocks.resize((blockslength = (len + 31) / 32) + 1);
	}
	inline void set(int i) { blocks[i / 32] |= 1 << i % 32; }
	void build() {
		if(length == 0) { count = 0; return; }
		ranktable.assign(blockslength + 1, 0);
		int prev0 = 0, prev1 = 0, count0 = 0, count1 = 0;
		for(int i = 0; i < blockslength; i ++) {
			ranktable[i] = count1;
			count1 += popcount(blocks[i]);
			count0 = 32 * (i + 1) - count1;
		}
		ranktable[blockslength] = count1;
		count = count1;
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
struct WaveletMatrix {
	typedef unsigned int Val;
	int length, bitsize; Val maxval;
	vector<RankDictionary> dicts;
	vector<int> mids;
	void init(const vector<Val>& data) {
		length = data.size();
		maxval = *max_element(data.begin(), data.end());
		if(Val(1) << (8 * sizeof(Val) - 1) <= maxval) bitsize = 8 * sizeof(Val);
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
	int rank_all(Val val, int left, int right, int& out_lt, int& out_gt) const {
		if(val > maxval) { out_lt = right - left; out_gt = 0; return 0; }
		out_lt = out_gt = 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			bool dir = val >> (bitsize - bit - 1) & 1;
			int leftcount = dicts[bit].rank(dir, left), rightcount = dicts[bit].rank(dir, right);
			(dir ? out_lt : out_gt) += (right - left) - (rightcount - leftcount);
			left = leftcount, right = rightcount;
			if(dir) left += mids[bit], right += mids[bit];
		}
		return right - left;
	}
	inline int rank_lt(Val val, int left, int right) const {
		if(val > maxval) { return right - left; }
		int lt = 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			bool dir = val >> (bitsize - bit - 1) & 1;
			int leftcount = dicts[bit].rank(dir, left), rightcount = dicts[bit].rank(dir, right);
			if(dir) lt += (right - left) - (rightcount - leftcount);
			left = leftcount, right = rightcount;
			if(dir) left += mids[bit], right += mids[bit];
		}
		return lt;
	}
	inline int rank_ge(Val val, int left, int right) const {
		if(val > maxval) { return 0; }
		int ge = 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			bool dir = val >> (bitsize - bit - 1) & 1;
			int leftcount = dicts[bit].rank(dir, left), rightcount = dicts[bit].rank(dir, right);
			if(!dir) ge += (right - left) - (rightcount - leftcount);
			left = leftcount, right = rightcount;
			if(dir) left += mids[bit], right += mids[bit];
		}
		return ge + (right - left);
	}
	inline int rangefreq(int left, int right, Val bottom, Val up) {
		return rank_lt(up, left, right) - rank_lt(bottom, left, right);
	}
};


//popcountを全てメモするバージョン
struct RankDictionaryAllMemo {
	static const int NOTFOUND = -1;
	int length, blockslength, count;
	vector<u32> blocks; vector<int> ranktable;
	RankDictionaryAllMemo(int len): length(len) {
		blocks.resize((blockslength = (len + 31) / 32) + 1);
	}
	inline void set(int i) { blocks[i / 32] |= 1 << i % 32; }
	void build() {
		if(length == 0) { count = 0; return; }
		ranktable.assign(length + 1, 0);
		int count1 = 0;
		for(int i = 0; i < length; i ++) {
			ranktable[i] = count1;
			count1 += (blocks[i / 32] >> i % 32) & 1;
		}
		ranktable[length] = count1;
		count = count1;
	}
	inline int rank(int pos) const {	//[0..pos)の1の個数
		return ranktable[pos];
	}
	inline int rank(bool b, int pos) const { return b ? rank(pos) : pos - rank(pos); }
	inline int rank(bool b, int left, int right) const { return rank(b, right) - rank(b, left); }
};












/////////////////////////////////////////////////////////
//Sufficientじゃない、Treap実装の永続動的Wavelet行列
unsigned xor128() {
	static unsigned x = (unsigned)time(0), y = 362436069, z = 521288629, w = 88675123;
	unsigned t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

template<typename Val>
struct PersistentTreap {
	typedef typename Val::Measured Measured;
	static const int HeapSize = (int)(1024*1024*1024*1.0/4/6);

	class Node;
	typedef const Node *Ref;
	class Node {
		friend PersistentTreap;
		unsigned priority;
		Ref left, right;
		int size;
		Val val;
		Measured measured;

		Node() {}
		Node(unsigned p, Ref l, Ref r, int s, const Val &v, const Measured &m):
			priority(p), left(l), right(r), size(s), val(v), measured(m) {}
	};
private:
	static Node heap[HeapSize]; static int heapindex;

	static Ref node(const Val &val, Ref left, Ref right) {
		if(heapindex == HeapSize) {
			cerr << "Out of Memory!!!!! at i = " << glb_i << endl;
			abort();
		}
		return new(&heap[heapindex++])
			Node(xor128(), left, right, size(left) + 1 + size(right), val,
				measure(left) + val.measure() + measure(right));
	}

	static Ref linkL(Ref p, Ref c) { return node(p->val, c, p->right); }
	static Ref linkR(Ref p, Ref c) { return node(p->val, p->left, c); }
	
	static Ref rotateL(Ref t) {
		return linkL(t->right, linkR(t, t->right->left));
	}
	static Ref rotateR(Ref t) {
		return linkR(t->left, linkL(t, t->left->right));
	}

public:
	static int size(Ref t) {
		return !t ? 0 : t->size;
	}
	static Measured measure(Ref t) {
		return !t ? Measured() : t->measured;
	}

	static Ref merge(Ref x, Ref y) {
		if(x == NULL) return y;
		if(y == NULL) return x;
		if(x->priority < y->priority)
			return linkR(x, merge(x->right, y));
		else
			return linkL(y, merge(x, y->left));
	}
	
	typedef pair<Ref, Ref> RefPair;
	static RefPair split(Ref t, int k) {
		if(t == NULL) return RefPair((Ref)NULL, (Ref)NULL);
		int s = size(t->left);
		if(k <= s) {
			RefPair u = split(t->left, k);
			return RefPair(u.first, linkL(t, u.second));
		}else {
			RefPair u = split(t->right, k - s - 1);
			return RefPair(linkR(t, u.first), u.second);
		}
	}

	static Ref insert(Ref t, int k, const Val &x) {
		if(t == NULL) return singleton(x);
		int s = size(t->left);
		if(k <= s) {
			t = linkL(t, insert(t->left, k, x));
			if(t->priority > t->left->priority) t = rotateR(t);
		}else {
			t = linkR(t, insert(t->right, k - s - 1, x));
			if(t->priority > t->right->priority) t = rotateL(t);
		}
		return t;
	}

	static Ref erase(Ref t, int k) {
		if(t == NULL) return NULL;
		int s = size(t->left);
		if(k < s) return linkL(t, erase(t->left, k));
		if(k > s) return linkR(t, erase(t->right, k - s - 1));
		if(t->left == NULL && t->right == NULL) return NULL;
		else if(t->left == NULL) t = rotateL(t);
		else if(t->right == NULL) t = rotateR(t);
		else t = (t->left->priority >= t->right->priority ? rotateL : rotateR)(t);
		return erase(t, k);
	}

	static Ref empty() { return NULL; }
	static Ref singleton(const Val &x) { return node(x, NULL, NULL); }

	static const Val &access(Ref t, int k) {
		int s = size(t->left);
		if(k < s) return access(t->left, k);
		if(k > s) return access(t->right, k - s - 1);
		return t->val;
	}

	static Measured sum(Ref t, int r) {	//区間sum。半開区間[0, r)
		if(t == NULL) return Measured();
		int s = size(t->left);
		if(r < s)
			return sum(t->left, r);
		if(r > s)
			return measure(t->left) + t->val.measure() + sum(t->right, r - s - 1);
		return measure(t->left);
	}

	static void toList(Ref t, vector<Val> &out_v) {
		if(t == NULL) return;
		toList(t->left, out_v);
		out_v.push_back(t->val);
		toList(t->right, out_v);
	}

	static ostream &debugShow(ostream &o, Ref t) {
		vector<Val> v;
		toList(t, v);
		o << "[ ";
		each(i, v) {
			o << *i;
			if(i+1 != v.end()) o << ", ";
		}
		o << " ]";
		return o;
	}
};
template<typename Val>
typename PersistentTreap<Val>::Node PersistentTreap<Val>::heap[PersistentTreap<Val>::HeapSize];
template<typename Val>
int PersistentTreap<Val>::heapindex;

class PersistentDynamicWaveletMatrix {
	struct Bit {
		struct Measured {
			int sum;
			Measured(): sum(0) {}
			Measured(int su): sum(su) {}
			inline Measured operator+(const Measured &that) const {
				return Measured(sum + that.sum);
			}
		};
		bool b;
		Bit(): b(0) {}
		Bit(bool b_): b(b_) {}
		Measured measure() const {
			return Measured(b);
		}
	};
	typedef PersistentTreap<Bit> BitSeq;
public:
	typedef const PersistentDynamicWaveletMatrix *Ref;
	typedef unsigned long long Val;
private:
	vector<BitSeq::Ref> dicts;
	vector<int> mids;

	PersistentDynamicWaveletMatrix() {}
	PersistentDynamicWaveletMatrix(int bitsize):
		dicts(bitsize, BitSeq::empty()), mids(bitsize, 0) {}

	static int rangeSum(BitSeq::Ref t, bool b, int right) {
		int x = BitSeq::sum(t, right).sum;
		return b ? x : right - x;
	}
	static int rangeSum(BitSeq::Ref t, bool b, int left, int right) {
		return rangeSum(t, b, right) - rangeSum(t, b, left);
	}
public:
	static int bitsize(Ref w) { return w->dicts.size(); }
	static int size(Ref w) { return BitSeq::size(w->dicts[0]); }

	static Ref insert(Ref w, int k, Val x) {
		PersistentDynamicWaveletMatrix *u = new PersistentDynamicWaveletMatrix;
		int n = bitsize(w);
		u->dicts.resize(n); u->mids.resize(n);
		rep(i, n) {
			bool b = x >> (n - i - 1) & 1;
			u->dicts[i] = BitSeq::insert(w->dicts[i], k, b);
			u->mids[i] = w->mids[i] + !b;
			k = rangeSum(w->dicts[i], b, k);
			if(b) k += u->mids[i];
		}
		return u;
	}

	static Ref empty(int bitsize) {
		return new PersistentDynamicWaveletMatrix(bitsize);
	}

	static Val access(Ref w, int k) {
		int n = bitsize(w);
		Val val = 0;
		rep(i, n) {
			bool b = BitSeq::access(w->dicts[i], k).b;
			val = val << 1 | (b ? 1 : 0);
			k = rangeSum(w->dicts[i], b, k);
			if(b) k += w->mids[i];
		}
		return val;
	}

	static int rank_all(Ref w, Val val, int left, int right, int &out_lt) {
		int n = bitsize(w);
		out_lt = 0;
		rep(i, n) {
			bool b = val >> (n - i - 1) & 1;
			int leftcount = rangeSum(w->dicts[i], b, left);
			int rightcount = rangeSum(w->dicts[i], b, right);
			if(b) out_lt += (right - left) - (rightcount - leftcount);
			left = leftcount, right = rightcount;
			if(b) left += w->mids[i], right += w->mids[i];
		}
		return right - left;
	}

	//a[lefta, righta) - b[leftb, rightb] のk番目
	static Val different_quantile(Ref a, Ref b, int lefta, int righta, int leftb, int rightb, int k) {
		if((righta - lefta) - (rightb - leftb) <= k) return Val(-1);
		int n = bitsize(a);
		Val val = 0;
		rep(i, n) {
			int counta = rangeSum(a->dicts[i], false, lefta, righta),
				countb = rangeSum(b->dicts[i], false, leftb, rightb);
			int count = counta - countb;
			bool dir = count <= k;
			val = val << 1 | (dir ? 1 : 0);
			if(dir) k -= count;
			lefta = rangeSum(a->dicts[i], dir, lefta);
			righta = rangeSum(a->dicts[i], dir, righta);
			leftb = rangeSum(b->dicts[i], dir, leftb);
			rightb = rangeSum(b->dicts[i], dir, rightb);
			if(dir) {
				lefta += a->mids[i], righta += a->mids[i];
				leftb += b->mids[i], rightb += b->mids[i];
			}
		}
		return val;
	}

	static void toList(Ref w, vector<Val> &out_v) {
		int n = size(w);
		out_v.resize(n);
		rep(i, n)
			out_v[i] = access(w, i);
	}

	static void debugShow(ostream &o, Ref w) {
		int n = bitsize(w);
		rep(i, n) {
			BitSeq::debugShow(o, w->dicts[i]);
			o << ", " << w->mids[i] << endl;
		}
	}

	friend ostream &operator<<(ostream &, const Bit &);
};


ostream &operator<<(ostream &o, const PersistentDynamicWaveletMatrix::Bit &v) {
	return o << v.b;
}

ostream &operator<<(ostream &o, PersistentDynamicWaveletMatrix::Ref w) {
	vector<PersistentDynamicWaveletMatrix::Val> v;
	PersistentDynamicWaveletMatrix::toList(w, v);
	o << "[ ";
	each(i, v) {
		o << *i;
		if(i+1 != v.end()) o << ", ";
	}
	o << " ]";
	return o;
}

void PDWMdebugOut(PersistentDynamicWaveletMatrix::Ref w) {
	PersistentDynamicWaveletMatrix::debugShow(cout, w);
}



////////////////////////////////////////////////////////
//再帰的なWaveletMatrix。
//多次元クエリがたぶん可能だが時間・メモリ共にΘ(n log^d n)という…

typedef unsigned int u32;
inline int popcount(u32 x) {
	x = x - ((x >> 1) & 0x55555555); 
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	return ((x + (x >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}

struct RankDictionary {
	static const int NOTFOUND = -1;
	int length, blockslength, count;
	vector<u32> blocks; vector<int> ranktable;
	RankDictionary(int len): length(len) {
		blocks.resize((blockslength = (len + 31) / 32) + 1);
	}
	inline void set(int i) { blocks[i / 32] |= 1 << i % 32; }
	void build() {
		if(length == 0) { count = 0; return; }
		ranktable.assign(blockslength + 1, 0);
		count = 0;
		for(int i = 0; i < blockslength; i ++) {
			ranktable[i] = count;
			count += popcount(blocks[i]);
		}
		ranktable[blockslength] = count;
	}
	inline int rank(int pos) const {	//[0..pos)の1の個数
		int block_idx = pos / 32;
		return ranktable[block_idx] + popcount(blocks[block_idx] & (1U << pos % 32)-1);
	}
	inline int rank(bool b, int pos) const { return b ? rank(pos) : pos - rank(pos); }
	inline int rank(bool b, int left, int right) const { return rank(b, right) - rank(b, left); }
};

template<typename T>
struct UnitStructure {
	UnitStructure() {}
	UnitStructure(T) {}
};

template<int Level, int Lv = 0>
struct WaveletMatrixPower {
	enum { level = Lv };
	typedef unsigned int Val;
	typedef WaveletMatrixPower<Level, Lv+1> NextLevel;
	int length, bitsize; Val maxval;
	vector<RankDictionary> dicts;
	vector<int> mids;
	vector<NextLevel> nextlevels;

	WaveletMatrixPower<Level, Lv>(const vector<Val*>& data) { init(data); }
	void init(const vector<Val*>& data) {
		length = data.size();
		maxval = 0;
		for(vector<Val*>::const_iterator it = data.begin(); it != data.end(); ++ it)
			maxval = max(maxval, (*it)[level]);	//ここ無駄
		if(Val(1) << (8 * sizeof(Val) - 1) <= maxval) bitsize = 8 * sizeof(Val);
		else for(bitsize = 0; Val(1) << bitsize <= maxval; bitsize ++) ;
		dicts.assign(bitsize, length);
		mids.assign(bitsize, 0);
		nextlevels.clear(); nextlevels.reserve(bitsize);

		vector<Val*> datacurrent(data), datanext(length);
		for(int bit = 0; bit < bitsize; bit ++) {
			int pos = 0;
			for(int i = 0; i < length; i ++)
				if((datacurrent[i][level] >> (bitsize - bit - 1) & 1) == 0)
					datanext[pos ++] = datacurrent[i];
			mids[bit] = pos;
			for(int i = 0; i < length; i ++)
				if((datacurrent[i][level] >> (bitsize - bit - 1) & 1) != 0)
					dicts[bit].set(i), datanext[pos ++] = datacurrent[i];
			dicts[bit].build();
			nextlevels.push_back(datanext);
			datacurrent.swap(datanext);
		}
	}
	int rank_all(Val val, int left, int right, int& out_lt) const {
		if(val > maxval) { out_lt = right - left; return 0; }
		out_lt = 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			bool dir = val >> (bitsize - bit - 1) & 1;
			int leftcount = dicts[bit].rank(dir, left), rightcount = dicts[bit].rank(dir, right);
			if(dir) out_lt += (right - left) - (rightcount - leftcount);
			left = leftcount, right = rightcount;
			if(dir) left += mids[bit], right += mids[bit];
		}
		return right - left;
	}
	int rank_lt(Val val, Val left, Val right, const Val *a, const Val *b) const {
		if(val > maxval) return right - left;
		int lt = 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			bool dir = val >> (bitsize - bit - 1) & 1;
			int leftcount = dicts[bit].rank(dir, left), rightcount = dicts[bit].rank(dir, right);
			if(dir) {
				lt += nextlevels[bit].regionfreq(
					left - leftcount, right - rightcount, a, b);
			}
			left = leftcount, right = rightcount;
			if(dir) left += mids[bit], right += mids[bit];
		}
		return lt;
	}
	int regionfreq(Val left, Val right, const Val *a, const Val *b) const {
		return
			rank_lt(*b, left, right, a+1, b+1) -
			rank_lt(*a, left, right, a+1, b+1);
	}
	int regionfreq(const Val *a, const Val *b) const {
		return regionfreq(*a, *b, a+1, b+1);
	}
	//このレベルの[left, right)の中で、
	//これ以降のレベルで(a, b)を満たす中で、
	//このレベルの値が(k+1)番目のもの
	Val quantile(Val left, Val right, int k, const Val *a, const Val *b) const {
		Val val = 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			int count = nextlevels[bit].regionfreq(
				dicts[bit].rank(false, left),
				dicts[bit].rank(false, right), a, b);
			if(bit == 0) {
				if(count +
					nextlevels[bit].regionfreq(
					mids[bit] + dicts[bit].rank(true, left),
					mids[bit] + dicts[bit].rank(true, right), a, b) <= k)
					return Val(-1);
			}
			bool dir = count <= k;
			val = val << 1 | (dir ? 1 : 0);
			if(dir) k -= count;
			left = dicts[bit].rank(dir, left), right = dicts[bit].rank(dir, right);
			if(dir) left += mids[bit], right += mids[bit];
		}
		return val;
	}
};

template<int Level>
struct WaveletMatrixPower<Level, Level> {
	enum { level = 0 };
	typedef unsigned int Val;
	WaveletMatrixPower(const vector<Val*>&) { }
	int regionfreq(Val left, Val right, const Val *a, const Val *b) const {
		return right - left;
	}
};



//with cumsum?
typedef unsigned int u32;
inline int popcount(u32 x) {
	x = x - ((x >> 1) & 0x55555555); 
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	return ((x + (x >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}

struct RankDictionary {
	static const int NOTFOUND = -1;
	int length, blockslength, count;
	vector<u32> blocks; vector<int> ranktable;
	RankDictionary(int len): length(len) {
		blocks.resize((blockslength = (len + 31) / 32) + 1);
	}
	inline void set(int i) { blocks[i / 32] |= 1 << i % 32; }
	void build() {
		count = 0;
		if(length == 0) return;
		ranktable.assign(blockslength + 1, 0);
		for(int i = 0; i < blockslength; i ++) {
			ranktable[i] = count;
			count += popcount(blocks[i]);
		}
		ranktable[blockslength] = count;
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
struct WaveletMatrixWithCumSum {
	typedef unsigned long long Val;
	typedef unsigned int SumVal;
	int length, bitsize; Val maxval;
	vector<RankDictionary> dicts;
	vector<vector<SumVal> > cumsum;
	vector<int> mids;
	void init(const vector<Val>& data, const vector<SumVal> &sumdata) {
		length = data.size();
		maxval = *max_element(data.begin(), data.end());
		if(Val(1) << (8 * sizeof(Val) - 1) <= maxval) bitsize = 8 * sizeof(Val);
		else for(bitsize = 0; Val(1) << bitsize <= maxval; bitsize ++) ;
		dicts.assign(bitsize, length);
		mids.assign(bitsize, 0);
		cumsum.assign(bitsize + 1, vector<SumVal>(length + 1, 0));
		vector<int> datacurrent(length), datanext(length);
		for(int i = 0; i < length; i ++) datacurrent[i] = i;
		for(int bit = 0; bit < bitsize; bit ++) {
			for(int i = 0; i < length; i ++)
				cumsum[bit][i+1] = cumsum[bit][i] + sumdata[datacurrent[i]];
			int pos = 0;
			for(int i = 0; i < length; i ++)
				if((data[datacurrent[i]] >> (bitsize - bit - 1) & 1) == 0)
					datanext[pos ++] = datacurrent[i];
			mids[bit] = pos;
			for(int i = 0; i < length; i ++)
				if((data[datacurrent[i]] >> (bitsize - bit - 1) & 1) != 0)
					dicts[bit].set(i), datanext[pos ++] = datacurrent[i];
			dicts[bit].build();
			datacurrent.swap(datanext);
		}
		for(int i = 0; i < length; i ++)
			cumsum[bitsize][i+1] = cumsum[bitsize][i] + sumdata[datacurrent[i]];
	}
	inline SumVal getSum(int bit, int left, int right) const {
		return cumsum[bit][right] - cumsum[bit][left];
	}
	SumVal sum_lt(Val val, int left, int right) const {
		if(val > maxval) { return getSum(0, left, right); }
		SumVal ltsum = 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			bool dir = val >> (bitsize - bit - 1) & 1;
			int leftcount = dicts[bit].rank(dir, left), rightcount = dicts[bit].rank(dir, right);
			if(dir)
				ltsum += getSum(bit+1, left - leftcount, right - rightcount);
			left = leftcount, right = rightcount;
			if(dir) left += mids[bit], right += mids[bit];
		}
		return ltsum;
	}
	Val quantile(int left, int right, int k) const {
		if(right - left <= k) { return (Val)-1; }
		Val val = 0;
		for(int bit = 0; bit < bitsize; bit ++) {
			int count = dicts[bit].rank(true, left, right);
			bool dir = k < count;
			val = val << 1 | (dir ? 1 : 0);
			if(!dir) k -= count;
			left = dicts[bit].rank(dir, left), right = dicts[bit].rank(dir, right);
			if(dir) left += mids[bit], right += mids[bit];
		}
		return val;
	}
};

