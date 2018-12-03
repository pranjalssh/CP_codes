//新ver.
struct CentroidPathDecomposition {
	vector<int> colors, positions;	//Vertex -> Color, Vertex -> Offset
	vector<int> lengths, parents, branches;	//Color -> Int, Color -> Color, Color -> Offset
	vector<int> parentnodes, depths;	//Vertex -> Vertex, Vertex -> Int
	//vector<FenwickTree>とかを避けて1次元にしたい時に使う
	vector<int> sortednodes, offsets;	//Index -> Vertex, Color -> Index

	//両方の辺があってもいいし、親から子への辺だけでもよい
	void build(const vector<vi> &g, int root) {
		int n = g.size();

		colors.assign(n, -1); positions.assign(n, -1);
		lengths.clear(); parents.clear(); branches.clear();
		parentnodes.assign(n, -1); depths.assign(n, -1);

		vector<int> subtreesizes;
		measure(g, root, subtreesizes);

		struct State {
			int i, len, parent;
			State() { }
			State(int i_, int l, int p): i(i_), len(l), parent(p) { }
		};
		depths[root] = 0;
		vector<State> s;
		s.push_back(State(root, 0, -1));
		while(!s.empty()) {
			State t = s.back(); s.pop_back();
			int i = t.i, len = t.len;
			int color = lengths.size();
			if(t.parent != -2) {
				assert(parents.size() == color);
				parents.push_back(t.parent);
				branches.push_back(len);
				len = 0;
			}
			colors[i] = color;
			positions[i] = len;

			int maxsize = -1, maxj = -1;
			each(j, g[i]) if(colors[*j] == -1) {
				if(maxsize < subtreesizes[*j]) {
					maxsize = subtreesizes[*j];
					maxj = *j;
				}
				parentnodes[*j] = i;
				depths[*j] = depths[i] + 1;
			}
			if(maxj == -1) {
				lengths.push_back(len + 1);
			}else {
				each(j, g[i]) if(colors[*j] == -1 && *j != maxj)
					s.push_back(State(*j, len, color));
				s.push_back(State(maxj, len + 1, -2));
			}
		}

		sortNodes();
	}

	void sortNodes() {
		int n = colors.size(), m = lengths.size();
		sortednodes.resize(n, -1);
		offsets.resize(m + 1);
		rep(i, m) offsets[i+1] = offsets[i] + lengths[i];
		rep(i, n) sortednodes[offsets[colors[i]] + positions[i]] = i;
	}

	void get(int v, int &c, int &p) const {
		c = colors[v]; p = positions[v];
	}
	bool go_up(int &c, int &p) const {
		p = branches[c]; c = parents[c];
		return c != -1;
	}

	inline const int *nodesBegin(int c) const { return &sortednodes[0] + offsets[c]; }
	inline const int *nodesEnd(int c) const { return &sortednodes[0] + offsets[c+1]; }

private:
	void measure(const vector<vi> &g, int root, vector<int> &out_subtreesizes) const {
		out_subtreesizes.assign(g.size(), -1);
		vector<int> s;
		s.push_back(root);
		while(!s.empty()) {
			int i = s.back(); s.pop_back();
			if(out_subtreesizes[i] == -2) {
				int s = 1;
				each(j, g[i]) if(out_subtreesizes[*j] != -2)
					s += out_subtreesizes[*j];
				out_subtreesizes[i] = s;
			}else {
				s.push_back(i);
				each(j, g[i]) if(out_subtreesizes[*j] == -1)
					s.push_back(*j);
				out_subtreesizes[i] = -2;
			}
		}
	}
};

//(使用例)
//深さがdであってxの祖先であるノードを取得する
int level_ancestor(const CentroidPathDecomposition &cpd, int x, int d) {
	int c, p;
	const int *head;
	if(d > cpd.depths[x]) return -1;
	cpd.get(x, c, p);
	while(d < cpd.depths[*(head = cpd.nodesBegin(c))])
		cpd.go_up(c, p);
	return head[d - cpd.depths[*head]];
}
int get_ancestor_child(const CentroidPathDecomposition &cpd, int x, int y) {
	return level_ancestor(cpd, y, cpd.depths[x] + 1);
}

int lowest_common_ancestor(const CentroidPathDecomposition &cpd, int x, int y) {
	int cx, px, cy, py;
	cpd.get(x, cx, px);
	cpd.get(y, cy, py);
	while(cx != cy) {
		if(cpd.depths[*cpd.nodesBegin(cx)] < cpd.depths[*cpd.nodesBegin(cy)])
			cpd.go_up(cy, py);
		else
			cpd.go_up(cx, px);
	}
	return cpd.nodesBegin(cx)[min(px, py)];
}


//デバッグ用にgraphvizする。#VisualizeColors.cppが必要
	void debug_out(ostream &o) const {
		o << "digraph {" << endl;
		int n = colors.size(), m = lengths.size();
		vector<string> nodenames(n);
		rep(i, n) {
			stringstream ss;
			ss << "\"" << i << "(" << colors[i] << ")" << "\"";
			nodenames[i] = ss.str();
		}
		vector<string> drawcolors;
		get_random_colors_str(m, drawcolors);
		
		rep(i, n)
			o << nodenames[i] << " [style=filled,fillcolor=" << drawcolors[colors[i]] << "];" << endl;
		rep(i, n) {
			int j = parentnodes[i];
			if(j == -1) continue;
			o << nodenames[j] << " -> " << nodenames[i] << " [";
			if(positions[i] == 0)
				o << "style=dotted";
			else
				o << "color=" << drawcolors[colors[i]];
			o << "];" << endl;
		}
		o << "}" << endl;
	}

//#####################################################################
//旧ver.
//edgesにグラフを入れておく。スタックオーバーフローに注意！
//親ノード・深さなど必ずしも必要ないものも利便性のため一緒に計算する。
//パスをたどるにはcpd_parentとcpd_brachを使う。下にサンプルがある

vector<int> edges[100000];
 
int cpd_size[100000];			//ノード -> 子木のサイズ-1
int cpd_parentnode[100000];		//ノード -> 親ノード (rootの場合-1)
int cpd_depth[100000];			//ノード -> 深さ(rootが0)
int cpd_colors;					//パス数
int cpd_color[100000];			//ノード -> パス
int cpd_position[100000];		//ノード -> パス中の位置(上から、0-origin)
vector<int> cpd_nodes[100000];	//パス -> ノードs
int cpd_length[100000];			//パス -> 長さ
int cpd_parent[100000];			//パス -> 親パス
int cpd_branch[100000];			//パス -> 親パス中でどの位置のノードからこのパスへ分岐しているか
 
int cpd_measure(int i, int parent, int dep) {
	cpd_parentnode[i] = parent;
	cpd_depth[i] = dep;
	int s = 0;
	each(j, edges[i]) if(*j != parent) {
		s += cpd_measure(*j, i, dep+1);
	}
	cpd_size[i] = s;
	return s + 1;
}
 
void cpd_getcolor(int i, int parent, int length) {
	int currentcolor = cpd_colors;
	cpd_color[i] = currentcolor;
	cpd_position[i] = length;
	cpd_nodes[currentcolor].push_back(i);
	int maxsize = -INF, maxj = -1;
	each(j, edges[i]) if(*j != parent) {
		if(maxsize < cpd_size[*j]) {
			maxsize = cpd_size[*j];
			maxj = *j;
		}
	}
	if(maxj == -1) {
		cpd_length[currentcolor] = length + 1;
		cpd_colors ++;
	}else {
		cpd_getcolor(maxj, i, length + 1);
		each(j, edges[i]) if(*j != parent && *j != maxj) {
			cpd_parent[cpd_colors] = currentcolor;
			cpd_branch[cpd_colors] = length;
			cpd_nodes[cpd_colors].clear();
			cpd_getcolor(*j, i, 0);
		}
	}
}
 
void cpd_init(int root) {
	cpd_nodes[0].clear();
	cpd_measure(root, -1, 0);
	cpd_colors = 0;
	cpd_parent[0] = -1;
	cpd_branch[0] = 0;
	cpd_getcolor(root, -1, 0);
}


//<http://codeforces.com/contest/372/submission/5433520>
//path_sumが区間足し込みのできるsegment tree。

void pathaddval(int v, int x) {
	int c = cpd_color[v], p = cpd_position[v];
	while(c != -1) {
		path_sum[c].addToRange(0, p+1, x);
		p = cpd_branch[c];
		c = cpd_parent[c];
	}
}
int pathgetval(int v) {
	int c = cpd_color[v], p = cpd_position[v];
	return path_sum[c].get(p);
}
//祖先のうち値が0でない一番下までの距離
int pathgetdist(int v) {
	int c = cpd_color[v], p = cpd_position[v];
	int res = 0;
	while(c != -1) {
		int i = path_sum[c].rightmost_le(p + 1, 1) - 1;
		if(i != -1) {
			res += p - i;
			break;
		}
		res += p+1;
		p = cpd_branch[c];
		c = cpd_parent[c];
	}
	return res;
}
