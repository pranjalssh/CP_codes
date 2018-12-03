//多次元(dense)の場合は<http://judge.u-aizu.ac.jp/onlinejudge/solution.jsp?pid=1068>みたいに。メモリΘ(nm),クエリΘ(log n log m)

///////////////////////////////////////////////////////////////////////////
//get,getRange,setのみ(lazinessが無い)
//non commutativeなものにも対応(Verified)。
struct GetRangeSegmentTree {
	typedef Mat22 Val;
	static Val combineVal(const Val &x, const Val &y) { return x * y; }
	static void combinesVal(Val &x, const Val &y) { x *= y; }
	static Val identityVal() { return Mat22::identity(); } 

	vector<Val> nodes;
	int n;
	void init(int n_, const Val &v = Val()) { init(vector<Val>(n_, v)); }
	void init(const vector<Val> &u) {
		n = 1; while(n < (int)u.size()) n *= 2;
		nodes.resize(n);
		nodes.insert(nodes.end(), u.begin(), u.end());
		nodes.resize(n * 2, Val());
		for(int i = n-1; i > 0; -- i)
			nodes[i] = combineVal(nodes[i*2], nodes[i*2+1]);
	}
	Val get(int i) {
		return nodes[i + n];
	}
	Val getRange(int l, int r) const {
		Val m = identityVal();
		static int indices[100]; int k = 0;
		for(; l && l + (l&-l) <= r; l += l&-l)
			combinesVal(m, nodes[(n+l) / (l&-l)]);
		for(; l < r; r -= r&-r)
			indices[k ++] = (n+r) / (r&-r) - 1;
		while(-- k >= 0) combinesVal(m, nodes[indices[k]]);
		return m;
	}
	void set(int i, const Val &x) {
		i += n; nodes[i] = x;
		for(i >>= 1; i > 0; i >>= 1)
			nodes[i] = combineVal(nodes[i*2], nodes[i*2+1]);
	}
};


///////////////////////////////////////////////////////////////////////////
//Lazyなpropagateのみ
//だいぶ適当。整ってはないがVerifiedではある
//例 線形式: <http://www.codechef.com/viewsolution/3155123>
const int logSegsSize = 10;
Val segs[2<<logSegsSize];
bool segslazy[2<<logSegsSize];
int segN;

void rangeAdd(int ql, int qr, const Val &p, int sl = 0, int sr = segN, int si = 0) {
	if(sr <= ql || qr <= sl) ;
	else if(ql <= sl && sr <= qr) {
		segs[si] *= p;
		segslazy[si] = true;
	} else {
		if(segslazy[si]) {
			segs[si * 2 + 1] *= segs[si];
			segs[si * 2 + 2] *= segs[si];
			segslazy[si * 2 + 1] = true;
			segslazy[si * 2 + 2] = true;
			segs[si].clear();
			segslazy[si] = false;
		}
		rangeAdd(ql, qr, p, sl, (sl + sr) / 2, si * 2 + 1);
		rangeAdd(ql, qr, p, (sl + sr) / 2, sr, si * 2 + 2);
	}
}


const Val &access(int q, int sl = 0, int sr = segN, int si = 0) {
	if(sl == q && q+1 == sr) return segs[si];
	if(segslazy[si]) {
		segs[si * 2 + 1] *= segs[si];
		segs[si * 2 + 2] *= segs[si];
		segslazy[si * 2 + 1] = true;
		segslazy[si * 2 + 2] = true;
		segs[si].clear();
		segslazy[si] = false;
	}
	if(q < (sl + sr) / 2)
		return access(q, sl, (sl + sr) / 2, si * 2 + 1);
	else
		return access(q, (sl + sr) / 2, sr, si * 2 + 2);
}


///////////////////////////////////////////////////////////////////////////
//インターフェースが良くない。non commutativeなものに対しては未検証。

/*
	区間sum・区間足し込みのできるsegment tree。
	！区間足し込みを使う時はオーバーフローに注意！最大(回数×値×サイズ)になる！
	Val,Lazinessの実装：
		・Val() (デフォコン): 単位元
		・operator+=(Val&,Val)
		・Val operator+(Val,Val)
		・Laziness() (デフォコン)
		・operator+=(Val&,Laziness): Lazinessとしての扱いに注意(例えばminと足し算の違いで、この関数は足し算する)
		・operator+=(Laziness&,Laziness)
		・Laziness operator*(Laziness,int): 第2引数は区間の幅
		・Val(Val&) (コピコン)
		・Laziness(Laziness&) (コピコン)
	実際の派生クラスとしてのサンプルを多く作るべきだ。
	下にある：
		・LazySumSegmentTree: 区間sum, 区間足し込み
		・StarrySkyTree: 区間min, 区間足し込み
	さらに下に：
		・rightmost_le: 木上2分探索風の参考として
*/

template<typename Val, typename Laziness>
struct LazyPropagationSegmentTreeBase {
	vector<Val> nodes;
	vector<Laziness> laziness;
	vector<bool> islazy;
	int n;
	void init(int n_, const Val &v = Val()) { init(vector<Val>(n_, v)); }
	void init(const vector<Val> &u) {
		n = 1; while(n < (int)u.size()) n *= 2;
		nodes.resize(n);
		nodes.insert(nodes.end(), u.begin(), u.end());
		nodes.resize(n * 2, Val());
		for(int i = n-1; i > 0; -- i)
			nodes[i] = nodes[i*2] + nodes[i*2+1];
		laziness.assign(n, Laziness());
		islazy.assign(n, false);
	}
	Val get(int i) {
		static int indices[128];
		int k = getIndices(indices, i, i+1);
		propagateRange(indices, k);
		return nodes[i + n];
	}
	Val getRange(int i, int j) {
		static int indices[128];
		int k = getIndices(indices, i, j);
		propagateRange(indices, k);
		Val res = Val();
		for(int l = i + n, r = j + n; l < r; l >>= 1, r >>= 1) {
			if(l & 1) res += value(l ++);
			if(r & 1) res += value(-- r);
		}
		return res;
	}
	void set(int i, const Val &x) {
		static int indices[128];
		int k = getIndices(indices, i, i+1);
		propagateRange(indices, k);
		nodes[n + i] = x;
		mergeRange(indices, k);
	}
	void addToRange(int i, int j, const Laziness &x) {
		if(i >= j) return;
		static int indices[128];
		int k = getIndices(indices, i, j);
		propagateRange(indices, k);
		int l = i + n, r = j + n;
		if(l & 1) nodes[l ++] += x;
		if(r & 1) nodes[-- r] += x;
		for(l >>= 1, r >>= 1; l < r; l >>= 1, r >>= 1) {
			if(l & 1) islazy[l] = true, laziness[l ++] += x;
			if(r & 1) laziness[-- r] += x, islazy[r] = true;
		}
		mergeRange(indices, k);
	}
private:
	int getIndices(int indices[128], int i, int j) {
		int k = 0, l, r;
		if(i >= j) return 0;
		for(l = (n + i) >> 1, r = (n + j - 1) >> 1; l != r; l >>= 1, r >>= 1) {
			indices[k ++] = l;
			indices[k ++] = r;
		}
		for(; l; l >>= 1) indices[k ++] = l;
		return k;
	}
	void propagateRange(int indices[], int k) {
		for(int i = k - 1; i >= 0; -- i)
			propagate(indices[i]);
	}
	void mergeRange(int indices[], int k) {
		for(int i = 0; i < k; ++ i)
			merge(indices[i]);
	}
	inline void propagate(int i) {
		if(i >= n || !islazy[i]) return;
		nodes[i] += laziness[i] * width(i);
		if(i * 2 < n) {
			laziness[i * 2] += laziness[i];
			laziness[i * 2 + 1] += laziness[i];
			islazy[i * 2] = true;
			islazy[i * 2 + 1] = true;
		}else {
			nodes[i * 2] += laziness[i];
			nodes[i * 2 + 1] += laziness[i];
		}
		laziness[i] = Laziness();
		islazy[i] = false;
	}
	inline void merge(int i) {
		if(i >= n) return;
		nodes[i] = value(i * 2) + value(i * 2 + 1);
	}
	inline Val value(int i) {
		propagate(i);
		return nodes[i];
	}
	inline int width(int i) {
		i |= i >> 1;
		i |= i >> 2;
		i |= i >> 4;
		i |= i >> 8;
		i |= i >> 16;
		return n / ((i + 1) >> 1);
	}
};

template<typename Val>
struct LazySumSegmentTree : LazyPropagationSegmentTreeBase<Val,Val> { };

template<typename Val>
struct AddVal {
	Val x;
	AddVal(): x(Val()) { }
	AddVal(Val x_): x(x_) { }
	operator Val() { return x; }
	AddVal &operator+=(AddVal that) { x += that.x; return *this; }
	AddVal operator*(int w) { return *this; }
};
template<typename Val>
struct MinVal {
	Val x;
	MinVal(): x(std::numeric_limits<Val>::max()) { }
	MinVal(Val x_): x(x_) { }
	operator Val() { return x; }
	MinVal operator+(MinVal that) const { return min(x, that.x); }
	MinVal &operator+=(MinVal that) { return *this = *this + that; }
	MinVal &operator+=(AddVal<Val> y) { x += y; return *this; }
};

template<typename Val>
struct StarrySkyTree : LazyPropagationSegmentTreeBase<MinVal<Val>,AddVal<Val> > { };

int main() {
	int n = 1000000;
	StarrySkyTree<int> s;
	s.init(n, 0);
//	vector<int> v(n);
	rep(ii, 1000000) {
		int t = rand() % 4 + 1;
		int ans1 = -1, ans2 = -1;
		int i = rand() % n;
		int l = rand() % (n+1), r = rand() % (n+1);
		if(l > r) swap(l, r);
		int x = rand() % 21 - 10;
//		cerr << ii << ": " << t << " " << i << " " << l << " " << r << " " << x << endl;
		switch(t) {
		case 1:
			ans1 = s.get(i);
//			ans2 = v[i];
			break;
		case 2:
			ans1 = s.getRange(l, r);
//			ans2 = accumulate(v.begin()+l, v.begin()+r, 0);
//			ans2 = 0x7fffffff;
//			reu(k, l, r) ans2 = min(ans2, v[k]);
			break;
		case 3:
			s.set(i, x);
//			v[i] = x;
			break;
		case 4:
			s.addToRange(l, r, x);
//			reu(k, l, r) v[k] += x;
			break;
		}
//		s.propagateAll();
/*		if(ans1 != ans2) {
			cerr << ans1 << " != " << ans2 << endl;
			rep(k, n) cerr << v[k] << " "; cerr << endl;
			exit(1);
		}
*/
	}
	puts("OK");
	return 0;
}

//再帰で適当だけど一応Verified
	//木上2分探索風の参考として。非負の数に対してsum[i,right)がx以上である最右のi≦rightを返す
	int rightmost_le(int right, const Val &x) {
		if(getRange(0, right) < x) return 0;
		return min(right, rightmost_le_rec(x + getRange(right, n), 1, 0, n));
	}
	int rightmost_le_rec(const Val &x, int i, int pos, int w) {
		if(n <= i) return pos + 1;
		propagate(i);
		int mid = pos + w / 2;
		if(value(i * 2 + 1) >= x)
			return rightmost_le_rec(x, i * 2 + 1, mid, w / 2);
		else return rightmost_le_rec(x - value(i * 2 + 1), i * 2, pos, w / 2);
	}

