//グラフイテレータインターフェイスの1つ
//<http://hbpc2012.contest.atcoder.jp/submissions/134822>
struct SemiImplicitGraph {	//edgeBegin(v)のたびに出てく辺を全部事前計算するもの。BFSだったりDFSでも高さが抑えられる場合に(メモリ的に)有効
	typedef int Vertex;
	struct To {
		Vertex to;
		To() { }
		To(Vertex to_): to(to_) { }
		const To *operator->() const { return this; }	//it->toとアクセスさせるために
	};
public:
	typedef struct EdgeIterator {
		friend SemiImplicitGraph;
		typedef int Index;
		inline EdgeIterator &operator++() {
			if((++ i) == tos.size()) i = -1;
			return *this;
		}
		inline const To &operator*() const {
			return tos[i];
		};
		inline const To &operator->() const { return operator*(); }
		inline EdgeIterator operator++(int) { EdgeIterator t = *this; ++ *this; return t; };
		//他にもランダムアクセスな演算子も定義できるけど使わないのでとりあえず書かない
		inline bool operator==(const EdgeIterator &that) const { return i == that.i; }
		inline bool operator!=(const EdgeIterator &that) const { return i != that.i; }
	private:
		struct end_tag { };
		EdgeIterator(Vertex v) { calcEdges(v, tos); i = tos.size() == 0 ? -1 : 0; }	//begin()
		EdgeIterator(Vertex v, end_tag) { i = -1; }	//end()。これはループ毎に呼び出されてそのたびにcalcEdgeすると重いのでとりあえず-1にしておく
		int i; vector<To> tos;
	} const_iterator;

	void init(int n_) { n = n_; }
	int numVertices() const { return n; }

	inline const_iterator edgesBegin(Vertex v) const { return EdgeIterator(v); }
	inline const_iterator edgesEnd(Vertex v) const { return EdgeIterator(v, EdgeIterator::end_tag()); }
private:
	int n;
	static void calcEdges(Vertex v, vector<To> &tos);	//これを適切に実装すること
};
typedef SemiImplicitGraph Graph;
