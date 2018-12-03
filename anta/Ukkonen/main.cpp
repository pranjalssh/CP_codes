//maximal repeat: <http://www.e-olimp.com/en/solutions/1317275>
/*
	Suffix Treeとは、文字列Sに対して、
		S$の全てのsuffix(init. tails$ s++"$")のパトリシア木(compressed trie)である。
	枝を文字列区間のポインタで表現するようにすると、Θ(n)のメモリで表現できる。

	Ukkonen'sを用いるとSuffix TreeをΘ(n) where n = |S| で構築できる。
		ただし、アルファベット数をパラメータに取る場合、
		実装によってΘ(n a), Θ(n log a), Θ(n log min(a,n)) where a = アルファベット数 等の計算量になる。
	Ukkonen'sはある意味で「オンライン」アルゴリズムである。
		先頭の文字から順番に構築していくことができる。
		ただし、それらの文字列は連続したアドレスになければならない。
		応用すると、Generalized Suffix Treeを、文字列を明示的に結合することなく構築することが出来る。
		この場合、各文字列は独立しているので、連続したアドレスにある必要はない。
		しかし、あまり意味は無い。

	Suffix Treeの欠点はメモリがそれなりに必要であること。
		ノードごとの枝をどのように管理するかによって、速度とメモリのトレードオフがある。
		・サイズaの配列
			小さいaでは最速だが、大きいaではメモリをΘ(a)で消費する。
			また、枝の列挙にも最悪Θ(a)の時間がかかる。
		・連結リスト
			小さいaでは高速で、メモリ消費もaに依存しないが、大きいaでは最悪Θ(a)で遅くなる。
			枝をソートすることも自然にできる。
		・ハッシュマップ
			平均定数の計算量ではあるが、その定数がそれなりに重い。また、メモリ消費の定数も重い。
			枝の列挙や最小の枝を得ることがo(a)では自然にできない。
		・平衡木
			logの計算量はやはりそれなりに重い。また、メモリ消費の定数も重い。
			枝の列挙や最小の枝を得ること、k番目の枝を得ることがよくできる。
		これらを適切に切り替えたり組み合わせたりすることもできる。
			そのような研究は不明。

	Suffix TreeをDFSする場合、最悪深さがnになることに注意(1つの文字の繰り返しの文字列によって達成できる)。
		再帰でやるとスタックオーバーフローすることも多い。

	Suffix Treeを利用すると文字列に関する様々な、主に何らかの「検索」が高速にできる。
	基本的な考えとして、Suffix Tree中には全ての部分文字列が含まれているということ。
		たとえば、文字列が出現するかどうかの判定は自明にΘ(m) where m = |検索文字列|でできる。

	・k-最長共通部分文字列(k-longest common substring)
		「このノードの子孫にkつの全ての終端文字が出現する」という性質を満たすノードのうち一番そこまでが長いものを求めればいい。
		終端文字の集合の管理にビット演算を使うと良い。

	・文字列の全て/最初/最後/次/前に出現する位置を求める
		Θ(n)で各葉に対して長さ(内部ノードの数ではなく、文字列の長さ)を求めておく。
		位置の列挙がしたい場合は、長さを一列に並べておけば、各ノードの子孫に区間が対応する(euler tour 葉だけver.)。
		実際の位置は、(文字列の長さ - 長さ)で求められる。
		長さと一緒にどの終端文字かを記録しておけば複数の文字列でも対応できる。
		あと、永続セグメント木やWaveletMatrixで管理すれば「k番目の出現位置」もできると思う(よりよい方法は不明)。

	・matching statistics
		matching statisticsとは、(map (commonPrefixLength s). init. tails$ s)。
		Ukkonen'sのSuffix Tree構築のような、suffix linkを使ったトラバースによって解ける。
		同じような解析によってΘ(m)でトラバースできることがわかる。
		suffix linkを使うたびに先頭の位置が右にずれると考えられる。それを記録しておき、
		各位置に対して statistics[先頭の位置 .. 現在の位置] をインクリメントする。
		それを累積和法でやればΘ(m)になる。

	・[longest ]k-repeated substring
		k-repeated substringとは、S中にk回以上出現する文字列。
		k以上の葉を子孫に持つノードが全てこれ。
		just kである全てを求めたくて、葉の数がkより大きい場合、k-組み合わせを全て求めればいい。

	・[shortest ]at most k-repeated substring
		名前は適当に作った。
		k-repeated substringと同じようにやればいい。
		今のところshortest k=1 (1度だけ出現)のみ実装(allShortestOccurringOnlyOnceSubstrings)。

	・supermaximal k-repeated substring (supermaximal 2-repeated = supermaximal repeat)
		supermaximal k-repeated substringとは、S中にk回以上出現し、
		その文字列を左右のどちらに広げてもS中にk回以上出現しなくなる文字列。
		右に極大は、子孫のノードでk-repeatedが無いノードのみを考えればいい。
		左に極大は、k-repeatedからのsuffix linkで到達できないノードのみを考えればいい。
		それを両方やれば極大(supermaximal)になる。

	・maximal k-repeeated substring (maximal 2-repeated = maximal repeat / maximal pair)
		*ちょうど* kつの *位置* に対して極大である、k回以上出現する部分文字列。
		右に極大は、kつの葉が、自分のノードから見て「全て同じ枝」ではないものを考えればいい。
		左に極大は、kつの葉が、「自分を表す文字列の左の文字が全て同じ」ではないものを考えればいい。
			これは葉も含めたノードごとに「singletonか？そうならどの文字か？」を再帰的に計算させればいい。
		"maximal pair"など位置で区別する場合は適当に組み合わせをする。
		"maximal repeat"など文字列だけを考える場合はノードごとに1つだけ考える。

	・最頻部分文字列 with 最小長さ制約
		ノードの子孫の葉の数を数えるだけ

	・部分文字列でない最短文字列(文字セットが与えられる)
		枝の長さが2以上のものに注意。
		|Σ|=1のときうまく同じようにやるやり方が思いつかなかったので別ルーチンにしてある。

	・部分文字列に対する最長共通接頭詞(Longest Common Extension, LCE, Longest Common Prefix, LCP)
		木のLCAとなる。適切なアルゴリズムを使えばクエリΘ(1)。
		事前に(位置(整数, GSTの場合は整数組) → ノードポインタもしくはLCA用ノード番号)のテーブルを作っておく。

	・文字列検索 with ワイルドカード(1文字だけにマッチする)
		まず対象文字列とパターンのGSTを作る。
		対象文字列のどの位置からマッチするかを全探索する。
		最初に両者0位置からのLCEを求める。するとどこまでマッチするかがわかる。
		その場所がワイルドカードであった場合、その文字を飛ばす(開始位置にLCE長さ+1を足す)。
		LCEがパターンの最後まで行ってたらマッチ。
		全探索n回に、ワイルドカードのたびにループするのでΘ(n k) (n=|対象文字列|, k=ワイルドカードの数)

	・k回までミスマッチを許す文字列検索 (k-mismatches)
		ワイルドカードのもののように全探索+LCEで進む。
		LCEの場所から最大k回まで1文字進めてあげる。
		これもΘ(n k)。

	・最長回文は他に効率的なアルゴリズム(Manacher's)があるので無視。
		だがGapやmismatchesのあるバージョンは実装したい…？
*/

#include <utility>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include <ctime>
#include <cmath>
#include "../Util/StaticGraph_Simple.cpp"
#include "../LowestCommonAncestor/EulerTourLCA.cpp"

#undef assert
void assert(bool b) {
	if(!b) {
		std::cerr << "Assertion error!" << std::endl;
		abort();
	}
}

typedef unsigned char Alpha; const int NAlpha = 256;
inline bool IsTerminatingAlpha(Alpha a) { return a < 32; }
void OutAlpha(std::ostream &o, Alpha a) {
	if(IsTerminatingAlpha(a))
		o << '$' << static_cast<unsigned>(a);
	else
		o << static_cast<char>(a);
}

const int ListToArrayParam = (int)pow(NAlpha*1., 0.4);
class GeneralizedSuffixTree {
public:
	typedef std::pair<const Alpha*, const Alpha*> StrPair;
private:
	struct Leaf {
		const Alpha *begin, *end;	//1つの文字列だけならendはLeafにはいらないけど、generalizedだと必要な気がする
		Leaf *next;
		Leaf() { }
		Leaf(const Alpha *b): begin(b), next(NULL), end(NULL) { }
		Alpha terminatingAlpha() const { return *(end-1); }
		int length() const { return end - begin; }
	};
	struct Node : Leaf {	//rootとinternal node
		union { Leaf *head; Leaf **edges; };
		Node *suffix;
		Node() { suffix = NULL; head = NULL; }
	};

	int nNodes, nLeafs;
	Node *nodes; Leaf *leafs, *poolend;
	Node *root;
	std::vector<Leaf**> edgeArrays;
	std::vector<StrPair> addedStrings;

	bool isLeaf(const Leaf *p) const { return leafs <= p; }
	bool isListStyle(const Node *p) const { return !p->head || ((Leaf*)nodes <= p->head && p->head < poolend); }

#define FOR_EACH_EDGES_(e, n, c)\
	for(Leaf *c *e##_ = isListStyle(n) ? &n->head : n->edges;\
	isListStyle(n) ? (*e##_ != NULL) : (e##_ < n->edges + NAlpha);\
	isListStyle(n) ? (e##_ = &(*e##_)->next) : (++ e##_))\
	if(c Leaf *e = *e##_)
#define FOR_EACH_EDGES_V(e, n) FOR_EACH_EDGES_(e, n, )
#define FOR_EACH_EDGES_C(e, n) FOR_EACH_EDGES_(e, n, const)

	Leaf **listToArray(Node *node) {
		Leaf **edges = (Leaf**)malloc(NAlpha * sizeof(Leaf*));
		memset(edges, 0, sizeof(Leaf*) * NAlpha);
		FOR_EACH_EDGES_V(leaf, node)
			edges[*leaf->begin] = leaf;
		edgeArrays.push_back(edges);
		return edges;
	}

	inline Leaf **getEdgeRef(Node *node, Alpha a) {
		if(!isListStyle(node)) return &node->edges[a];
		Leaf **ref; int len = 0;
		for(ref = &node->head; *ref != NULL; ref = &(*ref)->next, ++ len) {
			if(len >= ListToArrayParam)
				return &(node->edges = listToArray(node))[a];
			if(*(*ref)->begin == a)
				return ref;
		}
		return ref;
	}
	inline Leaf *getEdge(const Node *node, Alpha a) const {
		if(!isListStyle(node)) return node->edges[a];
		for(Leaf *leaf = node->head; leaf != NULL; leaf = leaf->next)
			if(*leaf->begin == a) return leaf;
		return NULL;
	}
	inline void addEdge(Node *node, Alpha a, Leaf *edge) {
		if(!isListStyle(node)) {
			node->edges[a] = edge;
		}else {
			Leaf *head = node->head;
			node->head = edge;
			edge->next = head;
		}
	}
	void outLeafName(std::ostream &o, const Leaf *leaf) const {
		o << '"' << static_cast<const void*>(leaf) << '"';
	}
	void outRec(std::ostream &o, const Node *node, bool visSuffix) const {
		if(visSuffix && node->suffix) {
			outLeafName(o, node); o << " -> "; outLeafName(o, node->suffix); o << ' ';
			o << "[style=dotted, constraint=false]" << ";\n";
		}
		FOR_EACH_EDGES_C(edge, node) {
			outLeafName(o, node); o << " -> "; outLeafName(o, edge); o << ' ';
			o << "[label=<";
			for(const Alpha *p = edge->begin; p < edge->end; ++ p) {
				OutAlpha(o, *p);
			}
			o << ">];\n";
			if(!isLeaf(edge))
				outRec(o, (Node*)edge, visSuffix);
		}
	}
public:
	GeneralizedSuffixTree(int n): nNodes(0), nLeafs(0),
		nodes((Node*)malloc(sizeof(Node) * n + sizeof(Leaf) * n)), leafs((Leaf*)(nodes + n)),
		poolend(leafs + n), root(new(&nodes[nNodes ++]) Node()) {
		root->begin = root->end = NULL;
		initTraversalData();
	}
	~GeneralizedSuffixTree() {
		free(nodes);
		for(int i = 0; i < (int)edgeArrays.size(); i ++)
			free(edgeArrays[i]);
		deleteTraversalData();
	}

	//strはIsTerminatingAlpha(a)がtrueとなる文字で終端されている必要がある
	//複数の文字列を追加する場合は、それぞれの終端文字が一意でなければならない
	void addString(const Alpha *str) {
		Node *active = root; const Alpha *activePos; int activeLen = 0;
		const Alpha *currentPos = str, *proceeded = str;
		int old_nLeafs = nLeafs;
		do {
			if(currentPos-str && (currentPos-str) % 100000 == 0) std::cout << (proceeded-str) << ", " << (currentPos-str) << ": edgeArrays = " << edgeArrays.size() << std::endl;
			Alpha currentAlpha = *currentPos; Node *prevNode = NULL;
			while(proceeded <= currentPos) {
				if(activeLen == 0) activePos = currentPos;
				Leaf **activeEdgeRef = getEdgeRef(active, *activePos);
				Leaf *activeEdge = *activeEdgeRef;
				Node *splitNode = active;
				if(activeEdge) {
					if(!isLeaf(activeEdge) && activeLen >= activeEdge->length()) {
						active = (Node*)activeEdge;
						activePos += ((Node*)activeEdge)->length();
						activeLen -= ((Node*)activeEdge)->length();
						continue;
					}else if(*(activeEdge->begin + activeLen) == currentAlpha) {
						activeLen ++;
						if(prevNode) prevNode->suffix = active;
						break;
					}else {
						const Alpha *splitPos = activeEdge->begin + activeLen;
						Leaf *nextLeaf = activeEdge->next;
						*activeEdgeRef = splitNode = new(&nodes[nNodes ++]) Node();
						splitNode->begin = activeEdge->begin;
						splitNode->end = activeEdge->begin = splitPos;
						addEdge(splitNode, *splitPos, activeEdge);
						splitNode->next = nextLeaf;
					}
				}
				addEdge(splitNode, currentAlpha, new(&leafs[nLeafs ++]) Leaf(currentPos));
				if(prevNode) prevNode->suffix = splitNode;
				prevNode = splitNode;
				proceeded ++;
				if(active == root && activeLen > 0) {
					activeLen --;
					activePos = proceeded;
				}else
					active = active->suffix ? active->suffix : root;
			}
		}while(!IsTerminatingAlpha(*(currentPos ++)));
		assert(currentPos == proceeded);
		addedStrings.push_back(std::make_pair(str, currentPos));
		for(int i = old_nLeafs; i < nLeafs; i ++)
			leafs[i].end = currentPos;
	}

	void debugOutDotGraph(std::ostream &o, bool visSuffix = true) const {
		o << "digraph {\n";
		o << "style = dot;\n";
		o << "node [shape=point];\n";
		outRec(o, root, visSuffix);
		o << "}" << std::endl;
	}

private:
	struct Position {
		const Leaf *leaf;
		const Alpha *pos;
		Position(const Leaf *l, const Alpha *p): leaf(l), pos(p) { }
	};
	inline Position initialPosition() const {
		return Position(root, NULL);
	}
	//Leafの最後に行ったあとにマッチする場合は未定義。
	//終端文字が出現しないか最後に1回だけ出現なら問題ない
	inline void matchAlpha(Position &p, Alpha a) const {
		if(!p.leaf) return;
		if(p.leaf->end == p.pos) {
			const Leaf *edge = getEdge(static_cast<const Node*>(p.leaf), a);
			p.leaf = edge;
			if(edge) p.pos = edge->begin + 1;
		}else {
			if(*p.pos == a) ++ p.pos;
			else p.leaf = NULL;
		}
	}
	//マッチしながら木を降りていく
	inline void matchString(Position &p, const Alpha *begin, const Alpha *end) const {
		while(begin < end) matchAlpha(p, *(begin ++));
	}

public:
	bool isSuffix(const Alpha *begin, const Alpha *end, Alpha terminatingAlpha) const {
		Position p = initialPosition();
		matchString(p, begin, end);
		matchAlpha(p, terminatingAlpha);
		return p.leaf != NULL;
	}
	bool isSubString(const Alpha *begin, const Alpha *end) const {
		Position p = initialPosition();
		matchString(p, begin, end);
		return p.leaf != NULL;
	}
	static const Alpha *getStringLast(const Alpha *str) {
		while(!IsTerminatingAlpha(*str)) ++ str;
		return str;
	}
	//Tested
	bool isSuffix(const Alpha *str) const {
		const Alpha *end = getStringLast(str);
		return isSuffix(str, end, *end);
	}

	//Verified: POJ 2217 (788K / 79MS)
	//繰り返しの多い長い文字列でやる場合にはスタックオーバーフロー・MLEに注意
	int longestCommonSubstringLength(unsigned kmask) const {
		return longestCommonSubstring_rec(root, kmask).second;
	}

private:
	//返り値: (ビットセット, if ビットセットがフル then 長さ else 0)
	//文字列を求めたいなら適当に経路復元すればよい。
	std::pair<unsigned, int> longestCommonSubstring_rec(const Leaf *leaf, unsigned kmask) const {
		if(isLeaf(leaf)) {	//終端ケース
			return std::make_pair(1U << leaf->terminatingAlpha(), 0);
		}
		const Node *node = static_cast<const Node*>(leaf);
		unsigned bits = 0;
		std::pair<int, const Leaf*> p = std::make_pair(-1, (const Leaf*)NULL);
		FOR_EACH_EDGES_C(edge, node) {
			std::pair<unsigned, int> q;
			q = longestCommonSubstring_rec(edge, kmask);
			bits |= q.first;
			if((q.first & kmask) == kmask)
				p = std::max(p, std::make_pair(q.second + edge->length(), edge));
		}
		if(!p.second) return std::make_pair(bits, 0);
		return std::make_pair(bits, p.first);
	}

public:
	void addByteString(const char *s) { addString((const Alpha*)s); }

private:
	//suffix linkを使う用。常にLeafでなくNodeを指すことがポイント
	struct PositionS {
		const Node *node;
		Alpha alpha;
		const Leaf *cache_edge;
		int length;
	};

	PositionS initialPositionS() const {
		PositionS pos;
		pos.node = root;
		pos.length = 0;
		pos.cache_edge = NULL;
		return pos;
	}

	//戻り値: suffix linkを使った回数(ただしrootから移動しようとしたのも含まれる)
	//Ukkonen'sのメイン処理と似ている。
	//(pos.nodeまでの長さ + pos.length)で抑えられて、ならしΘ(1)のはず
	int matchAlphaS(PositionS &pos, Alpha a) const {
		int count = 0;
		while(1) {
			if(pos.length == 0)
				pos.cache_edge = getEdge(pos.node, pos.alpha = a);
			if(pos.cache_edge && *(pos.cache_edge->begin + pos.length) == a)
				break;
			count ++;
			const Alpha *p = pos.cache_edge ? pos.cache_edge->begin : NULL;
			if(pos.node == root) {
				if(pos.length == 0) break;
				if(-- pos.length)
					pos.cache_edge = getEdge(pos.node, pos.alpha = *(++ p));
			}else {
				pos.node = pos.node->suffix;
				if(pos.length) pos.cache_edge = getEdge(pos.node, pos.alpha);
			}
			if(pos.cache_edge)
				while(pos.length >= pos.cache_edge->length()) {
					pos.length -= pos.cache_edge->length();
					p += pos.cache_edge->length();
					pos.node = static_cast<const Node*>(pos.cache_edge);
					if(pos.length)
						pos.cache_edge = getEdge(pos.node, pos.alpha = *p);
					else {
						pos.cache_edge = NULL;
						break;
					}
				}
		}
		if(pos.cache_edge) {
			if(pos.cache_edge->length() == ++ pos.length) {
				pos.node = static_cast<const Node*>(pos.cache_edge);
				pos.length = 0;
				pos.cache_edge = NULL;
			}
		}
		return count;
	}

private:

	struct LeafData {
		Leaf *leaf;
		int substringLength;
		const Alpha *getBegin() const { return leaf->end - substringLength; }
	};

	int *nodeLeftIndex, *nodeRightIndex, *leafIndex;
	LeafData *leafSequence;
	const LeafData **leftestSubstringLeaf;
	int *nodeSubstringLength;

	void initTraversalData() {
		nodeLeftIndex = nodeRightIndex = leafIndex = NULL;
		leafSequence = NULL;
		leftestSubstringLeaf = NULL;
		nodeSubstringLength = NULL;
	}
	void deleteTraversalData() {
		delete[] nodeLeftIndex;
		delete[] nodeRightIndex;
		delete[] leafIndex;
		delete[] leafSequence;
		delete[] leftestSubstringLeaf;
		delete[] nodeSubstringLength;
		initTraversalData();
	}

public:
	//スタックオーバーフローに注意
	//メモリ・速度が厳しい場合は必要なデータだけにすること
	void leafTraversal() {
		deleteTraversalData();
		leafIndex = new int[nLeafs];
		nodeLeftIndex = new int[nNodes];
		nodeRightIndex = new int[nNodes];
		leafSequence = new LeafData[nLeafs];
		nodeSubstringLength = new int[nNodes];
		int tmp_i = 0;
		leafTraversal_rec(root, tmp_i, 0);
		assert(tmp_i == nLeafs);
	}

private:
	void leafTraversal_rec(Leaf *leaf, int &inout_i, int len) {
		if(isLeaf(leaf)) {
			leafIndex[leaf - leafs] = inout_i;
			LeafData &data = leafSequence[inout_i ++];
			data.leaf = leaf;
			data.substringLength = len;
			return;
		}
		Node *node = (Node*)leaf;
		nodeSubstringLength[node - nodes] = len;
		nodeLeftIndex[node - nodes] = inout_i;
		FOR_EACH_EDGES_V(edge, node)
			leafTraversal_rec(edge, inout_i, len + edge->length());
		nodeRightIndex[node - nodes] = inout_i;
	}


	void getLeafInterval(const Leaf *leaf, int &out_left, int &out_right) const {
		if(isLeaf(leaf))
			out_right = (out_left = leafIndex[leaf - leafs]) + 1;
		else {
			const Node *node = (const Node*)leaf;
			out_left = nodeLeftIndex[node - nodes];
			out_right = nodeRightIndex[node - nodes];
		}
	}
public:
	//v中の順序は決まっていないことに注意
	void findAll(const Alpha *begin, const Alpha *end, std::vector<const Alpha *> &out_v) const {
		assert(nodeLeftIndex && nodeRightIndex && leafSequence);
		Position p = initialPosition();
		matchString(p, begin, end);
		if(p.leaf == NULL) return;
		int left, right;
		getLeafInterval(p.leaf, left, right);
		for(int i = left; i < right; i ++)
			out_v.push_back(leafSequence[i].getBegin());
	}

	int countSubstring(const Alpha *begin, const Alpha *end) {
		assert(nodeLeftIndex && nodeRightIndex);
		Position p = initialPosition();
		matchString(p, begin, end);
		if(p.leaf == NULL) return 0;
		int left, right;
		getLeafInterval(p.leaf, left, right);
		return right - left;
	}

	int longestKRepeatedSubstringLength(int k) const {
		assert(nodeLeftIndex && nodeRightIndex && nodeSubstringLength);
		int maxLen = 0;
		if(k <= 1)	//コーナーケース。k=1だと葉を見る必要がある。
					//さらに、長さに終端文字も含むことに注意(問題があれば削ること)
			for(int i = 0; i < nLeafs; i ++)
				maxLen = std::max(maxLen, leafSequence[i].substringLength);
		else for(int i = 0; i < nNodes; i ++)
			if(k <= nodeRightIndex[i] - nodeLeftIndex[i])
				maxLen = std::max(maxLen, nodeSubstringLength[i]);
		return maxLen;
	}

	//out_setsの各要素のサイズはk *以上* 。順序は決まっていないことに注意
	int allLongestKRepeatedSubstrings(int k, std::vector<std::vector<const Alpha*> > &out_sets) const {
		assert(nodeLeftIndex && nodeRightIndex && nodeSubstringLength && leafSequence);
		int maxLen = longestKRepeatedSubstringLength(k);
		if(k <= 1) {	//終端文字も含むことに注意
			for(int i = 0; i < nLeafs; i ++)
				if(leafSequence[i].substringLength == maxLen) {
					out_sets.push_back(std::vector<const Alpha*>());
					out_sets.back().push_back(leafSequence[i].getBegin());
				}	//余談：ここのifのせいで↑のif(k <= 1)に開き括弧をつける必要がある
		}else for(int i = 0; i < nNodes; i ++)
			if(k <= nodeRightIndex[i] - nodeLeftIndex[i] &&
				nodeSubstringLength[i] == maxLen) {
				out_sets.push_back(std::vector<const Alpha*>());
				for(int j = nodeLeftIndex[i]; j < nodeRightIndex[i]; j ++)
					out_sets.back().push_back(leafSequence[j].getBegin());
			}
		return maxLen;
	}

	typedef std::pair<int, std::vector<const Alpha*> > MaximalTuple;	//Tupleといっておきながら順序無しなのです
	//k=2の場合に"supermaximal repeat"と呼ばれる
	//out_setsは{(長さ, [先頭])}。out_setsの各要素のサイズはk *以上* 。順序は決まっていないことに注意
	void allSupermaximalKRepeatedSubstrings(int k, std::vector<MaximalTuple> &out_sets) const {
		assert(nodeLeftIndex && nodeRightIndex && nodeSubstringLength && leafSequence);
		if(k <= 1) {	//コーナーケース。全ての文字列を単に返せばいい
			for(int i = 0; i < (int)addedStrings.size(); i ++) {
				const std::pair<const Alpha*, const Alpha*> &p = addedStrings[i];
				out_sets.push_back(std::make_pair(p.second - p.first, std::vector<const Alpha*>(1, p.first)));
			}
			return;
		}
		char *tmp = new char[nNodes];
		memset(tmp, 0, nNodes);
		allSupermaximalKRepeatedSubstrings_rec(root, k, tmp);
		for(int i = 0; i < nNodes; i ++)
			if(tmp[i] == 1) {
				out_sets.push_back(std::make_pair(nodeSubstringLength[i], std::vector<const Alpha*>()));
				for(int j = nodeLeftIndex[i]; j < nodeRightIndex[i]; j ++)
					out_sets.back().second.push_back(leafSequence[j].getBegin());
			}
		delete[] tmp;
	}
private:
	bool allSupermaximalKRepeatedSubstrings_rec(const Leaf *leaf, const int k, char out_tmp[]) const {
		if(isLeaf(leaf))	//k=1はここでは無視
			return false;
		const Node *node = (const Node*)leaf;
		bool b = false;
		FOR_EACH_EDGES_C(edge, node)
			b |= allSupermaximalKRepeatedSubstrings_rec(edge, k, out_tmp);
		int i = node - nodes;
		if(!b && nodeRightIndex[i] - nodeLeftIndex[i] >= k) {
			b = true;
			out_tmp[i] |= 1;
			if(node != root) out_tmp[node->suffix - nodes] |= 2;
		}
		return b;
	}
public:
	typedef std::vector<std::pair<std::pair<Alpha, Alpha>, const Alpha*> > MaximalSet;
	typedef std::vector<std::pair<int, MaximalSet> > MaximalSets;
	//minimumLength以上のものだけを報告する。デフォルトは1(0だと空文字も報告される)。
	//k=2の場合に"maximal pair"と呼ばれる。
	//out_setsは{(長さ, [(前の文字,先頭)])}。
	//out_setsのうち、「全てfirst.firstが同じ」でない かつ「全てfirst.secondが同じ」でない k-組み合わせがすべてk-repeated tupleになる。
	void allMaximalKRepeatedSubstrings(int k, MaximalSets &out_sets, int minimumLength = 1) const {
		assert(nodeLeftIndex && nodeRightIndex && nodeSubstringLength && leafSequence);
		if(k <= 1) {
			for(int i = 0; i < (int)addedStrings.size(); i ++) {
				const std::pair<const Alpha*, const Alpha*> &p = addedStrings[i];
				if(p.second - p.first >= minimumLength) {
					out_sets.push_back(std::make_pair(p.second - p.first, MaximalSet()));
					out_sets.back().second.push_back(std::make_pair(
						std::make_pair(*(p.second-1), *(p.second-1)), p.first));
				}
			}
			return;
		}
		allMaximalKRepeatedSubstrings_rec(root, k, minimumLength, out_sets);
	}

	//out_combsの各要素.secondのサイズは *ちょうど* k。
	//allMaximalKRepeatedSubstringsの結果を組み合わせるだけ。数は指数関数的に増加するので注意。
	void allMaximalKRepeatedTupleCombinations(int k, std::vector<MaximalTuple> &out_combs, int minimumLength = 1) {
		MaximalSets sets;
		allMaximalKRepeatedSubstrings(k, sets, minimumLength);
		if(k <= 1) {
			for(int i = 0; i < (int)sets.size(); i ++) {
				out_combs.push_back(std::make_pair(sets[i].first, std::vector<const Alpha*>()));
				out_combs.back().second.push_back(sets[i].second[0].second);
			}
		}else for(int i = 0; i < (int)sets.size(); i ++) {
			const MaximalSet &set = sets[i].second;
			std::vector<const Alpha*> tmp;
			allMaximalKRepeatedTupleCombinations_rec(0, k, 0, Alpha(), Alpha(), tmp, sets[i].first, set, out_combs);
		}
	}

	//k=2の場合に"maximal repeat"と呼ばれる。
	//空文字も返すので、要らないならはじくこと
	void allMaximalKRepeatedSubstrings_stringOnly(int k, std::vector<StrPair> &out_strs) const {
		assert(nodeLeftIndex && nodeRightIndex && nodeSubstringLength && leafSequence);
		if(k <= 1) {
			for(int i = 0; i < (int)addedStrings.size(); i ++)
				out_strs.push_back(addedStrings[i]);
			return;
		}
		allMaximalKRepeatedSubstrings_stringOnly_rec(root, k, out_strs);
	}

private:
	const Alpha *getLeafPrevAlpha(const Leaf *leaf) const {
		const LeafData &data = leafSequence[leafIndex[leaf - leafs]];
		//とりあえず。文字列の数はアルファベットサイズで抑えられて最悪Θ(n a)なので、一応aが定数なら線形
		for(int j = 0; j < (int)addedStrings.size(); j ++) {
			const std::pair<const Alpha*, const Alpha*> &p = addedStrings[j];
			if(p.second == data.leaf->end && p.second - p.first == data.substringLength)
				return p.second-1;
		}
		return data.getBegin()-1;
	}

	const Alpha *allMaximalKRepeatedSubstrings_rec(const Leaf *leaf, const int k, const int minimumLength, MaximalSets &out_sets) const {
		if(isLeaf(leaf)) return getLeafPrevAlpha(leaf);
		const Node *node = (const Node*)leaf;
		const Alpha *a; bool first = true;
		FOR_EACH_EDGES_C(edge, node) {
			const Alpha *b = allMaximalKRepeatedSubstrings_rec(edge, k, minimumLength, out_sets);
			if(first) a = b, first = false;
			else if(a == NULL || b == NULL || *a != *b) a = NULL;
		}
		const int i = node - nodes;
		if(a == NULL && nodeRightIndex[i] - nodeLeftIndex[i] >= k &&
			nodeSubstringLength[i] >= minimumLength) {
			out_sets.push_back(std::make_pair(nodeSubstringLength[i], MaximalSet()));
			FOR_EACH_EDGES_C(edge, node) {
				int left, right;
				getLeafInterval(edge, left, right);
				for(int j = left; j < right; j ++) {
					const LeafData &data = leafSequence[j];
					std::pair<Alpha, Alpha> p(
						*allMaximalKRepeatedSubstrings_rec(data.leaf, k, minimumLength, out_sets),
						*edge->begin);
					out_sets.back().second.push_back(std::make_pair(p, data.getBegin()));
				}
			}
		}
		return a;
	}

	const Alpha *allMaximalKRepeatedSubstrings_stringOnly_rec(const Leaf *leaf, const int k, std::vector<StrPair> &out_strs) const {
		if(isLeaf(leaf)) return getLeafPrevAlpha(leaf);
		const Node *node = (const Node*)leaf;
		const Alpha *a; bool first = true;
		FOR_EACH_EDGES_C(edge, node) {
			const Alpha *b = allMaximalKRepeatedSubstrings_stringOnly_rec(edge, k, out_strs);
			if(first) a = b, first = false;
			else if(a == NULL || b == NULL || *a != *b) a = NULL;
		}
		const int i = node - nodes;
		if(a == NULL && nodeRightIndex[i] - nodeLeftIndex[i] >= k) {
			const Alpha *s = leafSequence[nodeLeftIndex[i]].getBegin();
			out_strs.push_back(std::make_pair(s, s + nodeSubstringLength[i]));
		}
		return a;
	}

	//ナイーブに組み合わせを生成する
	void allMaximalKRepeatedTupleCombinations_rec(int i, int remK, unsigned bitflags, Alpha a, Alpha b, std::vector<const Alpha*> &inout_seq, const int strLen, const MaximalSet &set, std::vector<MaximalTuple> &out_combs) {
		if(i == set.size()) {
			if(remK == 0 && bitflags == 7)
				out_combs.push_back(std::make_pair(strLen, inout_seq));
			return;
		}
		//使わない場合
		if(remK < (int)set.size() - i)
			allMaximalKRepeatedTupleCombinations_rec(i+1, remK, bitflags, a, b, inout_seq, strLen, set, out_combs);
		//使う場合
		if(remK > 0) {
			Alpha aa = set[i].first.first, bb = set[i].first.second;
			if(bitflags == 0) {
				bitflags = 1;
				a = aa; b = bb;
			}
			if(a != aa) bitflags |= 2;
			if(b != bb) bitflags |= 4;
			inout_seq.push_back(set[i].second);
			allMaximalKRepeatedTupleCombinations_rec(i+1, remK-1, bitflags, a, b, inout_seq, strLen, set, out_combs);
			inout_seq.pop_back();
		}
	}

public:
	int mostFrequentlyOccurrings(int minimumLength) const {
		assert(nodeLeftIndex && nodeRightIndex && nodeSubstringLength && leafSequence);
		int maxCount = 0;
		for(int i = 0; i < nNodes; i ++)
			if(nodeSubstringLength[i] >= minimumLength)
				maxCount = std::max(maxCount, nodeRightIndex[i] - nodeLeftIndex[i]);
		for(int i = 0; i < nLeafs; i ++)
			if(leafSequence[i].substringLength >= minimumLength)
				maxCount = std::max(maxCount, 1);
		return maxCount;
	}

	int allMostFrequentlyOccurringSubstring(int minimumLength, std::vector<StrPair> &out_v) const {
		int maxCount = mostFrequentlyOccurrings(minimumLength);
		for(int i = 0; i < nNodes; i ++)
			if(nodeRightIndex[i] - nodeLeftIndex[i] == maxCount) {
				int maxLen = nodeSubstringLength[i];
				const Alpha *p = leafSequence[nodeLeftIndex[i]].getBegin();
				//枝の間の中途半端な場所も必要なことに注意。実用的にはmaximalにするのかな？
				for(int len = maxLen; len >= minimumLength && maxLen - len < nodes[i].length(); len --)
					out_v.push_back(std::make_pair(p, p + len));
			}
		for(int i = 0; i < nLeafs; i ++)
			if(maxCount == 1) {
				const LeafData &data = leafSequence[i];
				int maxLen = data.substringLength;
				const Alpha *p = data.getBegin();
				for(int len = maxLen; len >= minimumLength && maxLen - len < data.leaf->length(); len --)
					out_v.push_back(std::make_pair(p, p + len));
			}
		return maxCount;
	}

	//終端文字は含まない。"aaa$"の"a$"は駄目(もし許すと自明に長さ2以下になってしまう)
	//空文字には0を返す(未定義である)。
	int shortestOccurringOnlyOnceSubstringLength() const {
		assert(leafSequence != NULL);
		int minLen = INT_MAX;
		for(int i = 0; i < nLeafs; i ++) {
			const Leaf *leaf = leafSequence[i].leaf;
			if(leaf->length() > 1)
				minLen = std::min(minLen, leafSequence[i].substringLength - (leaf->length() - 1));
		}
		return minLen != INT_MAX ? minLen : 0;
	}
	int allShortestOccurringOnlyOnceSubstrings(std::vector<StrPair> &out_v) const {
		int minLen = shortestOccurringOnlyOnceSubstringLength();
		if(minLen == 0) return 0;
		for(int i = 0; i < nLeafs; i ++) {
			const LeafData &data = leafSequence[i];
			const Leaf *leaf = data.leaf;
			if(leaf->length() > 1 && data.substringLength - (leaf->length() - 1) == minLen)
				out_v.push_back(std::make_pair(data.getBegin(), data.getBegin() + minLen));
		}
		return minLen;
	}
private:

public:

	//euler tourをしたあとに呼ぶ。findFirstに必要
	void makeFindFirstData() {
		assert(leafSequence != NULL);
		delete[] leftestSubstringLeaf;
		leftestSubstringLeaf = new const LeafData*[nNodes];
		makeFindFirstData_rec(root);
	}
	const Alpha *findFirst(const Alpha *begin, const Alpha *end) {
		assert(leafIndex != NULL && leafSequence != NULL && leftestSubstringLeaf != NULL);
		Position p = initialPosition();
		matchString(p, begin, end);
		if(p.leaf == NULL) return NULL;
		const LeafData *data;
		if(isLeaf(p.leaf))
			data = &leafSequence[leafIndex[p.leaf - leafs]];
		else
			data = leftestSubstringLeaf[(const Node*)p.leaf - nodes];
		return data->getBegin();
	}

private:

	const LeafData *makeFindFirstData_rec(const Leaf *leaf) {
		if(isLeaf(leaf))
			return &leafSequence[leafIndex[leaf - leafs]];
		const Node *node = (const Node*)leaf;
		const LeafData *leftest = NULL;
		FOR_EACH_EDGES_C(edge, node) {
			const LeafData *data = makeFindFirstData_rec(edge);
			if(!leftest || leftest->substringLength < data->substringLength)
				leftest = data;
		}
		leftestSubstringLeaf[node - nodes] = leftest;
		return leftest;
	}

public:
	//Tested
	void matchingStatistics(const Alpha *begin, const Alpha *end, int out_lengths[/*end - begin*/]) const {
		int n = end - begin;
		std::fill(out_lengths, out_lengths + n, 0);
		int matchHead = 0;
		PositionS pos = initialPositionS();
		for(int i = 0; i < n; ++ i) {
			matchHead += matchAlphaS(pos, *(begin + i));
			//out_lengths[matchHead..i] ++;
			out_lengths[i] ++;
			if(matchHead) out_lengths[matchHead-1] --;
		}
		//累積和を取る
		for(int i = n-2; i >= 0; i --) out_lengths[i] += out_lengths[i+1];
	}

	//Tested
	int commonPrefixLength(const Alpha *begin, const Alpha *end) const {
		Position pos = initialPosition();
		for(const Alpha *p = begin; p < end; ++ p) {
			matchAlpha(pos, *p);
			if(pos.leaf == NULL) return p - begin;
		}
		return end - begin;
	}

public:
	//alphasはソートされている必要がある
	int notOccurringShortestStringLength(const std::vector<Alpha> &alphas) {
		if(alphas.size() == 1) {	//コーナーケース
			Position p = initialPosition();
			int len = 0;
			while(p.leaf)
				matchAlpha(p, alphas[0]), len ++;
			return len;
		}else
			return notOccurringShortestString_rec(root, NULL, alphas);
	}
	//今の実装では辞書順最小のものを返す
	int notOccurringShortestString(const std::vector<Alpha> &alphas, std::vector<Alpha> &out_str) {
		if(alphas.size() == 1) {
			int len = notOccurringShortestStringLength(alphas);
			for(int i = 0; i < len; i ++)
				out_str.push_back(alphas[0]);
			return len;
		}else
			return notOccurringShortestString_rec(root, &out_str, alphas);
	}

private:
	//|alphas|=1はここでは無視
	int notOccurringShortestString_rec(const Leaf *leaf, std::vector<Alpha> *out_str, const std::vector<Alpha> &alphas) {
		if(isLeaf(leaf)) {
			if(out_str) out_str->push_back(alphas[0]);
			return 1;
		}
		const Node *node = (const Node*)leaf;
		std::vector<std::pair<Alpha, const Leaf*> > nexts;
		FOR_EACH_EDGES_C(edge, node)
			nexts.push_back(std::make_pair(*edge->begin, edge));
		std::sort(nexts.begin(), nexts.end());
		std::vector<Alpha>::const_iterator i = alphas.begin();
		std::vector<std::pair<Alpha, const Leaf*> >::const_iterator j = nexts.begin();

		std::pair<std::pair<int, Alpha>, const Leaf*> mini =
			std::make_pair(std::make_pair(INT_MAX, Alpha()), (const Leaf*)NULL);
		while(i != alphas.end() || j != nexts.end()) {
			if(i == alphas.end() || (j != nexts.end() && j->first < *i))
				++ j;
			else if(j == nexts.end() || j->first > *i) {
				mini = std::min(mini, std::make_pair(std::make_pair(1, *i), (const Leaf*)NULL));
				++ i;
			}else {
				const Leaf *edge = j->second;
				if(edge->length() > 1)
					mini = std::min(mini, std::make_pair(std::make_pair(2, *i), edge));
				else
					mini = std::min(mini, std::make_pair(std::make_pair(
						1 + notOccurringShortestString_rec(edge, NULL, alphas), *i),
						edge));
				++ i, ++ j;
			}
		}
		if(out_str) {
			out_str->push_back(mini.first.second);
			const Leaf *edge = mini.second;
			if(edge && edge->length() > 1) {
				for(int i = 0; i < (int)alphas.size(); i ++)
					if(alphas[i] != *(edge->begin+1)) {
						out_str->push_back(alphas[i]);
						break;
					}
			}else if(edge)
				notOccurringShortestString_rec(edge, out_str, alphas);
		}
		return mini.first.first;
	}

	inline Graph::Vertex getLeafNumber(const Leaf *leaf) const {
		if(isLeaf(leaf)) return leaf - leafs;
		else return nLeafs + ((const Node*)leaf - nodes);
	}
	inline const Leaf *numberToLeaf(Graph::Vertex v) const {
		if(v < nLeafs) return &leafs[v];
		else return &nodes[v - nLeafs];
	}

	std::vector<std::vector<Leaf*> > positionLeafs;
	EulerTourLCA lca;
public:
	void buildLCEData() {
		assert(leafSequence && leafIndex);
		{
			std::vector<Graph::Edge> edges;
			edges.reserve(nLeafs + nNodes - 1);
			for(int i = 0; i < nNodes; i ++) {
				const Node *node = &nodes[i];
				int num = getLeafNumber(node);
				FOR_EACH_EDGES_C(edge, node) {
					edges.push_back(Graph::Edge(num, getLeafNumber(edge)));
				}
			}

			Graph g(nLeafs + nNodes, edges); edges.shrink_to_fit();
			lca.build(g, getLeafNumber(root));
		}

		positionLeafs.assign(addedStrings.size(), std::vector<Leaf*>());
		int index = 0;
		for(int i = 0; i < (int)addedStrings.size();i ++) {
			int len = addedStrings[i].second - addedStrings[i].first;
			positionLeafs[i].resize(len);
			for(int j = index; j < index + len; j ++) {
				int x = len - leafSequence[leafIndex[j]].substringLength;
				positionLeafs[i][x] = &leafs[j];
			}
			index += len;
		}
	}

	//Longest Common Prefix とも呼ばれる。これは複数文字列にgeneralizeされている。
	//xi,yiは文字列のインデックスを、xj,yjはオフセットを指定する
	int longestCommonExtension(int xi, int xj, int yi, int yj) const {
		return substringLength(nodeLCA(positionLeafs[xi][xj], positionLeafs[yi][yj]));
	}
	int longestCommonExtension(int x, int y) const {
		return longestCommonExtension(0, x, 0, y);
	}
private:
	const Leaf *nodeLCA(const Leaf *x, const Leaf *y) const {
		return numberToLeaf(lca.query(getLeafNumber(x), getLeafNumber(y)));
	}

	int substringLength(const Leaf *leaf) const {
		if(isLeaf(leaf)) return leafSequence[leafIndex[leaf - leafs]].substringLength;
		else return nodeSubstringLength[(const Node*)leaf - nodes];
	}

public:
	//事前に2つの文字列で構築しておく必要がある
	//対象文字列, パターン の順番に
	//Θ(k (n + m)) where k = ワイルドカードの数
	void findAllWithWildCards(Alpha wildCard, std::vector<const Alpha*> &out_v) const {
		assert(addedStrings.size() == 2);
		const Alpha *s = addedStrings[0].first;
		const Alpha *t = addedStrings[1].first;
		int n = addedStrings[0].second - s - 1;
		int m = addedStrings[1].second - t - 1;
		for(int i = 0; i <= n - m; i ++) {
			int x = i, y = 0;
			while(1) {
				int matchLen = longestCommonExtension(0, x, 1, y);
				if(y + matchLen == m) {
					out_v.push_back(s + i);
					break;
				}
				if( *(s + x + matchLen) == wildCard ||
					*(t + y + matchLen) == wildCard) {
					x += matchLen + 1;
					y += matchLen + 1;
				}else break;
			}
		}
	}

	//Θ(k n)
	void findAllKMismatches(int k, std::vector<std::pair<int, const Alpha*> > &out_v) const {
		assert(addedStrings.size() == 2);
		const Alpha *s = addedStrings[0].first;
		const Alpha *t = addedStrings[1].first;
		int n = addedStrings[0].second - s - 1;
		int m = addedStrings[1].second - t - 1;
		for(int i = 0; i <= n - m; i ++) {
			int x = i, y = 0;
			for(int mismatches = 0; mismatches <= k; mismatches ++) {
				int matchLen = longestCommonExtension(0, x, 1, y);
				if(y + matchLen == m) {
					out_v.push_back(std::make_pair(mismatches, s + i));
					break;
				}
				x += matchLen + 1;
				y += matchLen + 1;
			}
		}
	}

};


int main() {
	void tests();
	void matching_interactions();
//	tests();
	matching_interactions();
	return 0;
}
void matching_interactions() {
	while(1) {
	std::string s;
	std::getline(std::cin, s);
	std::string t;
	std::getline(std::cin, t);

	s += '\0'; t += '\1';
	GeneralizedSuffixTree gst(s.length()+t.length());
	gst.addByteString(s.c_str());
	gst.addByteString(t.c_str());
	gst.leafTraversal();
	gst.buildLCEData();

	int lcp = gst.longestCommonExtension(0, 0, 1, 0);
	std::cout << "Longest Common Prefix: " << '"';
	for(int i = 0; i < lcp; i ++) std::cout << s[i];
	std::cout << '"' << std::endl;

	std::vector<const Alpha*> wwc;
	gst.findAllWithWildCards('?', wwc);
	std::cout << "wwc: ";
	for(int i = 0; i < (int)wwc.size(); i ++) {
		std::cout << '"';
		const Alpha *p = wwc[i];
		for(int j = 0; j < (int)t.length()-1; j ++)
			std::cout << *(p+j);
		std::cout << '"';
		if(i+1 != wwc.size()) std::cout << ", ";
	}
	std::cout << std::endl;

	int missK = 2;
	std::vector<std::pair<int, const Alpha*> > miss;
	gst.findAllKMismatches(2, miss);
	std::cout << "miss" << missK << ": ";
	for(int i = 0; i < (int)miss.size(); i ++) {
		std::cout << '"';
		const Alpha *p = miss[i].second;
		for(int j = 0; j < (int)t.length()-1; j ++)
			std::cout << *(p+j);
		std::cout << '"' << '(' << miss[i].first << ')';
		if(i+1 != miss.size()) std::cout << ", ";
	}
	std::cout << std::endl;
	}
}

void some_interactions() {
	while(1) {
	std::string str;
	std::getline(std::cin, str);

#if 0
	int k;
	std::cin >> k;
	while(getchar() != '\n');
#endif

	GeneralizedSuffixTree t(str.length()+1);
	t.addByteString(str.c_str());
	t.leafTraversal();
#if 0
	std::vector<std::vector<const char*> > v;
	int maxLen = t.allLongestKRepeatedSubstrings(k, *(std::vector<std::vector<const Alpha*> >*)&v);
	std::cout << "len: " << maxLen << std::endl;
	for(int i = 0; i < (int)v.size(); i ++) {
		std::cout << '"';
		for(int j = 0; j < maxLen; j ++)
			std::cout << v[i][0][j];
		std::cout << '"' << ": ";
		std::sort(v[i].begin(), v[i].end());
		for(int j = 0; j < (int)v[i].size(); j ++) {
			if(j != 0) std::cout << ", ";
			std::cout << v[i][j]-str.c_str();
		}
		std::cout << std::endl;
	}
#elif 0
	std::vector<std::pair<int, std::vector<const char*> > > v;
	std::vector<GeneralizedSuffixTree::MaximalTuple> *v_a = (std::vector<GeneralizedSuffixTree::MaximalTuple>*)&v;
	t.allSupermaximalKRepeatedSubstrings(k, *v_a);
	for(int i = 0; i < (int)v.size(); i ++) {
		std::cout << '"';
		std::vector<const char*> &w = v[i].second;
		for(int j = 0; j < v[i].first; j ++)
			std::cout << w[0][j];
		std::cout << '"' << ": ";
		std::sort(w.begin(), w.end());
		for(int j = 0; j < (int)w.size(); j ++) {
			if(j != 0) std::cout << ", ";
			std::cout << w[j]-str.c_str();
		}
		std::cout << std::endl;
	}
#elif 0
	std::vector<std::pair<int, std::vector<const char*> > > v;
	std::vector<GeneralizedSuffixTree::MaximalTuple> *v_a = (std::vector<GeneralizedSuffixTree::MaximalTuple>*)&v;
	t.allMaximalKRepeatedTupleCombinations(k, *v_a);
	for(int i = 0; i < (int)v.size(); i ++) {
		std::cout << '"';
		std::vector<const char*> &w = v[i].second;
		for(int j = 0; j < v[i].first; j ++)
			std::cout << w[0][j];
		std::cout << '"' << " ";
		std::sort(w.begin(), w.end());
		for(int j = 0; j < (int)w.size(); j ++) {
			if(j != 0) std::cout << ", ";
			std::cout << w[j]-str.c_str();
		}
		std::cout << std::endl;
	}
#elif 0
	std::vector<std::pair<const char*, const char*> > v;
	t.allMaximalKRepeatedSubstrings_stringOnly(k, *(std::vector<std::pair<const Alpha*, const Alpha*> >*)&v);
	for(int i = 0; i < (int)v.size(); i ++) {
		std::cout << '"';
		for(const char *p = v[i].first; p < v[i].second; ++ p)
			std::cout << *p;
		std::cout << '"' << std::endl;
	}
#elif 0
	std::vector<std::pair<const char*, const char*> > v;
	int count = t.allMostFrequentlyOccurringSubstring(k, *(std::vector<GeneralizedSuffixTree::StrPair>*)&v);
	std::cout << "count: " << count << std::endl;
	for(int i = 0; i < (int)v.size(); i ++) {
		std::cout << '"';
		for(const char *p = v[i].first; p < v[i].second; ++ p)
			std::cout << *p;
		std::cout << '"' << std::endl;
	}
#elif 0
	std::vector<unsigned char> alphas, v;
	for(int i = 0; i < k; i ++) alphas.push_back('a' + i);
	t.notOccurringShortestString(alphas, v);
	std::cout << '"';
	for(int i = 0; i < (int)v.size(); i ++)
		std::cout << (char)v[i];
	std::cout << '"' << std::endl;
#elif 0
	std::vector<GeneralizedSuffixTree::StrPair> v;
	int len = t.allShortestOccurringOnlyOnceSubstrings(v);
	std::cout << "length: " << len << std::endl;
	for(int i = 0; i < (int)v.size(); i ++) {
		for(const unsigned char *p = v[i].first; p < v[i].second; ++ p)
			std::cout << (char)*p;
		std::cout << std::endl;
	}
#endif
	}
}

void matchingstatistics_interaction() {
	std::string str;
	std::getline(std::cin, str);
	std::cout << "input OK; length: " << str.length() << std::endl;
	GeneralizedSuffixTree t(str.length()+1);
	std::cout << "construct Suffix Tree ...";
	t.addByteString(str.c_str());
	std::cout << "OK" << std::endl;
	while(1) {
	std::string findstr;
	std::getline(std::cin, findstr);
	const Alpha *begin = (const Alpha*)findstr.c_str(), *end = begin + findstr.length();
	std::vector<int> statistics(end - begin);
	t.matchingStatistics(begin, end, statistics.data());

	std::string fs = findstr + '\1';
	GeneralizedSuffixTree u(str.length()+1+fs.length());
	u.addByteString(str.c_str());
	u.addByteString(fs.c_str());
	u.leafTraversal();
	u.buildLCEData();
	for(const Alpha *p = begin; p < end; p ++) {
		std::cout << p << ": " << statistics[p - begin] << ": ";
		int mx = 0;
		for(int i = 0; i < (int)str.length(); i ++) {
			int x = u.longestCommonExtension(0, i, 1, p - begin);
			mx = std::max(mx, x);
			for(int j = i; j < i+x; j ++)
				std::cout << str[j];
			if(i != str.length()) std::cout << ",";
		}
		std::cout << std::endl;
		assert(mx == statistics[p - begin]);
	}

	}
}

void findall_etc_interaction() {
	std::fstream f("C:\\test\\sagyo\\e.txt");
	std::string str;
	int lines = 0;
	while(1) {
		std::string tmp;
		std::getline(f, tmp);
		if(tmp == "$") break;
		str += tmp;
		lines ++;
	}
	std::cout << "input OK; length: " << str.length() << ", lines: " << lines << std::endl;
	GeneralizedSuffixTree t(str.length()+1);
	std::cout << "construct Suffix Tree ...";
	t.addByteString(str.c_str());
	std::cout << "OK" << std::endl;
	std::cout << "Euler tour ...";
	t.leafTraversal();
	std::cout << "OK" << std::endl;
	t.makeFindFirstData();
	while(1) {
	std::vector<const char *> v;
	std::string findstr;
	std::getline(std::cin, findstr);
	const Alpha *begin = (const Alpha*)findstr.c_str(), *end = begin + findstr.length();

	int count = t.countSubstring(begin, end);

	const Alpha *first = t.findFirst(begin, end);

	std::cout << "count: " << count << "; display all? (y/n): " << std::flush;
	std::string yn; std::getline(std::cin, yn);
	if(yn == "Y" || yn == "y") {
		t.findAll(begin, end, *(std::vector<const Alpha*>*)&v);
		assert(v.size() == count);
		std::sort(v.begin(), v.end());
		assert(v.empty() ? first == NULL : (const Alpha*)v[0] == first);
		for(int i = 0; i < (int)v.size(); i ++) {
			std::cout << v[i] - str.c_str() << ": ";
			const char *p;
			for(p = v[i]; *p && p < v[i] + 50; ++ p) std::cout << *p;
			if(*p) std::cout << "...";
			std::cout << std::endl;
		}
	}

	}
}

void branch_implement_speed_compare() {
	for(int ii = 0; ii < 10; ii ++) {
	static Alpha s[10000001];
	int l = sizeof(s)/sizeof(Alpha)-1, m = NAlpha - 1;
	//Array n=10^7 random   3  3600ms
	//List         random   3  4900ms
	//List         random  26 28400ms
	//ListA p=0    random  26  7000ms  508596
	//ListA p=5    random  26  9100ms  399082
	//ListA p=13   random  26 14000ms  172458
	//ListA p=24   random  26 17500ms   16276
	//ListA p=26   random  26 28700ms       0
	//ListB p=0    random   3  7600ms 5506556
	//ListB p=1    random   3  7000ms 4206663
	//ListB p=2    random   3  5000ms       0
	//ListB p=0    random  26  6600ms 1209396
	//ListB p=13   random  26 12800ms  309910
	//ListB p=0    random 256  4400ms  452262
	//ListB p=1    random 256  4400ms  451072
	//ListB p=2    random 256  4000ms  119840
	//ListB p=4    random 256  3800ms   65308
	//ListB p=8    random 256  4000ms   64772
	//ListB p=16   random 256  4800ms   64772
	//ListB p=32   random 256  8400ms   64772
	//ListB p=64   random 256 24100ms   64772
	//ListB p=128  random 256 81500ms    3365
	//ListB p=256  random 256 ...           0
	//ListB p=0    fib2   256 ...           0
	//Map          random   3 -       OutOfMemory
	//Hash         random   3 -       OutOfMemory
	//Treap        random   3 31800ms
	//Treap        random 256 53300ms
	//
	//charに収まりきらないほど大きいアルファベットの場合は、やはりハッシュマップ戦略がいいと思う。
#if 0	//FIBONACCI
	int al = 1, bl = 2;
	s[0] = 1; s[1] = 2;
	for(int j = 2; j < l; ) {
		memcpy(s+j, s, std::min(al, l-j)); j += al;
		al = bl; bl = j;
	}
#elif 1	//RANDOM
	for(int j = 0; j < l; j ++) s[j] = rand()/((RAND_MAX+m)/m) + 1;
#elif 0	//CONST1
	for(int j = 0; j < l; j ++) s[j] = 1;
#endif
	s[l] = 0;
	GeneralizedSuffixTree t(l+1);
	clock_t c = clock();
	t.addString((Alpha*)s);
	std::cout << (clock() - c) * 1000. / CLOCKS_PER_SEC << "msec." << std::endl;
	}
}

void tests() {
	void test(const char*, int = -1);
	test("0100101001001");
	test("asaadasdadasdasdasd");
	test("asdasdjkasjdasd");
	test("asdreqwrer");
	test("afdafasf");
	test("qqeweqwe");
	test("0");
	test("");
	test("00");
	test("010");
	test("cdddcdc");
	test("abcabxabcd");
	int testcase = 0;
	for(int ii = 0; ii < 1000; ii ++) {
		static char b[100];
		int l = rand() % sizeof(b), m = rand() % 26 + 1;
		for(int i = 0; i < l; i ++) {
			b[i] = 'a' + rand() % m;
		}
		b[l] = 0;
		test(b, testcase + ii);
	}
	testcase += 10000;

}

void test_isSuffix(GeneralizedSuffixTree &t, const char *s) {
	int len = strlen(s);

//	t.debugOutDotGraph(std::cout);

	char *buf = new char[len+1];
	memcpy(buf, s, len+1);
	for(int i = 0; i <= len; i ++) {
		assert(t.isSuffix((Alpha*)(s + i), (Alpha*)(s + len), '\0'));
		for(int j = i; j < len; j ++) {
			char &c = buf[j], d;
			while((d = 'a' + rand() % 26) == c) ;
			std::swap(c, d);
			assert(!t.isSuffix((Alpha*)(buf + i), (Alpha*)(buf + len), '\0'));
			std::swap(c, d);
		}
	}
	delete[] buf;
}

void test_matchingStatistics(GeneralizedSuffixTree &t, const char *s) {
	int len = strlen(s);
	for(int ii = 0; ii < 100; ii ++) {
		static char b[100];
		static int statistics[sizeof(b)];
		int l = rand() % sizeof(b), m = rand() % 26 + 1;
		for(int i = 0; i < l; i ++) {
			b[i] = 'a' + rand() % m;
		}
		t.matchingStatistics((Alpha*)b, (Alpha*)b+l, statistics);
		for(int i = 0; i < l; i ++) {
			int ans = t.commonPrefixLength((Alpha*)b+i, (Alpha*)b+l);
			assert(statistics[i] == ans);
		}
	}
}

void test(const char *s, int num = -1) {
	std::cout << "test ";
	if(num == -1) std::cout << s;
	else std::cout << num;
	std::cout << " ..." << std::flush;
	int len = strlen(s);
	GeneralizedSuffixTree t(len+1);
	t.addString((Alpha*)s);

	test_isSuffix(t, s);
	test_matchingStatistics(t, s);

	std::cout << "OK\r" << std::flush;
}

void poj2217() {
	int n;
	std::cin >> n; while(getchar() != '\n');
	for(int ii = 0; ii < n; ii ++) {
	std::string s, t;
	std::getline(std::cin, s);
	std::getline(std::cin, t);
	s += '\0'; t += '\1';
	GeneralizedSuffixTree u(s.length() + t.length());
	u.addByteString(s.c_str());
	u.addByteString(t.c_str());
//	u.debugOutDotGraph(std::cout, false);
	int ans = u.longestCommonSubstringLength(3);
	std::cout << "Nejdelsi spolecny retezec ma delku " << ans << "." << std::endl;
	}
}
