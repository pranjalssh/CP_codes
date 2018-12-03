#pragma once
#include <cassert>
#include <vector>
#include "../Util/StaticGraph_Simple.cpp"

//Schieber-Vishkin algorithm
//参考: http://homepage.usask.ca/~ctl271/810/approximate_matching.shtml
//パスに分解することを考える。
//分解が「(I(･) = 子木の中で(h(･) = in-order indexで作られるsideways-heap上の高さ)が最大の頂点)でラベル付けする」
//この分解は非常に良い性質を多数持っている。その性質をビット演算によって引きだす。
//まず、「h(I(･))は上(根方向)に行くにつれて(広義)単調増加」
//	これは「最大の」という定義からわかる。
//そして、「I(･)は一意である」
//	in-order indexでは子木は区間によって表すことができる。
//	sideways-heapでの高さというのは、一番右(符号方向から遠い)の1のビットの位置。
//	もし重複するなら、それらは連続していて、そのうちどちらかはさらに高くなる、という観察で証明できる。
//それから、高さというのは0～floor(log n)。
//つまり、ある頂点から根へのパスを考えた時、
//「たかだかfloor(log n)+1種類」のh(I(･))が「単調増加」で出現する。
//ここで、(A(･) = そのパスの祖先(自ら含む)のh(I(･))の集合をビットセットで表す)ことを考える。
//	すると、その集合のANDや一番左の立ってるビットなどによって情報を得られる。

//Tested
class SchieberVishkinLCA {
public:
	typedef Graph::Vertex Vertex;
	typedef unsigned Word;
private:

	static inline Word lowestOneBit(Word v) {
		return v & (~v+1);
	}
	static inline Word highestOneBitMask(Word v) {
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		return v >> 1;
	}

	std::vector<Word> indices;			//Vertex -> index
	std::vector<Word> maxHIndices;		//Vertex -> index
	std::vector<Word> ancestorHeights;	//Vertex -> Word
	std::vector<Vertex> pathParents;	//index-1 -> Vertex
public:
	//gは親→子の枝のある根付き木
	void build(const Graph &g, Vertex root) {
		assert(g.m == g.n - 1);

		ancestorHeights.resize(g.n);
		std::vector<Vertex> parents(g.n);
		maxHIndices.resize(g.n);
		std::vector<Vertex> vertices; vertices.reserve(g.n);
		indices.resize(g.n);

		//euler tour
		Word currentIndex = 1;
		parents[root] = root;	//利便さのために
		vertices.push_back(root);
		while(!vertices.empty()) {
			Vertex v = vertices.back(); vertices.pop_back();
			indices[v] = currentIndex ++;
			for(const Graph::To *it = g.edgesBegin(v); it != g.edgesEnd(v); ++ it) {
				parents[it->to] = v;
				vertices.push_back(it->to);
			}
		}

		//BFS (トポロジカル順序を求めるために)
		int head = 0, tail = 1;
		vertices.resize(g.n); vertices[0] = root;
		while(head != tail) {
			Vertex v = vertices[head ++];
			for(const Graph::To *it = g.edgesBegin(v); it != g.edgesEnd(v); ++ it)
				vertices[tail ++] = it->to;
		}

		//深い方から
		for(std::vector<Vertex>::const_iterator it = vertices.begin(); it != vertices.end(); ++ it)
			maxHIndices[*it] = indices[*it];
		for(std::vector<Vertex>::const_reverse_iterator it = vertices.rbegin(); it != vertices.rend(); ++ it) {
			Vertex v = *it, parent = parents[v];
			if(lowestOneBit(maxHIndices[parent]) < lowestOneBit(maxHIndices[v]))
				maxHIndices[parent] = maxHIndices[v];
		}

		//Aを求める
		ancestorHeights[root] = 0;
		for(std::vector<Vertex>::const_iterator it = vertices.begin(); it != vertices.end(); ++ it) {
			Vertex v = *it;
			ancestorHeights[v] = ancestorHeights[parents[v]] | lowestOneBit(maxHIndices[v]);
		}

		pathParents.swap(parents);	//メモリをけちる
		pathParents[indices[root]-1] = root;
		for(std::vector<Vertex>::const_iterator it = vertices.begin(); it != vertices.end(); ++ it) {
			Vertex v = *it;
			for(const Graph::To *jt = g.edgesBegin(v); jt != g.edgesEnd(v); ++ jt) {
				if(maxHIndices[v] != maxHIndices[jt->to])
					pathParents[indices[jt->to]-1] = v;
				else
					pathParents[indices[jt->to]-1] = pathParents[indices[v]-1];
			}
		}
	}

	Vertex query(Vertex v, Vertex u) const {
		//binary tree上でのLCAの高さを求める
		Word Iv = maxHIndices[v], Iu = maxHIndices[u];
		Word hIv = lowestOneBit(Iv), hIu = lowestOneBit(Iu);
		Word hbMask = highestOneBitMask((Iv ^ Iu) | hIv | hIu);
		Word j = lowestOneBit(ancestorHeights[v] & ancestorHeights[u] & ~hbMask);
		//j = hI(lca(v,u)) となる (ここで、hI(x) = 2^(complete binary tree上でのI(x)の高さ), I(x) = maxHIndices[x])
		//(hI(lca(v,u)) = j)はhI(v)かhI(u)かその他の最大値。そして一意であることを考えると…
		Vertex x, y;
		if(j == hIv) x = v;
		else {			//lcaはvのパス上には無い
			Word kMask = highestOneBitMask(ancestorHeights[v] & (j-1));	//vの祖先で、jよりは低いけどその中で一番上にあるパス
			x = pathParents[(indices[v] & ~kMask | (kMask+1))-1];	//indices[v]のkの高さの祖先のパスの親
		}
		if(j == hIu) y = u;
		else {			//lcaはuのパス上には無い
			Word kMask = highestOneBitMask(ancestorHeights[u] & (j-1));	//uの祖先で、jよりは低いけどその中で一番上にあるパス
			y = pathParents[(indices[u] & ~kMask | (kMask+1))-1];	//indices[u]のkの高さの祖先のパスの親
		}
		return indices[x] < indices[y] ? x : y;	//in-orderなので、インデックスが低い方が祖先なはず
	}
};
