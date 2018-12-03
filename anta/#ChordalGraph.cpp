#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <map>
#include <queue>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstring>
#include <cctype>
#include <list>
#include <cassert>
#define rep(i,n) for(int (i)=0;(i)<(int)(n);++(i))
#define rer(i,l,u) for(int (i)=(int)(l);(i)<=(int)(u);++(i))
#define reu(i,l,u) for(int (i)=(int)(l);(i)<(int)(u);++(i))
#if defined(_MSC_VER)
#define aut(r,v) auto r = (v)
#else
#define aut(r,v) typeof(v) r = (v)
#endif
#define each(it,o) for(aut(it, (o).begin()); it != (o).end(); ++ it)
#define all(o) (o).begin(), (o).end()
#define pb(x) push_back(x)
#define mp(x,y) make_pair((x),(y))
#define mset(m,v) memset(m,v,sizeof(m))
#define INF 0x3f3f3f3f
#define INFL 0x3f3f3f3f3f3f3f3fLL
#define EPS 1e-9
using namespace std;
typedef vector<int> vi; typedef pair<int,int> pii; typedef vector<pair<int,int> > vpii;
typedef long long ll; typedef vector<long long> vl; typedef pair<long long,long long> pll; typedef vector<pair<long long,long long> > vpll;
typedef vector<string> vs; typedef long double ld;

string vertexStr(int v) {
	stringstream ss;
	ss << v;
	return ss.str();
}
int vertexRead(char a) {
	return '0' <= a && a <= '9' ? a - '0' : a - 'a';
}

string backetsStr(int headbacket, const vector<int> &nextbacket, const vector<int> &headvertex, const vector<int> &nextvertex) {
	stringstream ss;
	ss << "{";
	int backet = headbacket;
	while(backet != -1) {
		int vertex = headvertex[backet];
		ss << "[";
		while(vertex != -1) {
			ss << vertexStr(vertex);
			vertex = nextvertex[vertex];
			if(vertex != -1) ss << " ";
		}
		ss << "]";
		backet = nextbacket[backet];
		if(backet != -1) ss << " ";
	}
	ss << "}";
	return ss.str();
}

typedef vector<vector<int> > Graph;
void lexicographicBFS(const Graph &g, vector<int> &out_ord) {
	int n = g.size();
	out_ord.clear(); out_ord.reserve(n);

	vector<int>
		prevvertex(n, -2), nextvertex(n, -2), headvertex(n, -2),
		prevbacket(n, -2), nextbacket(n, -2), backetid(n, -2),
		backetlabelinit(n, -2), backetlabellast(n, -2),
		freebacketstack(n, -2);
	int headbacket = 0, freebacketsp = n-1;

	rep(i, n) backetid[i] = 0, prevvertex[i] = i-1, nextvertex[i] = i+1;
	nextvertex[n-1] = -1, headvertex[0] = 0;
	prevbacket[0] = -1, nextbacket[0] = -1;
	backetlabelinit[0] = -1, backetlabellast[0] = -1;
	rep(i, n-1) freebacketstack[i] = (n-1) - i;
	
	rep(t, n) {

		int v = headvertex[headbacket];
		out_ord.push_back(v);
		if(nextvertex[v] == -1) {
			//delete headbacket
			freebacketstack[freebacketsp ++] = headbacket;
			if(nextbacket[headbacket] != -1) prevbacket[headbacket = nextbacket[headbacket]] = -1;
		}
		else prevvertex[headvertex[headbacket] = nextvertex[v]] = -1;
		backetid[v] = -1;

//		cout << vertexStr(v) << " | " << backetsStr(headbacket, nextbacket, headvertex, nextvertex) << endl;;

		//辺がソートされていればそのままの順序になるように、後ろからやってる(デバッグの時に見た目がいいように)
		for(vector<int>::const_reverse_iterator u = g[v].rbegin(); u != g[v].rend(); ++ u) {
			int ubacket = backetid[*u];
			if(ubacket != -1) {
				int prevubacket = prevbacket[ubacket];
				if(prevubacket == -1
				|| backetlabelinit[prevubacket] != ubacket
				|| backetlabellast[prevubacket] != v) {
					//create backet at this place
					int newbacket = freebacketstack[-- freebacketsp];
					nextbacket[newbacket] = ubacket;
					prevbacket[newbacket] = prevubacket;
					if(prevubacket == -1) headbacket = newbacket;
					else nextbacket[prevubacket] = newbacket;
					prevbacket[ubacket] = newbacket;
					headvertex[newbacket] = -1;

					backetlabelinit[newbacket] = ubacket;
					backetlabellast[newbacket] = v;

					prevubacket = newbacket;
				}
				//remove u from ubacket
				if(prevvertex[*u] == -1 && nextvertex[*u] == -1) {
					//delete ubacket
					nextbacket[prevubacket] = nextbacket[ubacket];
					if(nextbacket[ubacket] != -1) prevbacket[nextbacket[ubacket]] = prevubacket;
					freebacketstack[freebacketsp ++] = ubacket;
				}else {
					if(prevvertex[*u] != -1) nextvertex[prevvertex[*u]] = nextvertex[*u];
					else headvertex[ubacket] = nextvertex[*u];
					if(nextvertex[*u] != -1) prevvertex[nextvertex[*u]] = prevvertex[*u];
				}
				//insert u into prevubacket
				prevvertex[*u] = -1;
				nextvertex[*u] = headvertex[prevubacket];
				if(headvertex[prevubacket] != -1) prevvertex[headvertex[prevubacket]] = *u;
				headvertex[prevubacket] = *u;
				backetid[*u] = prevubacket;

//				cout << "sepalate " << vertexStr(*u) << ": " << backetsStr(headbacket, nextbacket, headvertex, nextvertex) << endl;;
			}
		}
	}
}


bool isPerfectEliminationOrder(const Graph &g, const vector<int> &ord) {
	/*
		・parentの逆辺を作る
		・各頂点に対して、自分のNRをマークしてから、子どもたちにチェックさせる
	*/
	int n = g.size();
	vector<int> ord_inv(n);
	vector<int> headchild(n, -1), nextchild(n, -1);	//無駄に(リスト with 1次元)を使う(キャッシュ効率どうなんだろ？)
	vector<char> mark(n, 0);

	rep(i, n) ord_inv[ord[i]] = i;

	for(int i = n-1; i >= 0; i --) {	//childsのリストが昇順のほうが見やすいかなあって
		int parent = -1;
		for(vector<int>::const_iterator j = g[i].begin(); j != g[i].end(); ++ j)
			if(ord_inv[*j] < ord_inv[i] && (parent == -1 || ord_inv[parent] < ord_inv[*j]))
				parent = *j;
		if(parent != -1) {
			nextchild[i] = headchild[parent];
			headchild[parent] = i;
		}
	}
	rep(i, n) {
		if(headchild[i] == -1) continue;
		mark[i] = 1;
		for(vector<int>::const_iterator j = g[i].begin(); j != g[i].end(); ++ j)
			if(ord_inv[*j] < ord_inv[i]) mark[*j] = 1;
		int child = headchild[i];
		while(child != -1) {
			bool ok = true;
			for(vector<int>::const_iterator j = g[child].begin(); j != g[child].end(); ++ j)
				if(ord_inv[*j] < ord_inv[child])
					if(!mark[*j]) {
						ok = false;
						break;
					}
			if(!ok) return false;
			child = nextchild[child];
		}
		for(vector<int>::const_iterator j = g[i].begin(); j != g[i].end(); ++ j)
			if(ord_inv[*j] < ord_inv[i]) mark[*j] = 0;
		mark[i] = 0;
	}
	return true;
}

string undirectedGraphVisualize(const Graph &g) {
	int n = g.size();
	stringstream ss;
	ss << "http://chart.apis.google.com/chart?cht=gv:circo&chl=graph{";
	rep(i, n) ss << vertexStr(i) << ";";
	rep(i, n) each(j, g[i]) if(i <= *j)
		ss << vertexStr(i)<<"--"<<vertexStr(*j)<<";";
	ss << "}";
	return ss.str();
}


string levelsStr(int maximumlevel, const vector<int> &nextlevel, const vector<int> &headvertex, const vector<int> &nextvertex) {
	stringstream ss;
	ss << "{";
	int level = maximumlevel;
	while(level != -1) {
		int vertex = headvertex[level];
		ss << level << "[";
		while(vertex != -1) {
			ss << vertexStr(vertex);
			vertex = nextvertex[vertex];
			if(vertex != -1) ss << " ";
		}
		ss << "]";
		level = nextlevel[level];
		if(level != -1) ss << " ";
	}
	ss << "}";
	return ss.str();
}


void maximumCardinalitySearch(const Graph &g, vector<int> &out_ord) {
	int n = g.size();
	out_ord.clear(); out_ord.reserve(n);

	//prevlevelの方向にmaximumlevelがある(maximumlevel = l <-> prevlevel[l] = -1)
	vector<int>
		prevvertex(n, -2), nextvertex(n, -2), vertexlevel(n, -2),
		prevlevel(n+1, -2), nextlevel(n+1, -2), headvertex(n+1, -1);
	int maximumlevel = 0;

	rep(i, n) vertexlevel[i] = 0, prevvertex[i] = i-1, nextvertex[i] = i+1;
	nextvertex[n-1] = -1, headvertex[0] = 0;
	prevlevel[0] = -1, nextlevel[0] = -1;

	rep(t, n) {
		int v = headvertex[maximumlevel];

		out_ord.push_back(v);

		//remove v from maximumlevel and delete maximumlevel if it empty
		headvertex[maximumlevel] = nextvertex[v];
		if(nextvertex[v] != -1) prevvertex[nextvertex[v]] = -1;
		else {
			if(nextlevel[maximumlevel] != -1) prevlevel[nextlevel[maximumlevel]] = -1;
			maximumlevel = nextlevel[maximumlevel];
		}
		vertexlevel[v] = -1;

//		cout << vertexStr(v) << " | " << levelsStr(maximumlevel, nextlevel, headvertex, nextvertex) << endl;;

		//辺がソートされていればそのままの順序になるように、後ろからやってる(デバッグの時に見た目がいいように)
		for(vector<int>::const_reverse_iterator u = g[v].rbegin(); u != g[v].rend(); ++ u) {
			int ulevel = vertexlevel[*u];
			if(ulevel != -1) {
				//remove u from ulevel
				if(prevvertex[*u] != -1) nextvertex[prevvertex[*u]] = nextvertex[*u];
				else headvertex[ulevel] = nextvertex[*u];
				if(nextvertex[*u] != -1) prevvertex[nextvertex[*u]] = prevvertex[*u];

				int prevnextlevel = prevlevel[ulevel], nextnextlevel = ulevel;
				//delete ulevel if it empty
				if(headvertex[ulevel] == -1) {
					nextnextlevel = nextlevel[ulevel];
					if(prevnextlevel != -1) nextlevel[prevnextlevel] = nextnextlevel;
					else maximumlevel = nextlevel[ulevel];
					if(nextnextlevel != -1) prevlevel[nextnextlevel] = prevnextlevel;
				}

				//create (ulevel+1) if it not created
				if(prevnextlevel != ulevel+1) {
					headvertex[ulevel+1] = -1;
					prevlevel[ulevel+1] = prevnextlevel;
					nextlevel[ulevel+1] = nextnextlevel;

					if(prevnextlevel != -1) nextlevel[prevnextlevel] = ulevel+1;
					else maximumlevel = ulevel+1;
					if(nextnextlevel != -1) prevlevel[nextnextlevel] = ulevel+1;
				}

				//insert u into (ulevel+1)
				prevvertex[*u] = -1;
				nextvertex[*u] = headvertex[ulevel+1];
				if(headvertex[ulevel+1] != -1) prevvertex[headvertex[ulevel+1]] = *u;
				headvertex[ulevel+1] = *u;
				vertexlevel[*u] = ulevel+1;

//				cout << "increase level of " << vertexStr(*u) << " (" << ulevel << "+1): " << levelsStr(maximumlevel, nextlevel, headvertex, nextvertex) << endl;;
			}
		}
	}
}


/*

Algorithm 2: Maximum Cardinality Search and Clique-Tree
Data: A graph G = (V, E)
Result: If the input graph is chordal: a PEO and an associated clique-tree
T = (I, F) where I is the set of maximal cliques
begin
	each vertex of X is initialized with the empty set
	previousmark = 1
	j = 0
	for i = n to 1 do
		choose a vertex x not yet numbered such that mark(x) is maximum
		//クリークなら1つずつ増えていくので、そうじゃないなら別のクリークに行ったということで
1		if mark(x) <= previousmark then
			j = j + 1
			// (M(x) ∪ {x})はPEOでの(NR(v) ∪ {v})と一致する
			// それはPEOならクリークになるはず
			create the maximal clique C_j = M(x) ∪ {x}
			create the tie between C_j and C(last(x))
		else
			//1つずつ増えていくならクリークのはず
2			C_j = C_j ∪ {x}
3		for each y neighbour of x do
			M(y) = M(y) ∪ {x}
			mark(y) = mark(y) + 1
			last(y) = x
		previousmark = mark(x)
		x is numbered by i
		C(x) = j
end
*/
//vはout_treenode[v]のノードに含まれている
//out_treenode[v]がvならvはノードである
//また、ノードvはさらにNR(v)も含んでいる
//vのからout_treeedge[v]へと辺が張られている
//vがノードでないならout_treeedge[v] = -1
//ノードであって(out_treeedge[v] = -1)になるノードもちょうど1つだけ存在する(弦グラフならば)
void cliqueTreeMCS(const Graph &g, vector<int> &out_ord, vector<int> &out_treenode, vector<int> &out_treeedge) {
	int n = g.size();
	out_ord.clear(); out_ord.reserve(n);
	out_treenode.assign(n, -1); out_treeedge.assign(n, -1);

	//prevlevelの方向にmaximumlevelがある(maximumlevel = l <-> prevlevel[l] = -1)
	vector<int>
		prevvertex(n, -2), nextvertex(n, -2), vertexlevel(n, -2),
		prevlevel(n+1, -2), nextlevel(n+1, -2), headvertex(n+1, -1),
		lastneighbor(n, -1);
	int maximumlevel = 0, previouslevel = 1, lastnode = -1;

	rep(i, n) vertexlevel[i] = 0, prevvertex[i] = i-1, nextvertex[i] = i+1;
	nextvertex[n-1] = -1, headvertex[0] = 0;
	prevlevel[0] = -1, nextlevel[0] = -1;

	rep(t, n) {
		int v = headvertex[maximumlevel];

		out_ord.push_back(v);

		if(maximumlevel <= previouslevel) {
			//孤立点をそのままやるとどこにも繋がれずに孤立したままになってしまうので、lastnodeにでもつないどく？
			out_treeedge[v] = lastneighbor[v] == -1 ? lastnode : out_treenode[lastneighbor[v]];
			lastnode = v;
		}
		out_treenode[v] = lastnode;
		previouslevel = maximumlevel;

		//remove v from maximumlevel and delete maximumlevel if it empty
		headvertex[maximumlevel] = nextvertex[v];
		if(nextvertex[v] != -1) prevvertex[nextvertex[v]] = -1;
		else {
			if(nextlevel[maximumlevel] != -1) prevlevel[nextlevel[maximumlevel]] = -1;
			maximumlevel = nextlevel[maximumlevel];
		}
		vertexlevel[v] = -1;

//		cout << vertexStr(v) << " | " << levelsStr(maximumlevel, nextlevel, headvertex, nextvertex) << endl;;

		//辺がソートされていればそのままの順序になるように、後ろからやってる(デバッグの時に見た目がいいように)
		for(vector<int>::const_reverse_iterator u = g[v].rbegin(); u != g[v].rend(); ++ u) {
			int ulevel = vertexlevel[*u];
			if(ulevel != -1) {
				lastneighbor[*u] = v;

				//remove u from ulevel
				if(prevvertex[*u] != -1) nextvertex[prevvertex[*u]] = nextvertex[*u];
				else headvertex[ulevel] = nextvertex[*u];
				if(nextvertex[*u] != -1) prevvertex[nextvertex[*u]] = prevvertex[*u];

				int prevnextlevel = prevlevel[ulevel], nextnextlevel = ulevel;
				//delete ulevel if it empty
				if(headvertex[ulevel] == -1) {
					nextnextlevel = nextlevel[ulevel];
					if(prevnextlevel != -1) nextlevel[prevnextlevel] = nextnextlevel;
					else maximumlevel = nextlevel[ulevel];
					if(nextnextlevel != -1) prevlevel[nextnextlevel] = prevnextlevel;
				}

				//create (ulevel+1) if it avoid
				if(prevnextlevel != ulevel+1) {
					headvertex[ulevel+1] = -1;
					prevlevel[ulevel+1] = prevnextlevel;
					nextlevel[ulevel+1] = nextnextlevel;

					if(prevnextlevel != -1) nextlevel[prevnextlevel] = ulevel+1;
					else maximumlevel = ulevel+1;
					if(nextnextlevel != -1) prevlevel[nextnextlevel] = ulevel+1;
				}

				//insert u into (ulevel+1)
				prevvertex[*u] = -1;
				nextvertex[*u] = headvertex[ulevel+1];
				if(headvertex[ulevel+1] != -1) prevvertex[headvertex[ulevel+1]] = *u;
				headvertex[ulevel+1] = *u;
				vertexlevel[*u] = ulevel+1;

//				cout << "increase level of " << vertexStr(*u) << " (" << ulevel << "+1): " << levelsStr(maximumlevel, nextlevel, headvertex, nextvertex) << endl;;
			}
		}
	}
}

//cliqueTreeMCSで得られたものをグラフの形にする
void cliqueTreeMakeGraph(const Graph &g, const vector<int> &ord, const vector<int> &node, const vector<int> &edge, Graph &out_g, vector<vector<int> > &out_clique) {
	int n = g.size();

	//ノードを数え、連番をつける
	vector<int> nodenumber(n, -1);
	int nodes = 0;
	rep(i, n) if(node[i] == i) nodenumber[i] = nodes ++;

	out_g.assign(nodes, vector<int>());
	out_clique.assign(nodes, vector<int>());

	//まずエッジを
	rep(i, n) if(edge[i] != -1) {
		out_g[nodenumber[i]].push_back(nodenumber[edge[i]]);
		out_g[nodenumber[edge[i]]].push_back(nodenumber[i]);
	}

	//クリークのうちnodeにあるもの(PEO(普通の順序)で左にあるもの)を
	rep(i, n)
		out_clique[nodenumber[node[i]]].push_back(i);

	//クリークのうちNR(v)にあるもの(PEO(普通の順序)で右にあるもの)を
	vector<int> ord_rev(n);
	rep(i, n) ord_rev[ord[i]] = i;
	rep(v, n) if(nodenumber[v] != -1) {
		for(vector<int>::const_iterator u = g[v].begin(); u != g[v].end(); ++ u)
			if(ord_rev[*u] < ord_rev[v])
				out_clique[nodenumber[v]].push_back(*u);
	}
}

string cliqueStr(vector<int> clique) {
	sort(clique.begin(), clique.end());
	stringstream ss;
	ss << '"';
	rep(i, clique.size()) {
		ss << vertexStr(clique[i]);
		if(i+1 != clique.size()) ss << ",";
	}
	ss << '"';
	return ss.str();
}

string visualizeCliqueTree(const Graph &g, const vector<vector<int> > &clique) {
	int n = g.size();
	stringstream ss;
	ss << "http://chart.apis.google.com/chart?cht=gv:circo&chl=graph{";
	rep(i, n) ss << cliqueStr(clique[i]) << ";";
	rep(i, n) each(j, g[i]) if(i <= *j)
		ss << cliqueStr(clique[i])<<"--"<<cliqueStr(clique[*j])<<";";
	ss << "}";
	return ss.str();
}

//実際にcloringするにはPEOの逆順でgreedyにやればいいらしい。
//でもLinearはどうやればいいだろう？できるのかな？

//cliqueTreeMCSでordとnodeを得ること。
//chromaticNumber(彩色数) = maximumClique
//また、Treewidth = maximumClique-1
//clique treeはmaximal cliqueを列挙するので、
//weightedでも普通にできる
int chordalMaximumClique(const Graph &g, const vector<int> &ord, const vector<int> &node) {
	int n = g.size();

	vector<int> ord_rev(n), cliquesize(n, 0);
	rep(i, n) ord_rev[ord[i]] = i;

	rep(v, n) cliquesize[node[v]] ++;

	int maximum = 0;
	rep(v, n) if(node[v] == v) {
		for(vector<int>::const_iterator u = g[v].begin(); u != g[v].end(); ++ u)
			if(ord_rev[*u] < ord_rev[v])
				cliquesize[v] ++;
		maximum = max(maximum, cliquesize[v]);
	}
	return maximum;
}

//重み無し。weightedだと別のアルゴリズムが必要？
int chordalMaximumIndependenSet(const Graph &g, const vector<int> &ord) {
	int n = g.size();
	vector<char> mark(n, 0);
	int size = 0;
	for(int i = n-1; i >= 0; i --) {
		int v = ord[i];
		if(mark[v]) continue;
		size ++;
		each(u, g[v]) mark[*u] = 1;
	}
	return size;
}

struct ChordalWeightedIndependentSet {
	//Reference: http://www.jaist.ac.jp/~uehara/pdf/counting.pdf
	typedef int Weight;
	static const int MinusInf = -INF;

	const vector<Weight> &weight;
	//再帰のために
	Graph T;
	vector<vector<int> > cliques;
	vector<int> markbuffer;
	
	//rec(K) = O(|K|) + Σ_(L∈CHILDS(K)){ rec(L) }
	void rec(
			int parent, int K,
			Weight &out_notcontain, vector<Weight> &out_contain)
	{
		if(T[K].size() == 0 || (T[K].size() == 1 && T[K][0] == parent)) {	//K is a leaf of T
			out_notcontain = 0;
			out_contain.resize(cliques[K].size());
			rep(i, cliques[K].size())
				out_contain[i] = weight[cliques[K][i]];
		}else {
			vector<Weight> notcontain(T[K].size());
			vector<vector<Weight> > contain(T[K].size());

			//foreach child L of K do call #IndSetsIter(L);
			rep(i, T[K].size()) {
				int L = T[K][i];
				if(L == parent) continue;
				rec(K, L, notcontain[i], contain[i]);
			}

			//foreach child L of K do compute |NIS(G(L), K∩L)|
			// by |NIS(G(L), L)| + Σ_(u∈L-K){ |IS(G(L),u)| }
			vector<Weight> intersect(T[K].size(), MinusInf);
			vector<vector<pair<int,int> > > containchilds(cliques[K].size());

			rep(i, cliques[K].size()) markbuffer[cliques[K][i]] = i;	//O(|K|)
			rep(i, T[K].size()) {
				int L = T[K][i];
				if(L == parent) continue;
				intersect[i] = notcontain[i];
				rep(j, cliques[L].size()) if(markbuffer[cliques[L][j]] == -1)
					intersect[i] = max(intersect[i], contain[i][j]);	//O(Σ|L|)
				else containchilds[markbuffer[cliques[L][j]]].push_back(make_pair(i, j));
			}
			each(v, cliques[K]) markbuffer[*v] = -1;

			//compute |NIS(G(K), K)| by Π_(L∈CHILDS(K)){ |NIS(G(L), K∩L)| }
			out_notcontain = 0;
			rep(i, T[K].size()) {
				int L = T[K][i];
				if(L == parent) continue;
				out_notcontain += intersect[i];
			}

			//foreach v ∈ K do compute |IS(G(K), v)|
			// by Π_(L∈CHILDS(K),   v∈L ){ | IS(G(L),    v)| }
			// × Π_(L∈CHILDS(K), ~(v∈L)){ |NIS(G(L), K∩L)| }
			// =  |NIS(G(K), K)|
			// ÷ Π_(L∈CHILDS(K),   v∈L ){ |NIS(G(L), K∩L)| }
			// × Π_(L∈CHILDS(K),   v∈L ){ | IS(G(L),    v)| }
			//{v}もそれに掛ける
			out_contain.resize(cliques[K].size());
			rep(i, cliques[K].size()) {
				out_contain[i] = weight[cliques[K][i]];
				out_contain[i] += out_notcontain;
				each(j, containchilds[i]) {
					out_contain[i] -= intersect[j->first];
					out_contain[i] += contain[j->first][j->second] - weight[cliques[K][i]];
				}
			}
		}
	}

	ChordalWeightedIndependentSet(const Graph &g, const vector<int> &ord, const vector<int> &node, const vector<int> &edge, const vector<Weight> &weight_)
	: weight(weight_), markbuffer(g.size(), -1) {
		cliqueTreeMakeGraph(g, ord, node, edge, T, cliques);	//めんどくさいから定数大きそうな方法でいいか
	}

	Weight compute() {
		if(T.size() == 0) return 0;	//空だとルートノードが取れないので
		Weight notcontain; vector<Weight> contain;
		rec(-1, 0, notcontain, contain);
		Weight r = notcontain;
		rep(i, contain.size()) r = max(r, contain[i]);
		return r;
	}

};


unsigned xor128() {
	static unsigned x = 123456789;
	static unsigned y = 362436069;
	static unsigned z = 521288629;
	static unsigned w = 88675123;
	unsigned t;

	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

unsigned xor128_rand(unsigned x) {
	return xor128() % x;
}

int main() {
#if 0
	//lexBFSとMCSの速度比較
	/*
		※エッジはuniqueを取ってるのでMは正確ではない
		N = 100000, M = 1000000, Times = 10
			LexBFS median: 437ms.
			MCS median: 125ms.
		N = 1000000, M = 1000000, Times = 10
			LexBFS median: 985ms.
			MCS median: 671ms.
		N = 3000, M = 50000000, Times = 10
			LexBFS median: 172ms.
			MCS median: 156ms.

	*/
	const int N = 3000, M = 50000000, Times = 10;
	vi cls[2];
	unsigned used = 0;
	Graph g(N);
	rep(times, Times) {
		rep(i, N) g[i].clear();
		rep(i, M) {
			int v = xor128() % N, u = xor128() % (N-1);
			if(v <= u) u ++;
			g[v].pb(u); g[u].pb(v);
		}
		rep(i, N) {
			sort(all(g[i]));
			g[i].erase(unique(all(g[i])), g[i].end());
			random_shuffle(all(g[i]), xor128_rand);
		}
		rep(i, 2) {
			int kind = i ^ (times & 1);
			vi ord;
			clock_t cl = clock();
			(!kind ? lexicographicBFS : maximumCardinalitySearch)(g, ord);
			cls[kind].pb(clock() - cl);
			cout <<"(" << times << "+1)th " << (!kind ? "LexBFS" : "MCS") << ": " << cls[kind].back() << "ms." << endl;
			rep(i, N) used += ord[i];
		}
	}
	if(used == 0) cout << "";
	cout << "N = " << N << ", " << "M = " << M << ", " << "Times = " << Times << endl;
	rep(kind, 2) {
		sort(all(cls[kind]));
		cout << (!kind ? "LexBFS" : "MCS") << " median: " << cls[kind][Times/2] << "ms." << endl;
	}
#endif
#if 1
	const int N = 6;

//	/*
	//関数の全探索SmallCheck
	rep(edgesmask, 1<<(N*(N-1)/2)) {
		static clock_t cl = -1000000;
	if(clock() - cl >= 500) {
		cl = clock();
		double parcentage = edgesmask * 1. / (1<<(N*(N-1)/2));
		cerr << edgesmask << " / " << (1<<(N*(N-1)/2)) << " (" << parcentage*100 << "%)..." << "\r";
	}
	Graph g(N);
	rep(i, N) rep(j, i) if(edgesmask >> (i * (i-1) / 2 + j) & 1)
		g[j].pb(i), g[i].pb(j);
//	rep(i, N) random_shuffle(all(g[i]));
	/*/
	//手動テスト
	while(1) {
	Graph g(N);
	{
		//claw: 01 02 03
		//butterfly: 01 02 03 04 12 34
		//diamond: 01 02 12 13 23
		//quad(not chordal): 01 03 12 23
		//K_5: 01 02 03 04 12 13 14 23 24 34
		//~(P3 ∪ 2K_1): 01 02 03 12 13 14 23 34
		//01 02 03 04 12 13 14 23
		//ab bc bd be cd ce cg de df dg ef
		//ab ac bc bd be bg cd ce cg de df ef eg
		//(not chordal): ab ae bc cd de
		//(not chordal): 01 03 04 12 14 23 24 34
	string s;
	getline(cin,s);
	stringstream ss(s);
	char a, b;
	while(ss >> a >> b) {
		g[vertexRead(a)].pb(vertexRead(b));
		g[vertexRead(b)].pb(vertexRead(a));
	}
	}
//	*/
//	cout << undirectedGraphVisualize(g) << endl;
	vi ord, in_ord(N);
	rep(i, N) in_ord[i] = i;
	random_shuffle(all(in_ord));
	orderedLexicographicBFS(g, in_ord, ord);
//	maximumCardinalitySearch(g, ord);
//	vi node, edge;
//	cliqueTreeMCS(g, ord, node, edge);
	bool check = isPerfectEliminationOrder(g, ord);
//	rep(i, N) cout << vertexStr(ord[i]) << endl;
	{
	static bool mat[N][N];
	mset(mat, 0);
	rep(i, N) each(j, g[i]) mat[i][*j] = 1;
	/*
	cerr << "http://chart.apis.google.com/chart?cht=gv:dot&chl=digraph{";
	rep(i, N) {
		int v = ord[i];
		rep(j, i) if(mat[v][ord[j]]) {
			cerr << vertexStr(v) << "->" << vertexStr(ord[j]) << ";";
		}
	}
	cerr << "}" << endl;
	*/

#if 0
	//cliqueTree
	Graph ct; vector<vector<int> > cc;
	cliqueTreeMakeGraph(g, ord, node, edge, ct, cc);
	cout << visualizeCliqueTree(ct, cc) << endl;
	//Verfy
	if(check) {
		static bool edgevis[N][N];
		mset(edgevis, 0);
		//各ノードがクリークになっていて、それだけでグラフが十分に表現できること
		bool cliqueok = true;
		rep(i, ct.size()) {
			rep(j, cc[i].size()) rep(k, j) {
				edgevis[cc[i][j]][cc[i][k]] = edgevis[cc[i][k]][cc[i][j]] = 1;
			}
		}
		rep(i, N) rep(j, N) cliqueok &= mat[i][j] == edgevis[i][j];
		//頂点が木上で連結である(サブツリーである)こと
		bool connectok = true;
		static bool containclique[N][N];
		mset(containclique, 0);
		rep(i, ct.size()) each(v, cc[i]) containclique[*v][i] = 1;
		static bool visvertex[N];
		static bool visnode[N][N];
		mset(visvertex, 0);
		mset(visnode, 0);
		static vi stk;
		rep(i, ct.size()) each(v, cc[i]) if(!visvertex[*v]) {
			visvertex[*v] = true;
			stk.clear();
			stk.push_back(i);
			while(!stk.empty()) {
				int x = stk.back(); stk.pop_back();
				if(visnode[*v][x]) continue;
				visnode[*v][x] = 1;
				each(y, ct[x]) stk.push_back(*y);
			}
		}else if(!visnode[*v][i]) {
			connectok = false;
		}
		if(!cliqueok || !connectok) {
			if(!cliqueok) cerr << "Tree node is not clique!!!!!" << endl;
			if(!connectok) cerr << "Vertex is not connected!!!!!" << endl;
			cerr << undirectedGraphVisualize(g) << endl;
			cerr << visualizeCliqueTree(ct, cc) << endl;
		}
	}

#endif

#if 0
	//Independent Set
	if(check) {
		bool isok = true;
		int issize = chordalMaximumIndependenSet(g, ord);

		if(N < 31) {
		static int gmask[N];
		rep(i, N) {
			gmask[i] = 1 << i;
			each(j, g[i]) gmask[i] |= 1 << *j;
		}
		static int bitDP[N+1][1<<N];
		mset(bitDP, -1);
		bitDP[0][0] = 0;
		rep(i, N) {
			rep(j, 1<<N) if(bitDP[i][j] != -1) {
				bitDP[i+1][j] = max(bitDP[i+1][j], bitDP[i][j]);
				if(~j >> i & 1) bitDP[i+1][j | gmask[i]] =
					max(bitDP[i+1][j | gmask[i]], bitDP[i][j]+1);
			}
		}
		int bitDPans = -1;
		rep(i, 1<<N) bitDPans = max(bitDPans, bitDP[N][i]);
		if(issize != bitDPans) {
			isok = false;
			cout << "Wrong answer..." << endl;
		}
		}
	}
#endif

#if 0
	//Weighted Independent Set
	if(check) {
		bool wisok = true;
		vector<int> weight(N);
		rep(i, N) weight[i] = xor128() % 5 - 2;
		int wis = ChordalWeightedIndependentSet(g, ord, node, edge, weight).compute();

		if(N < 31) {
		static int gmask[N];
		rep(i, N) {
			gmask[i] = 1 << i;
			each(j, g[i]) gmask[i] |= 1 << *j;
		}
		static int bitDP[N+1][1<<N];
		mset(bitDP, 0xC0);
		bitDP[0][0] = 0;
		rep(i, N) {
			rep(j, 1<<N) if(bitDP[i][j] != -1) {
				bitDP[i+1][j] = max(bitDP[i+1][j], bitDP[i][j]);
				if(~j >> i & 1) bitDP[i+1][j | gmask[i]] =
					max(bitDP[i+1][j | gmask[i]], bitDP[i][j]+weight[i]);
			}
		}
		int bitDPans = -1;
		rep(i, 1<<N) bitDPans = max(bitDPans, bitDP[N][i]);
		if(wis != bitDPans) {
			wisok = false;
			cout << "Wrong answer..." << endl;
		}
		}
	}
#endif

#if 1
	//lexBFS/MCSとisPEOのVerify
	bool naive = true;
	rep(i, N) {
		int v = ord[i];
		vi vs;
		rep(j, i) if(mat[v][ord[j]]) vs.pb(ord[j]);
		rep(j, vs.size()) rep(k, j) naive &= mat[vs[j]][vs[k]];
	}
	bool morenaive = false;
	vi ord2; rep(i, N) ord2.pb(i);
	do {
		bool b = true;
		rep(i, N) {
			int v = ord2[i];
			vi vs;
			rep(j, i) if(mat[v][ord2[j]]) vs.pb(ord2[j]);
			rep(j, vs.size()) rep(k, j) b &= mat[vs[j]][vs[k]];
		}
		if(b) { morenaive = true; break; }
	}while(next_permutation(all(ord2)));
//	cerr << "PEO linear check: " << (check ? "Chordal" : "Not Chordal") << endl;
//	cerr << "PEO naive check: " << (naive ? "Chordal" : "Not Chordal") << endl;
//	cerr << "PEO MORE naive check: " << (morenaive ? "Chordal" : "Not Chordal") << endl;
//	if(!check) cout << undirectedGraphVisualize(g) << endl;
	if(naive != morenaive) {
		cerr << "Different between naive check and more naive check!!!!!!!!!" << endl;
		cerr << undirectedGraphVisualize(g) << endl;
	}
	if(check != naive) {
		cerr << "Different between naive check and linear check!!!!!!!!!" << endl;
		cerr << undirectedGraphVisualize(g) << endl;
	}
#endif
	}
	}
	cout << "OK." << endl;
#endif

	while(getchar()!='\n');
	return 0;
}
