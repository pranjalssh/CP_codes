template<unsigned P>
struct UnorderedHash {
	unsigned long long x;
	vector<unsigned long long> p;
	UnorderedHash(int n): x(0) {
		p.assign(n, 0);
		p[0] = 1;
		reu(i, 1, n) p[i] = p[i-1] * P;
	}
	void add(int t) {
		x += p[t];
	}
	void sub(int t) {
		x -= p[t];
	}
	bool operator==(const UnorderedHash<P> &that) const {
		return x == that.x;
	}
};

struct UnorderedHashes {
	UnorderedHash<1000000007> h1;
	UnorderedHash<1000000009> h2;
	UnorderedHash<1000000021> h3;
	UnorderedHash<1000000033> h4;

	vector<unsigned long long> p;
	UnorderedHashes(int n): h1(n), h2(n), h3(n), h4(n) { }
	void add(int t) {
		h1.add(t);
		h2.add(t);
		h3.add(t);
		h4.add(t);
	}
	void sub(int t) {
		h1.sub(t);
		h2.sub(t);
		h3.sub(t);
		h4.sub(t);
	}
	bool operator==(const UnorderedHashes &that) const {
		return
			h1 == that.h1 &&
			h2 == that.h2 && 
			h3 == that.h3 && 
			h4 == that.h4 ;
	}
};
