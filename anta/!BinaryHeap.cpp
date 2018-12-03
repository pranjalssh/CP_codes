//Verified: <http://qupc2014.contest.atcoder.jp/submissions/150322>
//普通ヒープでもupdateはgetMin()をならしO(1)にすることでエミュレートできる。
//	それに比べて利点は1.メモリ削減, 2.ならしでなく実時間
//	速度比較はしていないが、場合によってpriority_queueでエミュレートしたほうが速いかもしれないので試してみること！
//	エミュレートするには:
//		1. ヒープの要素は(値,index)とする
//		2. 各indexに本来の値を別個に持っておく
//		3. getMin()では、返り値が本来の値と違う限りpop()し続ける

template<typename Value, typename Compare = std::less<Value>, typename Index = int>
class BinaryHeap {
	Compare cmp;
	vector<Value> a;
	vector<Index> idx, nodeidx;
	int pos;

public:
	BinaryHeap(Compare cmp_ = Compare()): cmp(cmp_) { }
	BinaryHeap(int n, Compare cmp_ = Compare()): cmp(cmp_) { init(n); }

	void init(int n) {
		a.resize(n+1);
		idx.assign(n+1, -1);
		nodeidx.assign(n+1, -1);
		pos = 1;
	}

	const Value &min() const { return a[1]; }
	const Value &get(Index i) const { return a[nodeidx[i]]; }
	Index argmin() const { return idx[1]; }
	Index size() const { return pos - 1; }
	bool empty() const { return pos == 1; }

	const Value &add(Index i, const Value &x) {
		Index node = nodeidx[i];
		if(node >= 0) return a[node];
		a[pos] = x;
		idx[pos] = i;
		nodeidx[i] = pos;
		++ pos;
		up(pos-1);
		return x;
	}

	const Value &update(Index i, const Value &x) {
		Index node = nodeidx[i];
		if(node < 0) {
			a[pos] = x;
			idx[pos] = i;
			nodeidx[i] = pos;
			++ pos;
			up(pos-1);
		}else {
			a[node] = x;
			up(node);
			down(node);
		}
		return x;
	}

	const Value &updatemin(Index i, const Value &x) {
		Index node = nodeidx[i];
		if(node >= 0 && !cmp(x, a[node])) return a[node];
		else return update(i, x);
	}

	const Value remove(Index i) {
		if(i < 0) return Value();
		Index node = nodeidx[i];
		if(node < 0) return Value();
		-- pos;
		idx[node] = idx[pos];
		nodeidx[idx[pos]] = node;
		nodeidx[i] = -1;
		idx[pos] = -1;
		swap(a[pos], a[node]);
		up(node);
		down(node);
		return a[pos];
	}

private:
	void up(Index i) {
		for(Index c = i, p = c >> 1; p > 0 && cmp(a[c], a[p]); c >>= 1, p >>= 1) {
			swap(a[p], a[c]);
			swap(nodeidx[idx[p]], nodeidx[idx[c]]);
			swap(idx[p], idx[c]);
		}
	}

	void down(Index i) {
		Index bottom = pos;
		for(Index c = i; c * 2 < bottom; ) {
			Index b = c * 2 + (c * 2 + 1 < bottom && cmp(a[c * 2 + 1], a[c * 2]));
			if(!cmp(a[b], a[c])) break;
			swap(a[c], a[b]);
			swap(nodeidx[idx[c]], nodeidx[idx[b]]);
			swap(idx[c], idx[b]);
			c = b;
		}
	}
};
