//[a,b]と[c,d]が共通する点を持つか？
inline bool commonPoint1D(int a, int b, int c, int d) {
	return c <= b && a <= d;
}
//[a,b]と[c,d]が共通する線分を持つか？
inline bool commonSeg1D(int a, int b, int c, int d) {
	return c < b && a < d;
}

typedef pair<int,int> Seg1D;
inline bool commonPoint1D(const Seg1D &x, const Seg1D &y) {
	return y.first <= x.second && x.first <= y.second;
}
inline bool commonSeg1D(const Seg1D &x, const Seg1D &y) {
	return y.first < x.second && x.first < y.second;
}

typedef pair<int,int> Seg1D;

inline Seg1D intersectSeg1D(const Seg1D &a, const Seg1D &b) {
	return Seg1D(max(a.first, b.first), min(a.second, b.second));
}
inline int lengthSeg1D(const Seg1D &a) {
	if(a.first >= a.second) return 0;
	else return a.second - a.first;
}

typedef pair<Seg1D,Seg1D> Seg2D;

inline Seg2D intersectSeg2D(const Seg2D &a, const Seg2D &b) {
	return Seg2D(intersectSeg1D(a.first, b.first), intersectSeg1D(a.second, b.second));
}
inline long long lengthSeg2D(const Seg2D &a) {
	return (long long)lengthSeg1D(a.first) * lengthSeg1D(a.second);
}

