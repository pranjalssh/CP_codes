typedef char Alpha;
const Alpha EndAlpha = -1;
const int Alphas = 26;

template<typename T>
struct Trie {
	bool terminate;
	T val;
	Trie<T> *next[Alphas];

	Trie(): terminate(false) { memset(next, 0, sizeof(next)); }
	~Trie() { for(Alpha a = 0; a < Alphas; ++ a) delete next[a]; }

	void insert(Alpha *s, const T &v) {
		if(*s == EndAlpha) {
			terminate = true;
			val = v;
			return;
		}else {
			if(!next[*s])
				next[*s] = new Trie;
			return next[*s]->insert(s+1, v);
		}
	}
	bool find(Alpha *s, T &out_v) const {
		if(*s == EndAlpha) {
			if(terminate) {
				out_v = val;
				return true;
			}else return false;
		}
		return next[*s] && next[*s]->find(s+1, out_v);
	}
};
