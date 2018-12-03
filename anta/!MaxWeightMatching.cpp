/*
	注意点・メモ (必ず読むこと)
	・"Max"、最大化である。最小化の場合にはマイナスすること。
	・無向グラフで、片方のエッジだけがあるものを必要とする
	・エッジはユニークでなければならない。
		もし複数の重複したエッジがあるならば(ほとんどの場合そう)
		uniqueEdges関数を通す必要がある。
		これはExampleでは気づきにくいので注意
	・一つのノードだけにいるようなものは、
		適当な他のどことも繋がってないノードと繋ぐことで出来る
*/
typedef int Weight;
struct Edge {
	int src, dst;
	Weight weight;
	Edge(int src_, int dst_, Weight weight_) : src(src_), dst(dst_), weight(weight_) { }
	Edge(){};
};
typedef vector<Edge> Edges;
struct MaxWeightMatching {
	int nEdge, nVertex;
	Weight maxWeight;
	vi endPoint, mate, label, labelEdge,
		inBlossom, blossomParent, blossomBase,
		bestEdge, unusedBlossoms, allowEdge,
		blossomBestEdgesUsed,
		sVerteices;
	vector<Weight> dualVar;
	vector<vi> neighbEnd, blossomChilds, blossomEdges,
		blossomBestEdges;
	Edges edges;
	bool maxCardinality;

	Weight slack(int k) {
		Edge &e = edges[k];
		return dualVar[e.src] + dualVar[e.dst] - 2 * e.weight;
	}
	struct BlossomLeaves {
		int b;
		MaxWeightMatching *mw;
		enum { INITAL, END, NEXT, THIRD } state;
		vi::const_iterator iter, endi;
		BlossomLeaves *bl;
		BlossomLeaves(MaxWeightMatching *mw_, int b_) :
			mw(mw_), b(b_), state(INITAL), bl(NULL) {}
		bool next(int &r) {
			int t;
			switch(state) {
			case INITAL:
				if(b < mw->nVertex) {
					r = b;
					state = END;
					return true;
				}else {
					iter = mw->blossomChilds[b].begin();
					endi = mw->blossomChilds[b].end();
					state = NEXT;
					return next(r);
				}
			case END:
				return false;
			case NEXT:
				if(iter == endi)
					return false;
				t = *iter;
				++ iter;
				if(t < mw->nVertex) {
					r = t;
					return true;
				}else {
					bl = new BlossomLeaves(mw, t);
					state = THIRD;
					return next(r);
				}
			case THIRD:
				if(!bl->next(t)) {
					state = NEXT;
					delete bl; bl = NULL;
					return next(r);
				}else {
					r = t;
					return true;
				}
			}
			return false;
		}
	};
	void assignLabel(int w, int t, int p) {
		int b = inBlossom[w];
		label[w] = label[b] = t;
		labelEdge[w] = labelEdge[b] = p;
		bestEdge[w] = bestEdge[b] = -1;
		if(t == 1) {
			BlossomLeaves bl(this, b);
			for(int r; bl.next(r); ) sVerteices.push_back(r);
		}else if(t == 2) {
			int base = blossomBase[b];
			assignLabel(endPoint[mate[base]], 1, mate[base] ^ 1);
		}
	}
	int scanBlossom(int v, int w) {
		vi path;
		int base = -1;
		while(v != -1) {
			int b = inBlossom[v];
			if(label[b] & 4) {
				base = blossomBase[b];
				break;
			}
			path.push_back(b);
			label[b] = 5;
			if(labelEdge[b] == -1) {
				v = -1;
			}else {
				v = endPoint[labelEdge[b]];
				b = inBlossom[v];
				v = endPoint[labelEdge[b]];
			}
			if(w != -1)
				swap(v, w);
		}
		each(it, path)
			label[*it] = 1;
		return base;
	}
	void addBlossom(int base, int k) {
		Edge &e = edges[k];
		int bb = inBlossom[base],
			bv = inBlossom[e.src],
			bw = inBlossom[e.dst];
		int b = unusedBlossoms.back(); unusedBlossoms.pop_back();
		blossomBase[b] = base;
		blossomParent[b] = -1;
		blossomParent[bb] = b;
		blossomChilds[b].clear();
		blossomEdges[b].clear();
		vi &bChilds = blossomChilds[b];
		vi &bEdges = blossomEdges[b];
		while(bv != bb) {
			blossomParent[bv] = b;
			bChilds.push_back(bv);
			bEdges.push_back(labelEdge[bv]);
			bv = inBlossom[endPoint[labelEdge[bv]]];
		}
		bChilds.push_back(bb);
		reverse(all(bChilds));
		reverse(all(bEdges));
		bEdges.push_back(2*k);
		while(bw != bb) {
			blossomParent[bw] = b;
			bChilds.push_back(bw);
			bEdges.push_back(labelEdge[bw] ^ 1);
			bw = inBlossom[endPoint[labelEdge[bw]]];
		}
		label[b] = 1;
		labelEdge[b] = labelEdge[bb];
		dualVar[b] = 0;
		BlossomLeaves bl(this, b);
		for(int v; bl.next(v); ) {
			if(label[inBlossom[v]] == 2)
				sVerteices.push_back(v);
			inBlossom[v] = b;
		}
		vi bestEdgeTo(2 * nVertex, -1);
		each(it, bChilds) {
			int bv2 = *it;
			vector<vi> nbLists;
			if(!blossomBestEdgesUsed[bv2]) {
				BlossomLeaves bl2(this, bv2);
				for(int v2; bl2.next(v2); ) {
					nbLists.push_back(neighbEnd[v2]);
					vi &vv = nbLists.back();
					each(i, vv)
						*i /= 2;
				}
			}else {
				nbLists.push_back(blossomBestEdges[bv2]);
			}
			each(itt, nbLists) {
				each(ittt, *itt) {
					int k2 = *ittt;
					Edge &e2 = edges[k2];
					int i = e2.src, j = e2.dst;
					if(inBlossom[j] == b) 
						swap(i, j);
					int bj = inBlossom[j];
					if(bj != b && label[bj] == 1 &&
						(bestEdgeTo[bj] == -1 ||
						slack(k2) < slack(bestEdgeTo[bj]))) {
						bestEdgeTo[bj] = k2;
					}
				}
			}
			blossomBestEdgesUsed[bv2] = false;
			blossomBestEdges[bv2].clear();
			bestEdge[bv2] = -1;
		}
		blossomBestEdgesUsed[b] = true;
		blossomBestEdges[b].clear();
		each(it, bestEdgeTo)
			if(*it != -1)
				blossomBestEdges[b].push_back(*it);
		bestEdge[b] = -1;
		each(it, blossomBestEdges[b])
			if(bestEdge[b] == -1 || slack(*it) < slack(bestEdge[b]))
				bestEdge[b] = *it;
	}
	void expandBlossom(int b, bool endStage) {
		int cSize = blossomChilds[b].size();
		each(it, blossomChilds[b]) {
			int s = *it;
			blossomParent[s] = -1;
			if(s < nVertex)
				inBlossom[s] = s;
			else if(endStage && dualVar[s] == 0)
				expandBlossom(s, endStage);
			else {
				BlossomLeaves bl(this, s);
				for(int v; bl.next(v); )
					inBlossom[v] = s;
			}
		}
		if(!endStage && label[b] == 2) {
			int entryChild = inBlossom[endPoint[labelEdge[b] ^ 1]];
			int j = find(all(blossomChilds[b]), entryChild) - blossomChilds[b].begin();
			int jStep, evenOdd;
			if(j & 1)
				jStep = 1, evenOdd = 0;
			else
				jStep = -1, evenOdd = 1;
			int p = labelEdge[b];
			while(j != 0) {
				label[endPoint[p ^ 1]] = 0;
				label[endPoint[blossomEdges[b][j-evenOdd]^evenOdd^1]] = 0;
				assignLabel(endPoint[p ^ 1], 2, p);
				allowEdge[blossomEdges[b][j-evenOdd]/2] = true;
				j = (j + jStep + cSize) % cSize;
				p = blossomEdges[b][j-evenOdd] ^ evenOdd;
				allowEdge[p/2] = true;
				j = (j + jStep + cSize) % cSize;
			}
			int bv = blossomChilds[b][j];
			label[endPoint[p ^ 1]] = label[bv] = 2;
			labelEdge[endPoint[p ^ 1]] = labelEdge[bv] = p;
			bestEdge[bv] = -1;
			j = (j + jStep + cSize) % cSize;
			while(blossomChilds[b][j] != entryChild) {
				bv = blossomChilds[b][j];
				if(label[bv] == 1) {
					j = (j + jStep + cSize) % cSize;
					continue;
				}
				BlossomLeaves bl(this, bv);
				int v;
				while(bl.next(v))
					if(label[v] != 0)
						break;
				if(label[v] != 0) {
					label[v] = 0;
					label[endPoint[mate[blossomBase[bv]]]] = 0;
					assignLabel(v, 2, labelEdge[v]);
				}
				j = (j + jStep + cSize) % cSize;
			}
		}
		label[b] = labelEdge[b] = -1;
		blossomChilds[b].clear();
		blossomEdges[b].clear();
		blossomBestEdgesUsed[b] = false;
		blossomBestEdges[b].clear();
		bestEdge[b] = -1;
		unusedBlossoms.push_back(b);
	}
	void augmentBlossom(int b, int v) {
		int cSize = blossomChilds[b].size();
		int t = v;
		while(blossomParent[t] != b)
			t = blossomParent[t];
		if(t >= nVertex)
			augmentBlossom(t, v);
		int i, j, jStep, evenOdd;
		i = j = find(all(blossomChilds[b]), t)-blossomChilds[b].begin();
		if(i & 1)
			jStep = 1, evenOdd = 0;
		else
			jStep = -1, evenOdd = 1;
		while(j != 0) {
			j = (j + jStep + cSize) % cSize;
			t = blossomChilds[b][j];
			int p = blossomEdges[b][j-evenOdd] ^ evenOdd;
			if(t >= nVertex)
				augmentBlossom(t, endPoint[p]);
			j = (j + jStep + cSize) % cSize;
			t = blossomChilds[b][j];
			if(t >= nVertex)
				augmentBlossom(t, endPoint[p ^ 1]);
			mate[endPoint[p]] = p ^ 1;
			mate[endPoint[p ^ 1]] = p;
		}
		rotate(blossomChilds[b].begin(), blossomChilds[b].begin()+i, blossomChilds[b].end());
		rotate(blossomEdges[b].begin(), blossomEdges[b].begin()+i, blossomEdges[b].end());
		blossomBase[b] = blossomBase[blossomChilds[b][0]];
	}
	void augmentMatching(int k) {
		Edge &e = edges[k];
		int v = e.src, w = e.dst;
		rep(ii, 2) {
			int s = ii==0 ? v : w, p = ii==0 ? 2*k+1 : 2*k;
			while(true) {
				int bs = inBlossom[s];
				if(bs >= nVertex)
					augmentBlossom(bs, s);
				mate[s] = p;
				if(labelEdge[bs] == -1)
					break;
				int bt = inBlossom[endPoint[labelEdge[bs]]];
				s = endPoint[labelEdge[bt]];
				int j = endPoint[labelEdge[bt] ^ 1];
				if(bt >= nVertex)
					augmentBlossom(bt, j);
				mate[j] = labelEdge[bt];
				p = labelEdge[bt] ^ 1;
			}
		}
	}
	void mainLoop() {
		rep(t, nVertex) {
			label.assign(2 * nVertex, 0);
			bestEdge.assign(2 * nVertex, -1);
			fill(blossomBestEdgesUsed.begin()+nVertex, blossomBestEdgesUsed.end(), false);
			allowEdge.assign(nEdge, false);
			sVerteices.clear();
			rep(v, nVertex)
				if(mate[v] == -1 && label[inBlossom[v]] == 0)
					assignLabel(v, 1, -1);
			bool augmented = false;
			while(true) {
				Weight kSlack;
				while(!sVerteices.empty() && !augmented) {
					int v = sVerteices.back(); sVerteices.pop_back();
					each(it, neighbEnd[v]) {
						int p = *it;
						int k = p / 2, w = endPoint[p];
						if(inBlossom[v] == inBlossom[w])
							continue;
						if(!allowEdge[k]) {
							kSlack = slack(k);
							if(kSlack <= 0)
								allowEdge[k] = true;
						}
						if(allowEdge[k]) {
							if(label[inBlossom[w]] == 0) {
								assignLabel(w, 2, p ^ 1);
							}else if(label[inBlossom[w]] == 1) {
								int base = scanBlossom(v, w);
								if(base >= 0) {
									addBlossom(base, k);
								}else {
									augmentMatching(k);
									augmented = true;
									break;
								}
							}else if(label[w] == 0) {
								label[w] = 2;
								labelEdge[w] = p ^ 1;
							}
						}else if(label[inBlossom[w]] == 1) {
							int b = inBlossom[v];
							if(bestEdge[b] == -1 || kSlack < slack(bestEdge[b]))
								bestEdge[b] = k;
						}else if(label[w] == 0) {
							if(bestEdge[w] == -1 || kSlack < slack(bestEdge[w]))
								bestEdge[w] = k;
						}
					}
				}
				if(augmented)
					break;
				int deltaType = -1;
				Weight delta;
				int deltaEdge, deltaBlossom;
				if(!maxCardinality) {
					deltaType = 1;
					delta = *min_element(dualVar.begin(), dualVar.begin()+nVertex);
				}
				rep(v, nVertex) {
					if(label[inBlossom[v]] == 0 && bestEdge[v] != -1) {
						Weight d = slack(bestEdge[v]);
						if(deltaType == -1 || d < delta) {
							delta = d;
							deltaType = 2;
							deltaEdge = bestEdge[v];
						}
					}
				}
				rep(b, 2 * nVertex) {
					if(blossomParent[b] == -1 && label[b] == 1 && bestEdge[b] != -1) {
						kSlack = slack(bestEdge[b]);
						Weight d = kSlack / 2;
						if(deltaType == -1 || d < delta) {
							delta = d;
							deltaType = 3;
							deltaEdge = bestEdge[b];
						}
					}
				}
				reu(b, nVertex, 2 * nVertex) {
					if(blossomBase[b] >= 0 && blossomParent[b] == -1 &&
						label[b] == 2 &&
						(deltaType == -1 || dualVar[b] < delta)) {
						delta = dualVar[b];
						deltaType = 4;
						deltaBlossom = b;
					}
				}
				if(deltaType == -1) {
					deltaType = 1;
					delta = max((Weight)0, *min_element(dualVar.begin(), dualVar.begin()+nVertex));
				}
				rep(v, nVertex) {
					if(label[inBlossom[v]] == 1) {
						dualVar[v] -= delta;
					}else if(label[inBlossom[v]] == 2){
						dualVar[v] += delta;
					}
				}
				reu(b, nVertex, 2*nVertex) {
					if(blossomBase[b] >= 0 && blossomParent[b] == -1) {
						if(label[b] == 1) {
							dualVar[b] += delta;
						}else if(label[b] == 2) {
							dualVar[b] -= delta;
						}
					}
				}
				if(deltaType == 1) {
					break;
				}else if(deltaType == 2) {
					allowEdge[deltaEdge] = true;
					Edge &e = edges[deltaEdge];
					int i = e.src, j = e.dst;
					if(label[inBlossom[i]] == 0)
						swap(i, j);
					sVerteices.push_back(i);
				}else if(deltaType == 3) {
					allowEdge[deltaEdge] = true;
					Edge &e = edges[deltaEdge];
					int i = e.src;
					sVerteices.push_back(i);
				}else if(deltaType == 4) {
					expandBlossom(deltaBlossom, false);
				}
			}
			if(!augmented)
				break;
			reu(b, nVertex, 2*nVertex)
				if(blossomParent[b] == -1 && blossomBase[b] >= 0 &&
					label[b] == 1 && dualVar[b] == 0)
					expandBlossom(b, true);
		}
	}
	vi matching(const Edges& edges_, bool maxCardinality_ = false) {
		edges = edges_; maxCardinality = maxCardinality_;
		nEdge = edges.size();
		nVertex = 0;
		each(i, edges) {
			if(i->src >= nVertex) nVertex = i->src + 1;
			if(i->dst >= nVertex) nVertex = i->dst + 1;
		}
		maxWeight = 0;
		endPoint.clear();
		neighbEnd.assign(nVertex, vi());
		rep(i, nEdge) {
			Edge &e = edges[i];
			maxWeight = max(maxWeight, e.weight);
			endPoint.push_back(e.src);
			endPoint.push_back(e.dst);
			neighbEnd[e.src].push_back(2*i+1);
			neighbEnd[e.dst].push_back(2*i);
		}
		mate.assign(nVertex, -1);
		label.assign(2 * nVertex, 0);
		labelEdge.assign(2 * nVertex, -1);
		inBlossom.clear();
		blossomBase.clear();
		blossomParent.assign(2 * nVertex, -1);
		blossomChilds.assign(2 * nVertex, vi());
		blossomEdges.assign(2 * nVertex, vi());
		bestEdge.assign(2 * nVertex, -1);
		blossomBestEdgesUsed.assign(2 * nVertex, false);
		blossomBestEdges.assign(2 * nVertex, vi());
		unusedBlossoms.clear();
		dualVar.clear();
		rep(i, nVertex)
			inBlossom.push_back(i), 
			blossomBase.push_back(i),
			unusedBlossoms.push_back(nVertex + i),
			dualVar.push_back(maxWeight);
		rep(i, nVertex)
			blossomBase.push_back(-1),
			dualVar.push_back(0);
		allowEdge.assign(nEdge, 0);
		sVerteices = vi();
		mainLoop();
		rep(v, nVertex)
			if(mate[v] >= 0)
				mate[v] = endPoint[mate[v]];
		return mate;
	}
	Weight getWeight(const Edges& e, bool maxCardinality_ = false) {
		vector<int> v = matching(e, maxCardinality_);
		Weight w = 0;
		rep(i, v.size()) if(v[i] != -1)
			each(j, e) if(i == j->src && j->dst == v[i]) w += j->weight;
		return w;
	}
};

Edges uniqueEdges(Edges e) {
	map<pii,Weight> m; Edges r;
	each(i, e) {
		if(i->src == i->dst) continue;	//！注意
		pii p = i->src < i->dst ? mp(i->src, i->dst) : mp(i->dst, i->src);
		Weight t = m.count(p) ? max(m[p], i->weight) : i->weight;
		m[p] = t;
	}
	each(i, m) {
		r.pb(Edge(i->first.first, i->first.second, i->second));
	}
	return r;
}