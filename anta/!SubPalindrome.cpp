//distinctな部分回分文字列を全て求める。その数はn個以下
//Theorem: 部分回文文字列は全てleft-maximalな部分回文文字列である。

template<unsigned p>
struct RollingHash {
	int n;
	vector<unsigned long long> pow, phash;
	RollingHash(const char *s, int n_): n(n_), pow(n+1), phash(n+1) {
		pow[0] = 1; phash[0] = 0;
		rep(i, n) {
			phash[i+1] = s[i] + phash[i] * p;
			pow[i+1] = pow[i] * p;
		}
	}
	unsigned long long h(int i) const {
		return phash[i];
	}
	unsigned long long h(int i, int j) const {
		return h(j) - h(i) * pow[j - i];
	}
};


typedef char Alpha;

std::vector<int> getDiameters(const Alpha *str, int n) {
	std::vector<int> diameters(n * 2 - 1);
	int center = 0, right = 0;
	for(int i = 0; i < n * 2 - 1; i ++) {
		int maxp = std::min(i + 1, n * 2 - 1 - i);
		int p = right > i ? std::min(right - i, diameters[2 * center - i]) : 0;
		if((i + p) % 2 == 0) p ++;
		while(p < maxp && str[(i + p + 1) / 2] == str[(i - p - 1) / 2]) p += 2;
			p = std::min(p, maxp);
			if(i + p > right) {
				center = i;
				right = i + p;
			}
		diameters[i] = p;
	}
return diameters;
}

int countDistinctSubpalindromes(const char *s, int n) {
	vector<int> diameters = getDiameters(s, n);
	vector<pii> v;
	rep(i, n * 2 - 1) {
		int d = diameters[i];
		if(d == 0) continue;
		int left = (i - d + 1) / 2, right = left + d;
		v.pb(mp(-right, left));
	}
	sort(all(v));
	vector<int> maximal(n+1, INF);
	{	int left = INF, right = n;
		rep(i, v.size()) {
			int r = -v[i].first;
			while(r < right) {
				maximal[right] = min(maximal[right], left);
				right --; left ++;
			}
			left = min(left, v[i].second);
			maximal[right] = min(maximal[right], left);
		}
	}
	set<pair<unsigned long long,unsigned long long> > hashes;
	RollingHash<1000000007> h1(s, n);
	RollingHash<1000000009> h2(s, n);
	rer(i, 0, n) {
		int left = maximal[i];
		if(left >= i) continue;
		pair<unsigned long long,unsigned long long> h = mp(h1.h(left, i), h2.h(left, i));
//		if(!hashes.count(h)) cerr << string(s+left, s+i) << endl;
		hashes.insert(h);
	}
	return hashes.size();
}