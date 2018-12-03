#include <iostream>
#include <algorithm>
#include <utility>
#include <functional>
#include <vector>
#include <queue>
#include <cstdio>

//元論文: <http://www.ics.uci.edu/~eppstein/pubs/Epp-SJC-98.pdf>
//紹介スライド: <http://www.isi.edu/natural-language/people/epp-cs562.pdf>
//Steps(論文のものと変数の名前などが違うので注意)
//1. shortest path treeを構築する(ここはDAGである時などのために分離できる)
//2. それぞれの頂点vに対して、vから出ているsidetrackのうち最小のものをbest(v)とする。
//   それぞれの頂点vに対して、vからtへのパス上の頂点wに対してbest(w)の集合からなる二分ヒープを作る(H_best(v)とする)。
//3. それぞれの頂点vに対して、vから出ているsidetracksからbest(v)を引いたものの集合からなる二分ヒープを作る(H_out(v)とする)。
//4. それぞれの頂点vに対して、H_best(v)の根からH_out(v)への辺を張る(これをH_3(v)とする)。
//5. それぞれの頂点vに対して、H_3(v)のそれぞれのノードnに対して、nが表現する辺の行き先の頂点をuとすると、
//   nからH_3(u)の根への辺を張る。全て合わせてD_4'とする。
//6. D_4'の中でもともとH_3(s)であったノードへ新たに作った根から辺を張る。これをD_4とする。
//7. D_4の辺を重みつきにする。5または6で追加された辺の重みは行き先のノードの重みとする。
//   それ以外の辺の重みは重みの差とする。
//8. いま、D_4は出次数たかだか4のグラフで、根からある頂点へのウォークは元のグラフGでのsからtへのウォークに対応する。
//   距離は(D_4上の距離 + sからtへの最短距離)である。
//   2で構築したヒープを降りることは、shortest path treeのある部分を通ることに対応する。
//   3で構築したヒープを降りることは、ある頂点から出ているsidetrackを選ぶことに対応する。
//   5で追加した辺を通ることは、そのsidetrack edgeを通ることに対応する。
//   ある頂点で終了することは、その頂点に対応するsidetrack edgeを最後に通ってそこからshortest path treeを通ってtへ行くことに対応する。
//9. このグラフ上をnearest first searchすることによってO(k log k)で見つけられる。
//
//Improvements
//1. 2で永続的にやって、また5で辺を張る時には暗黙的または破壊的にやってシェアさせることによってO(n log n + m)となる。
//2. さらに、なんか木を分解したりすると線形で構築できるらしい。
//3. D_4上でk番目を得るのはFredericksonによるアルゴリズムでO(k)でできる。
//
//TODO:
//・実際のパスを得られるようにする
//・辺の数などの値を実際にパスを辿らなくても得られるようにする
//・DAGに対応する(負の辺にも)
//・Θ(k)のヒープk番目アルゴリズム？
//・線形での構築？
//
//パスを記録するには(k番目→(prevが何番目か,辺))をk個持てばいいかな
//Θ(k)のヒープk番目アルゴリズムは小さい方からk個全部取得することには応用できないっぽい？
//  最後に配列上のselecting algorithmを使っているので。
//  いや、k番目のものを得た後にそれ以下のものをDFSで列挙すれば *順不同でなら* k個列挙できる。
//  そもそも順番通りにやるにはΩ(k log k)が必要。Θ(n)でヒープ構築してそれを使えばソートできてしまうので。
//
//k番目の閉路: <http://utpc2013.contest.atcoder.jp/submissions/140415>

struct WeightedStaticGraph {
	typedef int Vertex;
	typedef long long Weight; static const Weight InfWeight;
	struct To {
		Vertex to; Weight w;
		To() { }
		To(Vertex to_, Weight w_): to(to_), w(w_) { }
	};
	typedef std::pair<Vertex,To> Edge;
	typedef const To *const_iterator;

	void init(int n_, const std::vector<Edge> &edges) {
		n = n_; int m = edges.size();
		tos.resize(m+1); offsets.resize(n+1);
		for(int e = 0; e < m; e ++) offsets[edges[e].first] ++;
		for(int v = 1; v <= n_; v ++) offsets[v] += offsets[v-1];
		for(int e = 0; e < m; e ++)
			tos[-- offsets[edges[e].first]] = edges[e].second;
	}
	int numVertices() const { return n; }

	inline const_iterator edgesBegin(int v) const { return &tos[offsets[v]]; }
	inline const_iterator edgesEnd(int v) const { return &tos[offsets[v+1]]; }
private:
	int n;
	std::vector<To> tos;
	std::vector<int> offsets;
};
const WeightedStaticGraph::Weight WeightedStaticGraph::InfWeight = 0x3f3f3f3f3f3f3f3fLL;	//2倍がオーバーフローしない必要がある
typedef WeightedStaticGraph Graph;

void transposeGraph(const WeightedStaticGraph &g, WeightedStaticGraph &h) {
	int n = g.numVertices();
	std::vector<Graph::Edge> edges;
	for(int v = 0; v < n; v ++)
		for(Graph::const_iterator it = g.edgesBegin(v), et = g.edgesEnd(v); it != et; ++ it) {
			Graph::To t = *it; t.to = v;
			edges.push_back(Graph::Edge(it->to, t));
		}
	h.init(n, edges);
}

class KShortestPaths {
	friend int main();
public:
	typedef Graph::Weight Weight; typedef Graph::Vertex Vertex;

	void build(const Graph &g, Vertex destination);
	void buildMain(const Graph &g, const Weight dist[], const Vertex treeParent[], const std::vector<Vertex> &treeOrder);

	void getDistances(Vertex source, int k, std::vector<Weight> &lengths) const;
private:
	typedef int NodeIndex;
	struct HBestNode {	//H_bestを構成するノード(永続的2分ヒープ)
		Vertex vertex;	//どの頂点のbestか
		NodeIndex left, right;	//ヒープの子。ポインタの代わりにインデックス
	};
	std::vector<HBestNode> hBestNodes;
	std::vector<NodeIndex> hBestRoots;
	std::vector<bool> hasSidetrack;
	std::vector<Graph::To> bestSidetracks;
	std::vector<Graph::To> sidetrackHeaps;
	std::vector<int> sidetrackHeapOffsets;
	std::vector<Weight> toDestinationDistance;
	std::vector<int> vertexIndices;

	void buildShortestPathTree(const Graph &g, int t, Weight dist[], Vertex prev[], std::vector<Vertex> &order) const;

	NodeIndex insertToBestHeap(NodeIndex rootNode, int heapSize, Vertex v);
	NodeIndex newHBestNode(Vertex v, NodeIndex left = -1, NodeIndex right = -1);

	struct HeapCompareEdge {
		bool operator()(const Graph::To &x, const Graph::To &y) const { return x.w > y.w; }
	};

	struct SearchState {
		Weight dist;
		Vertex vertex;
		int index;
		SearchState() { }
		SearchState(Weight d, Vertex v, int i): dist(d), vertex(v), index(i) { }
		bool operator>(const SearchState &that) const { return dist > that.dist; }
	};
};

void KShortestPaths::buildShortestPathTree(const Graph &g, int t, Weight dist[], Vertex prev[], std::vector<Vertex> &order) const {
	int n = g.numVertices();
	Graph h; transposeGraph(g, h);
	for(int v = 0; v < n; v ++) {
		dist[v] = Graph::InfWeight;
		prev[v] = -1;
	}
	dist[t] = 0;
	order.clear();
	std::vector<bool> visited(n, false);
	typedef std::pair<Weight,Vertex> DistPair;
	std::priority_queue<DistPair,std::vector<DistPair>,std::greater<DistPair> > q;
	q.push(DistPair(0, t));
	while(!q.empty()) {
		Vertex v = q.top().second; q.pop();
		if(visited[v]) continue;
		visited[v] = true;
		order.push_back(v);
		for(Graph::const_iterator it = h.edgesBegin(v), et = h.edgesEnd(v); it != et; ++ it) {
			Weight d = dist[v] + it->w; Vertex u = it->to;
			if(d < dist[u]) {
				prev[u] = v;
				dist[u] = d;
				q.push(DistPair(d, u));
			}
		}
	}
}

KShortestPaths::NodeIndex KShortestPaths::newHBestNode(Vertex v, NodeIndex left, NodeIndex right) {
	HBestNode newNode = { v, left, right };
	hBestNodes.push_back(newNode);
	return (NodeIndex)hBestNodes.size() - 1;
}
 
KShortestPaths::NodeIndex KShortestPaths::insertToBestHeap(NodeIndex rootNode, int heapSize, Vertex v) {
	if(rootNode == -1) return newHBestNode(v, -1, -1);
	int newPosition = heapSize + 1, height = 0;
	while((1 << height) <= newPosition) height ++;	//1: 1, 2,3: 2
	NodeIndex levelNodes[32], currentNode = rootNode;
	int insertHeight = 0;
	for(int h = height - 1; h > 0; h --) {
		levelNodes[h] = currentNode;
		const HBestNode &cnt = hBestNodes[currentNode];
		if(insertHeight == 0 && bestSidetracks[v].w <= bestSidetracks[cnt.vertex].w)
			insertHeight = h;
		currentNode = newPosition >> (h-1) & 1 ? cnt.right : cnt.left;
	}
	currentNode = -1;
	for(int h = 0; h <= insertHeight; h ++) {
		Vertex u = h == insertHeight ? v : hBestNodes[levelNodes[h+1]].vertex;
		if(h == 0)
			currentNode = newHBestNode(u, -1, -1);
		else if(newPosition >> (h-1) & 1)
			currentNode = newHBestNode(u, hBestNodes[levelNodes[h]].left, currentNode);
		else
			currentNode = newHBestNode(u, currentNode, hBestNodes[levelNodes[h]].right);
	}
	for(int h = insertHeight+1; h < height; h ++) {
		const HBestNode &node = hBestNodes[levelNodes[h]];
		if(newPosition >> (h-1) & 1)
			currentNode = newHBestNode(node.vertex, node.left, currentNode);
		else
			currentNode = newHBestNode(node.vertex, currentNode, node.right);
	}
	return currentNode;
}

void KShortestPaths::buildMain(const Graph &g, const Weight dist[], const Vertex treeParent[], const std::vector<Vertex> &treeOrder) {
	int n = g.numVertices();
	hBestNodes.clear();
	hBestRoots.assign(n, -1);
	bestSidetracks.resize(n);
	hasSidetrack.assign(n, false);
	bestSidetracks.resize(n);
	sidetrackHeaps.clear();
	sidetrackHeapOffsets.resize(treeOrder.size()+1);
	toDestinationDistance.assign(dist, dist + n);
	vertexIndices.assign(n, -1);
	std::vector<int> hBestSizes(n, 0);

	for(int idx = 0; idx < (int)treeOrder.size(); idx ++) {
		Vertex v = treeOrder[idx], p = treeParent[v];
		vertexIndices[v] = idx;

		Graph::To bestSidetrack; bestSidetrack.w = Graph::InfWeight;
		int treeEdgeIndex = -1, bestEdgeIndex = -1;
		{	bool sawTreeEdge = false;
			int ei = 0; 
			for(Graph::const_iterator it = g.edgesBegin(v), et = g.edgesEnd(v); it != et; ++ it, ++ ei) {
				Vertex u = it->to; Weight delta = it->w - (dist[v] - dist[u]);
				if(!sawTreeEdge && treeParent[v] == u && delta == 0) {
					treeEdgeIndex = ei;
					sawTreeEdge = true;
				}else if(bestSidetrack.w > delta) {
					hasSidetrack[v] = true;
					bestEdgeIndex = ei;
					Graph::To t = *it; t.w = delta;
					bestSidetrack = t;
				}
			}
		}
		bestSidetracks[v] = bestSidetrack;
		hBestRoots[v] = p == -1 ? -1 : hBestRoots[p];
		hBestSizes[v] = p == -1 ? 0 : hBestSizes[p];
		if(hasSidetrack[v]) {
			hBestRoots[v] = insertToBestHeap(hBestRoots[v], hBestSizes[v], v);
			hBestSizes[v] ++;
		}
		sidetrackHeapOffsets[idx] = sidetrackHeaps.size();
		{	int ei = 0;
			for(Graph::const_iterator it = g.edgesBegin(v), et = g.edgesEnd(v); it != et; ++ it, ++ ei) {
				Vertex u = it->to; Weight delta = it->w - (dist[v] - dist[u]);
				if(ei != treeEdgeIndex && ei != bestEdgeIndex && delta < Graph::InfWeight) {
					Graph::To t = *it; t.w = delta;
					sidetrackHeaps.push_back(t);
				}
			}
		}
		std::make_heap(sidetrackHeaps.begin() + sidetrackHeapOffsets[idx], sidetrackHeaps.end(), HeapCompareEdge());
	}
	sidetrackHeapOffsets[treeOrder.size()] = sidetrackHeaps.size();
}

void KShortestPaths::build(const Graph &g, Vertex destination) {
	int n = g.numVertices();
	std::vector<Graph::Weight> dist(n);
	std::vector<Graph::Vertex> treeParent(n);
	std::vector<Graph::Vertex> treeOrder;
	buildShortestPathTree(g, destination, &dist[0], &treeParent[0], treeOrder);
	buildMain(g, &dist[0], &treeParent[0], treeOrder);
}

void KShortestPaths::getDistances(Vertex source, int k, std::vector<Weight> &lengths) const {
	lengths.clear();
	Weight shortestDistance = toDestinationDistance[source];
	if(shortestDistance == Graph::InfWeight || k <= 0) return;
	lengths.push_back(shortestDistance);
	int hBests = hBestNodes.size();
	//indexが-1ならbestTree、そうでないならsidetrackHeap
	typedef SearchState State;
	std::priority_queue<State,std::vector<State>,std::greater<State> > q;
	NodeIndex root = hBestRoots[source];
	if(root != -1) {
		Vertex rootVertex = hBestNodes[root].vertex;
		q.push(State(bestSidetracks[rootVertex].w, rootVertex, root));
	}
	while(!q.empty() && (int)lengths.size() < k) {
		Weight d = q.top().dist;
		Vertex v = q.top().vertex, vIndex = vertexIndices[v];
		int index = q.top().index;
		q.pop();
		lengths.push_back(shortestDistance + d);
		if(index < hBests) {
			const HBestNode &node = hBestNodes[index];
			const Graph::To &edge = bestSidetracks[v];
			Weight e = d - edge.w;
			//H_bestの子たち
			NodeIndex left = node.left, right = node.right;
			if(left != -1) {
				Vertex leftVertex = hBestNodes[left].vertex;
				q.push(State(e + bestSidetracks[leftVertex].w, leftVertex, left));
			}
			if(right != -1) {
				Vertex rightVertex = hBestNodes[right].vertex;
				q.push(State(e + bestSidetracks[rightVertex].w, rightVertex, right));
			}
			//辺の行き先
			Vertex u = edge.to; NodeIndex uRoot = hBestRoots[u];
			if(uRoot != -1) {
				Vertex rootVertex = hBestNodes[uRoot].vertex;
				q.push(State(d + bestSidetracks[rootVertex].w, rootVertex, uRoot));
			}
			//best以外の辺
			int heapRoot = sidetrackHeapOffsets[vIndex];
			if(heapRoot != sidetrackHeapOffsets[vIndex+1])
				q.push(State(e + sidetrackHeaps[heapRoot].w, v, hBests + heapRoot));
		}else {
			const Graph::To &edge = sidetrackHeaps[index - hBests];
			Weight e = d - edge.w;
			int heapBase = sidetrackHeapOffsets[vIndex], heapEnd = sidetrackHeapOffsets[vIndex+1];
			int localIndex = index - hBests - heapBase;
			int left = heapBase + localIndex * 2 + 1, right = heapBase + localIndex * 2 + 2;
			//H_outの子たち
			if(left < heapEnd)
				q.push(State(e + sidetrackHeaps[left].w, v, hBests + left));
			if(right < heapEnd)
				q.push(State(e + sidetrackHeaps[right].w, v, hBests + right));
			//辺の行き先
			Vertex u = edge.to; NodeIndex uRoot = hBestRoots[u];
			if(uRoot != -1) {
				Vertex rootVertex = hBestNodes[uRoot].vertex;
				q.push(State(d + bestSidetracks[rootVertex].w, rootVertex, uRoot));
			}
		}
	}
}

#include "../Util/TimeIt_Simple.hpp"
#include "../Util/GetCurrentHeapSize.hpp"
double getMiB(long long x) { return (int)(x / 1024. / 1024. * 1000) / 1000.; }
int main() {
	using namespace std;
	int n, k;
	scanf("%d%d", &n, &k);
	vector<Graph::Edge> edges;
	for(int i = 0; i < n; i ++) {
		for(int j = 0; j < 2; j ++)
			edges.push_back(Graph::Edge(i, Graph::To((i+1)%n, 1)));
	}
	cerr << "Nodes, Edges:   " << n << ", " << edges.size() << endl;
	int s = 0, t = 0;
	Graph g; g.init(n, edges);
	long long h1 = getCurrentHeapSize(), mem;
	int bestHeapNodes, bestHeapCapa;
	TIMEIT("Eppstein") {
	KShortestPaths kShortestPaths;
	kShortestPaths.build(g, t);
	vector<Graph::Weight> lengths;
	TIMEIT("getDistances") {
	kShortestPaths.getDistances(s, k, lengths);
	}
	bestHeapNodes = kShortestPaths.hBestNodes.size();
	bestHeapCapa = kShortestPaths.hBestNodes.capacity();
	mem = getCurrentHeapSize() - h1;
	}
	cerr << "Total:         " << getMiB(mem) << "MiB" << endl;
	cerr << "bestHeapNodes: " << getMiB(bestHeapCapa * sizeof(KShortestPaths::HBestNode)) << "MiB" << " (" << bestHeapNodes << ", x" << (int)(bestHeapNodes * 1. / n * 100) / 100. << ")" << endl;
	/*
	for(int i = 0; i < k; i ++) {
		if((int)lengths.size() <= i)
			puts("-1");
		else
			printf("%lld\n", lengths[i]);
	}
	*/
	return 0;
}
