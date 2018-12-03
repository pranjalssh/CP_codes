#pragma once
#include <cassert>
#include <vector>
#include "../Util/StaticGraph_Simple.cpp"
#include "PlusMinusRMQ.cpp"

class EulerTourLCA {
public:
	typedef Graph::Vertex Vertex;
private:
	typedef PlusMinusRMQ<ValAndPos<int,int>, ValAndPos<char,char>, 10> RMQ;
	RMQ rmq;
	std::vector<Vertex> vertexSequence;
	std::vector<int> positionTableL, positionTableR;

public:
	void build(const Graph &g, Vertex root) {
		assert(g.n == g.m+1);
		int seqSize = g.m + g.n;
		std::vector<std::pair<Vertex, const Graph::To *> > s;
		s.reserve(g.n);
		std::vector<RMQ::BitSet> rmqDepthSeq(seqSize / RMQ::BitSetSize + 2, 0);
		vertexSequence.resize(seqSize);
		positionTableL.resize(g.n);
		positionTableR.resize(g.n);

		int currentDepth = 0, currentIndex = 0;
		s.push_back(std::make_pair(root, g.edgesBegin(root)));
		while(!s.empty()) {
			Vertex v = s.back().first;
			const Graph::To *it = s.back().second;
			s.pop_back();

			if(it == g.edgesBegin(v))
				positionTableL[v] = currentIndex;
			if(it == g.edgesEnd(v)) {
				positionTableR[v] = currentIndex;
				rmqDepthSeq[currentIndex / RMQ::BitSetSize] |= RMQ::BitSet(1) << (currentIndex % RMQ::BitSetSize);
				vertexSequence[currentIndex ++] = v;
			}else {
				vertexSequence[currentIndex ++] = v;
				//Œã‚©‚çpush‚µ‚½‚Ù‚¤‚ªæ‚Épop‚³‚ê‚é‚±‚Æ‚É’ˆÓ
				s.push_back(std::make_pair(v, it+1));
				s.push_back(std::make_pair(it->to, g.edgesBegin(it->to)));
			}
		}
		s.swap(std::vector<std::pair<Vertex, const Graph::To *> >());	//shrink-to-fit
		rmq.build(&rmqDepthSeq[0], seqSize, ValAndPos<int,int>(0, 0));
	}

	Vertex query(Vertex v, Vertex u) const {
		int l, r;
		if(positionTableL[v] < positionTableL[u])
			l = positionTableL[v], r = positionTableR[u];
		else
			l = positionTableL[u], r = positionTableR[v];
		ValAndPos<int,int> x = rmq.query(l, r);
		return vertexSequence[x.pos];
	}
};
