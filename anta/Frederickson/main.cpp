#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
using std::cerr; using std::endl;

//Frederickson, Greg N. "An optimal algorithm for selection in a min-heap." Information and Computation 104.2 (1993): 197-214.
//<http://www.sciencedirect.com/science/article/pii/S0890540183710308>
//(のSEL1と呼ばれている基本的な部分のみ)
//
//"clan"と呼ばれる、floor(log k)サイズのグループに(lazyに)分けて、
//それぞれで *最大値* を求めて代表(representative)とし、
//それが成すヒープ(auxiliary heap)上を普通に探索する。
//clansを適切に構築すれば、auxiliary heap上での探索でceil(k/floor(log k))回目のもののrankはk～2kとなる。
//これらを全部取り出して配列上のselection algorithmを用いればO(k)となる。
//探索部分はΘ((k/log k) log (k/log k))なので、O(k)である。
//よって、どのようにclanに分割するかが問題となる。
//
//Algorithm SEL1.
//まず、clan C_j に対する offspring os(C_j) を、{ u | v \in C_j, u \in children(v), u \notin C_j }と定義する。
//clanを構築する2つの方法を考える。
//1つ目の方法：
//	auxiliary heap上の探索でclan C_jを取り出した時、
//	os(C_j)たちを根とするsubheapsのうち小さい順のfloor(log k)個を新たなclan C_iとする。
//2つ目の方法：
//	1つ目の方法で選ばれなかったos(C_j)のノードは永久に選ばれることが無くなってしまうのは困る。
//	よって、選ばれなかったノードたちを新たに作ったclan C_iに関連付ける。
//	これをpoor relations pr(C_i)と呼ぶ。
//	auxiliary heap上の探索でclan C_jを取り出した時もしpr(C_j)が空で無ければ、
//	pr(C_j)たちを根とするsubheapsのうち小さい順のfloor(log k)個を新たなclan C_{i+1}とする。
//	このとき、pr(C_j)のうち選ばれなかった頂点たちをpr(C_{i+1})とする。
//os(C_j)もpr(C_j)もサイズが最大でも 2 floor(log k) 以下であることに注意する。
//普通のヒープで探索して構築すればO(log k log log k)時間で1つのclanが構築できる。
//ceil(k/floor(log k))個目の代表を取り出して、最後にその代表以下の全てのノードを列挙してk番目を求める。
//
//正当性：
//	ceil(k/floor(log k))個目の代表のrankは少なくともkである。
//	なぜなら、ceil(k/floor(log k))個のclanの代表はこれ以下であって、さらにそれぞれに代表以下があるから、推移的にk個得られる。	
//	また、最後の代表未満のノードは、全てにclanに属している。
//	なぜなら、もし代表未満で属していないノードがあったら、
//	まずそこから上にいってclanにぶつかった地点を考えると、heap propertyよりそのノードも代表未満であって、
//	それが上のclanに含まれていないということは、そのclanは全てそのノード以下の値しか持たないことになって、
//	そのためそのclanは代表値もそのノード以下であるが、
//	そのために最後の代表より早くextractされているはずである。
//	また、そのextractされた時や、その後のpoor relationからこのノードが選ばれていないということは、
//	ずっとこのノードより小さなノードのみで作られたclanたちが得られていたということになるが、
//	そのclanは最後の代表未満なので最後のclanでないことを考えると、永遠に続かなければおかしく、有限なので矛盾。
//計算量：
//	各clanを構築するのにΘ(log k log log k)。これをΘ(k/log k)回繰り返すのだから、Θ(k log log k)。
//	clanの代表値からなるヒープ探索は、サイズがΘ(k/log k)で出次数が定数(最大でosとprの2つ)であるので、普通のヒープを使えばO(k)。
//	最後に選ぶノードの数が問題となる。2分木であるので、どうやっても2*ceil(k/floor(log k))+1個のclanしか作られない。
//	代表以下の頂点は全てclanに属しているので、それらの数はΘ(k/log k)×Θ(log k)＝Θ(k)。
//	よって最後にk番目を得るのにΘ(k)。
//	計算量はΘ(k log log k)となる。
//
//Algorithm SEL2.
//SEL1ではclanでfloor(log k)個の値を得るのに普通のヒープ探索を使っていたが、
//これは実際にはこの問題自体である。よって再帰的にアルゴリズムを適用できる。
//各再帰でr番目を求めるとする。最初にr = kである。再帰の不変条件として、ヒープの個数は2r以下であるとできる。
//しかし、単純にやるとx個のヒープからΘ(x log r)個のclanを作ろうとして計算量が大きくなってしまうのでどうにかしなければならない。
//まず、ヒープたちを2*floor(log r)サイズのsubsetsに分割する。
//それぞれのsubset Hs_iで再帰的にfloor(log r)番目に小さい値までを求め、それをclan C_iとする。
//sp(C_i)は普通のclanと同じように決め、pr(C_i)はHs_iの根のうちC_iに含まれないノードたちとする。
//イメージとしては、仮想的なx個のノードがfloor(log r)個ずつclanをなしていて、そのoffspringとしてヒープたちが存在しているイメージ。
//あとはSEL1と同じようにする。
//
//計算量：
//	
//	構築しなければならないclanの数は代表が選ばれるまでの数とその子clanを足して最大でも3倍である。
//	T(r) ≦ c r + 3 ceil(r/floor(log r)) T(2 floor(log r))
//	という式がたつ。ここでcは定数である。
//	ベースケースは自明に定数である。
//	これを解くとT(r) ∈ O(r 3^{log^* r})となる。
//
//TODO: SEL3 と SEL4 を読み解く？
//
//■実装
//offspringを見る時に、clanのうちどれが葉であるのかを判定する必要がある。
//	これは代表と根たちを格納しておいて毎回同じようにdfsすれば判定可能である。
//poor relationをどうやって表わすか？
//	まず、その親のclanのオフセットを持つ。このclanのoffspringがいくつかのpoor relations setsのもととなる。
//	その頂点たちを使う時、clanの構成方法より、「offspring自体のうちある頂点以下だけ(の全て)を使う」という頂点が決定できる。
//	その頂点を保存しておき使うときに見てやればよい。
//nodesへはclanがextractされた時初めて追加したい。
//	その時にrepresentativeを用いてdfsをすればよい。
//	そのためには、親clanを持っておくだけでよい。
//これらによってメモリ(2k nodes + O(k/log k) (words + nodes))となるかな。
//ヒープの端の問題。
//	仮想的に大きな値を取るとすればよいが、それにはまた問題があって、
//	単純に∞を単一の特殊な値とするとuniquenessが成り立たなくなってしまう。
//	uniquenessは重要であるので、何とかしなければならない。
//	そこで、∞はフラグとして、その直前のノードの順序を保存するようにすればよい。
//	∞と比較が必要な箇所を適切に考えてやればよい。
//次数は2以外でもいける。でも定数部分がいろいろ変わるかと思う

struct ArrayHeap {
	typedef int T;
	typedef const T *HeapNode;
	const HeapNode a; const int N;
	ArrayHeap(HeapNode a_, int N_): a(a_), N(N_) { }

	bool is_null(HeapNode x) const {
		return x >= a + N;
	}
	bool compare(HeapNode x, HeapNode y) const {
		return *x != *y ? *x < *y : x < y;
	}
	static const int D = 2;
	HeapNode get_child(HeapNode x, int i) const {
		return a + ((x - a) * 2 + 1 + i);
	}
};

template<typename Heap>
class Frederickson {
	typedef typename Heap::HeapNode Node;
	static const int D = Heap::D;

	struct ClanP {	//priority queueに入れる用
		Node representative;
		bool infinity;
		int parentindex;	//親clanのindex
		ClanP() { }
		ClanP(const Node &representative_, bool infinity_, int parentindex_):
			representative(representative_), infinity(infinity_), parentindex(parentindex_) { }
		struct GT {
			const Heap &h;
			GT(const Heap &h_): h(h_) { }
			inline bool operator()(const ClanP &a, const ClanP &b) const {
				if(a.infinity != b.infinity) return a.infinity > b.infinity;
				return h.compare(b.representative, a.representative);	//PQ用に逆
			}
		};
	};
	struct Clan {
		Node representative;
		int offset, numsubheaps;	//numsubheapsはサブヒープの数(連結成分の数)。nodes[offset, offset+numsubheaps)が根たちになる。
		Node poorbound;	//!ispoorの場合これは未定義。poorbound以下のoffspringは既に使われていることを表している
						//順番にextractされるので、動的に変更される
		bool ispoor;	//1回以上使われたか。それ以降はpoor relationとして使われることになる。動的に変更される。
		int parentindex;	//親clanのindex
		Clan() { }
		Clan(Node representative_, int offset_, int numsubheaps_, int parentindex_):
			representative(representative_), offset(offset_), numsubheaps(numsubheaps_), poorbound(), ispoor(false), parentindex(parentindex_) { }
	};
	struct NodeLT {
		const Heap &h;
		NodeLT(const Heap &h_): h(h_) { }
		inline bool operator()(const Node &a, const Node &b) const {
			return h.compare(a, b);
		}
	};
	struct NodeGT {
		const Heap &h;
		NodeGT(const Heap &h_): h(h_) { }
		inline bool operator()(const Node &a, const Node &b) const {
			return h.compare(b, a);	//PQ用に逆
		}
	};

	Heap heap;

	Node build_clan(std::vector<Node> &q, bool &infinity, const int clansize) const;
	int extract_clan(const ClanP &clanp, int heapsubsets, std::vector<Clan> &clans, const std::vector<Node> &roots, std::vector<Node> &nodes, bool last, std::vector<Node> &q1, std::vector<Node> &q2, std::vector<Node> &stk1, std::vector<Node> &stk2) const;
public:
	Frederickson(const Heap &heap_): heap(heap_) { }
	//rootsの各要素はnullではない必要がある
	void enum_candidates(const std::vector<Node> &roots, int k, std::vector<Node> &nodes) const;
	void enum_candidates(Node root, int k, std::vector<Node> &nodes) const;
};

template<typename Heap>
typename Frederickson<Heap>::Node Frederickson<Heap>::build_clan(std::vector<Node> &q, bool &infinity, const int clansize) const {
	Node repr;
	if(clansize == 1) {
		infinity = false;
		repr = *std::min_element(q.begin(), q.end(), NodeLT(heap));
/*	}else {	//SEL2、遅い…？
		std::vector<Node> nodes;
		enum_candidates(q, clansize, nodes);
		if((int)nodes.size() < clansize) {
			infinity = true;
			repr = *std::max_element(nodes.begin(), nodes.end(), NodeLT(heap));
		}else {
			infinity = false;
			std::nth_element(nodes.begin(), nodes.begin() + (clansize-1), nodes.end(), NodeLT(heap));
			repr = nodes[clansize-1];
		}
*/
	}else {
		std::make_heap(q.begin(), q.end(), NodeGT(heap));
		repr = q.front(); int size = 0;
		while(!q.empty()) {
			Node x = q.front();
			std::pop_heap(q.begin(), q.end(), NodeGT(heap));
			q.pop_back();
			repr = x;
			if((++ size) == clansize) break;
			for(int i = 0; i < D; i ++) {
				Node y = heap.get_child(x, i);
				if(!heap.is_null(y)) {
					q.push_back(y);
					std::push_heap(q.begin(), q.end(), NodeGT(heap));
				}
			}
		}
		infinity = size < clansize;
	}

	q.clear();	//インターフェイス的に今clearしておくのがよさそう
	return repr;
}

template<typename Heap>
int Frederickson<Heap>::extract_clan(const ClanP &clanp, int heapsubsets, std::vector<Clan> &clans, const std::vector<Node> &roots, std::vector<Node> &nodes, bool last, std::vector<Node> &q1, std::vector<Node> &q2, std::vector<Node> &stk1, std::vector<Node> &stk2) const {
	Node repr = clanp.representative;
	bool infinity = clanp.infinity;
	int parentindex = clanp.parentindex;
	bool parentisvirtual = parentindex < heapsubsets;
	Clan &parent = clans[parentindex];
	int offset = nodes.size();
	if(parentisvirtual) {
		for(int i = 0; i < parent.numsubheaps; i ++) {
			Node y = roots[parent.offset + i];
			if(parent.ispoor && !heap.compare(parent.poorbound, y)) {
				//既に親・自分以外が使っている
			}else if(infinity || !heap.compare(repr, y)) {
				//親のoffspringであって、自分自身である。
				stk2.push_back(y);
				nodes.push_back(y);	//根は最初に並べるようにする
			}else {
				//自分のpoor relation set
				if(!last) q2.push_back(y);
			}
		}
	}else {
		for(int i = 0; i < parent.numsubheaps; i ++)
			stk1.push_back(nodes[parent.offset + i]);
		while(!stk1.empty()) {
			Node x = stk1.back(); stk1.pop_back();
			for(int i = 0; i < D; i ++) {
				Node y = heap.get_child(x, i);
				if(heap.is_null(y)) continue;
				if(!heap.compare(parent.representative, y)) {
					//親自身が使っている
					stk1.push_back(y);
				}else if(parent.ispoor && !heap.compare(parent.poorbound, y)) {
					//既に親・自分以外が使っている
				}else if(infinity || !heap.compare(repr, y)) {
					//親のoffspringであって、自分自身である。
					stk2.push_back(y);
					nodes.push_back(y);	//根は最初に並べるようにする
				}else {
					//自分のpoor relation set
					if(!last) q2.push_back(y);
				}
			}
		}
	}
	parent.ispoor = true;
	parent.poorbound = repr;
	//このclanを展開する(nodesに並べる)
	int numsubheaps = (int)nodes.size() - offset;
	while(!stk2.empty()) {
		Node x = stk2.back(); stk2.pop_back();
		for(int i = 0; i < D; i ++) {
			Node y = heap.get_child(x, i);
			if(heap.is_null(y)) continue;
			if(infinity || !heap.compare(repr, y)) {
				stk2.push_back(y);
				nodes.push_back(y);	//根以外を並べる
			}else {
				//自分のoffspring
				if(!last) q1.push_back(y);
			}
		}
	}
	return numsubheaps;
}

template<typename Heap>
void Frederickson<Heap>::enum_candidates(const std::vector<Node> &roots, int k, std::vector<Node> &nodes) const {
	if(k <= 0 || roots.empty()) return;
	int clansize = 1;
	while(1 << (clansize+1) <= k) clansize ++;
	int maxextracts = (k + clansize - 1) / clansize;
	std::vector<Clan> clans;	//clanたち。最初のいくつかは根ヒープの親としての仮想的なclanたち
	std::vector<Node> stk1, stk2;	//dfsで使うスタック
	std::vector<Node> q1, q2;	//priority_queueだとかゆい所(まとめてmake_heapしたい・clearしたい・順序関係なくイテレートしたい)に手がとどかないのでvectorでやる
	std::vector<ClanP> clansq;	//これもpriority queue
	//C_iの構築
	int heapsubsetsize = clansize * 2;	//定数部分はより適切な値があるかもしれない。つまり普通のos(C)のサイズまでということなので？
	int heaps = roots.size(), heapsubsets = (heaps + heapsubsetsize - 1) / heapsubsetsize;
	for(int i = 0; i < heapsubsets; i ++) {
		std::vector<Node> q;
		int first = i * heapsubsetsize;
		int size = std::min(heapsubsetsize, heaps - first);
		for(int j = first; j < first + size; j ++)
			q.push_back(roots[j]);
		bool inf;
		Node r = build_clan(q, inf, clansize);
		clansq.push_back(ClanP(r, inf, clans.size()));
		clans.push_back(Clan(r, first, size, -1));	//repr = rはダミー。他の項も別の意味である
	}
	std::make_heap(clansq.begin(), clansq.end(), typename ClanP::GT(heap));
	int extracts = 0;
	Node lastrepr = roots[0]; bool lastinfinity = false;
	while(!clansq.empty()) {
		ClanP clanp = clansq.front();
		std::pop_heap(clansq.begin(), clansq.end(), typename ClanP::GT(heap));
		clansq.pop_back();
		//まずこのclanを展開しなければならない。そのために親clanのoffspringから根たちを得なければならない
		int offset = nodes.size();
		int numsubheaps = extract_clan(clanp, heapsubsets, clans, roots, nodes, false, q1, q2, stk1, stk2);
		int clanindex = clans.size();
		clans.push_back(Clan(clanp.representative, offset, numsubheaps, clanp.parentindex));

		//最後に(k/log k)番目を展開したら終わり
		if((++ extracts) == maxextracts) {
			lastrepr = clanp.representative;
			lastinfinity = clanp.infinity;
			break;
		}

		//offspringから派生するclanを構築する。事前にq1にoffspringsを入れておく。
		if(!q1.empty()) {
			bool inf;
			Node r = build_clan(q1, inf, clansize);
			clansq.push_back(ClanP(r, inf, clanindex));
			std::push_heap(clansq.begin(), clansq.end(), typename ClanP::GT(heap));
		}
		//poor relation setから派生するclanを構築する。事前にq2にpoor relation setを入れておく。
		if(!q2.empty()) {
			bool inf;
			Node r = build_clan(q2, inf, clansize);
			clansq.push_back(ClanP(r, inf, clanp.parentindex));
			std::push_heap(clansq.begin(), clansq.end(), typename ClanP::GT(heap));
		}
	}
	//clansqの残りの要素を展開する
	for(int idx = 0; idx < (int)clansq.size(); idx ++) {
		ClanP clanp = clansq[idx];
		extract_clan(clanp, heapsubsets, clans, roots, nodes, true, q1, q2, stk1, stk2);
	}
}

template<typename Heap>
void Frederickson<Heap>::enum_candidates(Node root, int k, std::vector<Node> &nodes) const {
	if(heap.is_null(root)) return;
	std::vector<Node> roots(1, root);
	enum_candidates(roots, k, nodes);
}

#include "../Util/TimeIt_Simple.hpp"

const int N = 3000;
int a[N], sorted[N];
int main() {
	while(1) {
	for(int i = 0; i < N; i ++) a[i] = std::rand() % 100;
	std::make_heap(a, a + N, std::greater<int>());
//	for(int i = 0; i < N; i ++) cerr << a[i] << ", "; cerr << endl;
	for(int i = 0; i < N; i ++) sorted[i] = a[i];
	std::sort(sorted, sorted+N);
	ArrayHeap heap(a, N);
	Frederickson<ArrayHeap> frederickson(heap);
	TIMEIT("frederickson_selection k<-[0..N)")
	for(int k = 0; k < N; k ++) {
		std::vector<ArrayHeap::HeapNode> nodes;
		frederickson.enum_candidates(a, k + 1, nodes);
		std::nth_element(nodes.begin(), nodes.begin() + k, nodes.end(), [&heap](const int *x, const int *y) -> bool {
			return heap.compare(x, y);
		});
		ArrayHeap::HeapNode res = nodes[k];
		if(*res != sorted[k])
			cerr << k << ": " << *res << " != " << sorted[k] << endl;
	}
	cerr << "OK" << endl;
	}
	return 0;
}
