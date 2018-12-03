template<typename T> struct GetID {
	map<T,int> ids;
	void clear() { ids.clear(); }
	int get(const T &x) {
		typename map<T,int>::iterator it = ids.find(x);
		if(it != ids.end()) return it->second;
		int newID = ids.size();
		ids.insert(make_pair(x, newID));
		return newID;
	}
};

//Trieを使ったタイプ
struct GetStrID {
	static const int MaxLen = 1000;	//書き換えること！
	static const int Alphas = 52;	//書き換えること！
	typedef char Alpha;
	static const int EndAlpha = -1;

private:
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

public:
	GetStrID(const char *alphas): cnt(0) {
		assert(strlen(alphas) <= Alphas);
		memset(idx, -1, sizeof(idx));
		for(const char *p = alphas; *p; ++ p)
			idx[*p] = p - alphas;
	}
	char idx[128];
	Trie<int> trie;
	int cnt;

	void clear() { trie = Trie<int>(); cnt = 0; }
	int get(const char *x) {
		static char tmp[MaxLen+1];
		{	int i;
			for(i = 0; x[i]; i ++) tmp[i] = idx[x[i]];
			tmp[i] = EndAlpha;
		}
		int ret = -1;
		if(!trie.find(tmp, ret)) {
			ret = cnt ++;
			trie.insert(tmp, ret);
		}
		return ret;
	}
};
