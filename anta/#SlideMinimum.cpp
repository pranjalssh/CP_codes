//！全く検証していない！WAった！
struct SlideMinimum {
	typedef short Val;
	static const inline Val Inf() { return std::numeric_limits<Val>::max(); }
	typedef pair<Val,Val> P;
	vector<P> s1, s2;

	SlideMinimum() { }
	inline void enqueue(Val x) {
		s2.push_back(P(x, min(x, s2.empty() ? Inf() : s2.back().second)));
	}
	inline void dequeue() {
		ensure();
		s1.pop_back();
	}
	inline Val minimum() const {
		Val res = Inf();
		if(!s1.empty()) res = min(res, s1.back().second);
		if(!s2.empty()) res = min(res, s2.back().second);
		return res;
	}
	inline Val back() const {
		return !s2.empty() ? s2.back().first : s1.front().first;
	}
	inline Val front() const {
		return !s1.empty() ? s1.back().first : s2.front().first;
	}
	bool empty() const { return s1.empty() && s2.empty(); }
private:
	inline void ensure() {
		if(!s1.empty()) return;
		Val mi = Inf();
		for(int i = (int)s2.size()-1; i >= 0; i --) {
			mi = min(mi, s2[i].first);
			s1.push_back(P(s2[i].first, mi));
		}
		s2.clear();
	}
};
