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

typedef vector<vector<int> > Graph;

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

//Linear
//(v, u)のuをordによって並び替える
void backetSortGraph(const Graph &in_g, const vector<int> &ord, Graph &out_g) {
	int n = in_g.size();

	vector<int> ord_inv(n);	//ord: N -> V, ord_inv: V -> N
	rep(i, n) ord_inv[ord[i]] = i;

	vector<vector<int> > backets(n);
	
	rep(i, n) for(vector<int>::const_iterator j = in_g[i].begin(); j != in_g[i].end(); ++ j)
		backets[ord_inv[*j]].push_back(i);
	
	out_g.assign(n, vector<int>());
	rep(i, n) for(vector<int>::const_iterator j = backets[i].begin(); j != backets[i].end(); ++ j)
		out_g[*j].push_back(ord[i]);
}

//ordによって初期順序が指定される
template<bool complement>
void orderedLexicographicBFS(const Graph &g_original, const vector<int> &ord, vector<int> &out_ord) {
	Graph g;
	backetSortGraph(g_original, ord, g);

	int n = g.size();
	out_ord.clear(); out_ord.reserve(n);

	vector<int>
		prevvertex(n, -2), nextvertex(n, -2), headvertex(n, -2),
		prevbacket(n, -2), nextbacket(n, -2), backetid(n, 0),
		backetlabelinit(n, -2), backetlabellast(n, -2),
		freebacketstack(n, -2);
	int headbacket = 0, freebacketsp = n-1;

	rep(i, n) {
		prevvertex[ord[i]] = i   == 0 ? -1 : ord[i-1];
		nextvertex[ord[i]] = i+1 == n ? -1 : ord[i+1];
	}
	headvertex[0] = ord[0];

	prevbacket[0] = -1, nextbacket[0] = -1;
	backetlabelinit[0] = -1, backetlabellast[0] = -1;
	rep(i, n-1) freebacketstack[i] = (n-1) - i;
	
	rep(t, n) {

		int v = headvertex[headbacket];
		out_ord.push_back(v);
		if(nextvertex[v] == -1) {
			//delete headbacket
			freebacketstack[freebacketsp ++] = headbacket;
			if((headbacket = nextbacket[headbacket]) != -1) prevbacket[headbacket] = -1;
		}
		else prevvertex[headvertex[headbacket] = nextvertex[v]] = -1;
		backetid[v] = -1;

//		cout << vertexStr(v) << " | " << backetsStr(headbacket, nextbacket, headvertex, nextvertex) << endl;;

		//辺がソートされていればそのままの順序になるように、後ろからやってる
		for(vector<int>::const_reverse_iterator u = g[v].rbegin(); u != g[v].rend(); ++ u) {
			int ubacket = backetid[*u];
			if(ubacket != -1) {
				int uplacebacket = !complement ? prevbacket[ubacket] : nextbacket[ubacket];
				if(uplacebacket == -1
				|| backetlabelinit[uplacebacket] != ubacket
				|| backetlabellast[uplacebacket] != v) {
					//create backet at this place
					int newbacket = freebacketstack[-- freebacketsp];

					if(!complement) {	//普通は新しいのは前に置く
						nextbacket[newbacket] = ubacket;
						prevbacket[newbacket] = uplacebacket;
						if(uplacebacket == -1) headbacket = newbacket;
						else nextbacket[uplacebacket] = newbacket;
						prevbacket[ubacket] = newbacket;
					}else {	//complementなら新しいのは後に置く。uplacebacket = nextbacket[ubacket] であることに注意
						nextbacket[newbacket] = uplacebacket;
						prevbacket[newbacket] = ubacket;
						if(uplacebacket != -1) prevbacket[uplacebacket] = newbacket;
						nextbacket[ubacket] = newbacket;
					}

					headvertex[newbacket] = -1;

					backetlabelinit[newbacket] = ubacket;
					backetlabellast[newbacket] = v;

					uplacebacket = newbacket;
				}
				//remove u from ubacket
				if(prevvertex[*u] == -1 && nextvertex[*u] == -1) {
					//delete ubacket
					if(prevbacket[ubacket] != -1)
						nextbacket[prevbacket[ubacket]] = nextbacket[ubacket];
					else
						headbacket = nextbacket[ubacket];
					if(nextbacket[ubacket] != -1) prevbacket[nextbacket[ubacket]] = prevbacket[ubacket];
					freebacketstack[freebacketsp ++] = ubacket;
				}else {
					if(prevvertex[*u] != -1) nextvertex[prevvertex[*u]] = nextvertex[*u];
					else headvertex[ubacket] = nextvertex[*u];
					if(nextvertex[*u] != -1) prevvertex[nextvertex[*u]] = prevvertex[*u];
				}
				//insert u into uplacebacket
				prevvertex[*u] = -1;
				nextvertex[*u] = headvertex[uplacebacket];
				if(headvertex[uplacebacket] != -1) prevvertex[headvertex[uplacebacket]] = *u;
				headvertex[uplacebacket] = *u;
				backetid[*u] = uplacebacket;

//				cout << "sepalate " << vertexStr(*u) << ": " << backetsStr(headbacket, nextbacket, headvertex, nextvertex) << endl;;
			}
		}
	}
}

//G is cograph <-> Gとその補グラフがNSPを満たす
//NSPを満たす <-> ∀ v i, local(v, i) ⊃ local(v, i+1)
//それはlocalを適当に(普通に？)ソートして適当にやればよい
//???? //local(i, v) = { w | u∈S(v, i), w∈N(u), w∈S(v), σ(w) < σ(v_i) }
//???? //S(v)の中で、S(v, i)のどれかと接してるやつ

int main() {
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
	rep(i, N) random_shuffle(all(g[i]));
	/*/
	//手動テスト
	while(1) {
	Graph g(N);
	{
		//claw: 01 02 03
		//butterfly: 01 02 03 04 12 34
		//diamond: 01 02 12 13 23
		//quad: 01 03 12 23
		//K_5: 01 02 03 04 12 13 14 23 24 34
		//~(P3 ∪ 2K_1): 01 02 03 12 13 14 23 34
		//01 02 03 04 12 13 14 23
		//ab bc bd be cd ce cg de df dg ef
		//ab ac bc bd be bg cd ce cg de df ef eg
		//ab ae bc cd de
		//01 03 04 12 14 23 24 34
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
	
	vi initord(N), ord, ord2; Graph complementg(N);
	rep(i, N) {
		set<int> s(all(g[i]));
		rep(j, N) if(i != j && !s.count(j))
			complementg[i].pb(j);
	}
	rep(i, N) initord[i] = N - i - 1;
	random_shuffle(all(initord));
	orderedLexicographicBFS<false>(g, initord, ord);
	orderedLexicographicBFS<true>(complementg, initord, ord2);
	//complementのcheck
	if(ord != ord2) {
		cerr << undirectedGraphVisualize(g) << endl;
		cerr << "initord: "; rep(i, N) cerr << initord[i] << ", "; cerr << endl;
		cerr << "Error: ord != ord2" << endl;
	}
	}
}
