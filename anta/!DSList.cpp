template<typename T>
struct DSList {	//Disjoint-Set のつもり
	int M;
	vector<int> head, next;
	vector<T> value;
	DSList(int n, int maxm):
		M(0), head(n, -1), next(maxm), value(maxm) {}
	void insert(int i, const T &val) {
		value[M] = val;
		next[M] = head[i];
		head[i] = M ++;
	}
	T &operator[](int it) { return value[it]; }
};
#define EACH_AL(it, a, i) for(int it = (a).head[i]; it != -1; it = (a).next[it])
