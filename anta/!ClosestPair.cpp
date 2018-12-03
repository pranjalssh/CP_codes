//<http://ir5.hatenablog.com/entry/20131221/1387557630>で紹介されている「逐次構成法」。
//Verified: SRM 199 DIV1 Hard ClosestPoints (3D,擬似乱数ケース)

//とても有用な補題:
//  任意の幅cδの超立方体の中に含まれる点の個数は高々(cD+c)D。
//  ただし、δ = 最近点対の距離, D = 次元, c＞0。
//つまりδさえあっていればそれぞれに定数個しか入らないグリッド分割が可能であるということ。
//ほとんどのアルゴリズムがこれに依存している。
//なお、最近点対の個数が定数であることも言えることに注意。

#include <unordered_map>

struct P3D {
	int x[3];
	int &operator[](int i) { return x[i]; }
	int operator[](int i) const { return x[i]; }
};
bool operator<(const P3D &a, const P3D &b) {
	if(a[0] != b[0]) return a[0] < b[0];
	if(a[1] != b[1]) return a[1] < b[1];
	if(a[2] != b[2]) return a[2] < b[2];
	return false;
}
bool operator==(const P3D &a, const P3D &b) {
	return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}
ostream &operator<<(ostream &o, const P3D &p) {
	return o << "(" << p[0] << "," << p[1] << "," << p[2] << ")";
}

const int D = 3;
typedef P3D P;
typedef long long Dist;
Dist dist(const P &p, const P &q) {
	Dist d = 0;
	for(int k = 0; k < D; k ++) {
		Dist t = p[k] - q[k];
		d += t * t;
	}
	return d;
}
inline double distsqrt(Dist d) { return sqrt((double)d); }

class Grid {
public:
	struct PHash {
		size_t operator()(const P &p) const {
			size_t h = 0;
			for(int k = 0; k < D; k ++) h ^= p[k] + 0x9e3779b9 + (h << 6) + (h >> 2);
			return h;
		}
	};
	typedef unordered_multimap<P,int,PHash> Map;
	typedef typename Map::const_iterator CIt;
	Grid(): d(1) { }
	void setd_clear(double d_) {
		d = d_;
		blocks.clear();
	}
	void add(const P &p, int i) {
		blocks.insert(make_pair(getblock(p), i));
	}
	pair<CIt,CIt> equal_range(const P &p, const P &neig) {
		P block = getblock(p);
		for(int k = 0; k < D; k ++) block[k] += neig[k];
		return blocks.equal_range(block);
	}
private:
	Map blocks;
	double d;
	P getblock(const P &p) const {
		P block;
		for(int k = 0; k < D; k ++)
			block[k] = (int)(p[k] / d);
		return block;
	}
};

pair<Dist,int> closest_pair(const P points[], int N) {
	if(N <= 1) return make_pair(0, 0);
	vector<P> neighbors;
	{	P p; vector<P> next;
		for(int k = 0; k < D; k ++) p[k] = 0;
		next.push_back(p);
		for(int k = 0; k < D; k ++) {
			neighbors.clear(); neighbors.swap(next);
			for(int i = 0; i < (int)neighbors.size(); i ++) {
				for(int j = -1; j <= 1; j ++) {
					p = neighbors[i]; p[k] = j;
					next.push_back(p);
				}
			}
		}
		neighbors.swap(next);
	}
	vector<int> ord(N); for(int i = 0; i < N; i ++) ord[i] = i;
	random_shuffle(ord.begin(), ord.end());
	
	Dist mind = dist(points[ord[0]], points[ord[1]]), gridd = -1; int cnt = 0;
	for(int i = 2; i < N; i ++) {
		Dist d = dist(points[ord[0]], points[ord[i]]);
		if(mind > d) mind = d;
	}
	Grid grid;
	for(int i = 0; i < N; i ++) {
		if(gridd != mind) {
			grid.setd_clear(distsqrt(mind));
			for(int j = 0; j < i; j ++)
				grid.add(points[ord[j]], ord[j]);
			gridd = mind;
		}
		P p = points[ord[i]];
		for(int k = 0; k < (int)neighbors.size(); k ++) {
			pair<Grid::CIt,Grid::CIt> range = grid.equal_range(p, neighbors[k]);
			for(Grid::CIt it = range.first; it != range.second; ++ it) {
				P q = points[it->second]; Dist d = dist(p, q);
				if(mind > d) mind = d, cnt = 0;
				if(mind == d) cnt ++;
			}
		}
		grid.add(p, ord[i]);
	}
	return make_pair(mind, cnt);
}
