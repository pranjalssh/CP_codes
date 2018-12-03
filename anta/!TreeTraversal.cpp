//gの辺が下向きだけでも部分的に動くかもしれないが、t_parentが設定されないので注意！

//parentとordだけ
vector<int> t_parent;
vi t_ord;

void tree_getorder(const vector<vi> &g, int root) {
	int n = g.size();
	t_parent.assign(n, -1);

	vector<int> stk; stk.push_back(root);
	while(!stk.empty()) {
		int i = stk.back(); stk.pop_back();
		t_ord.push_back(i);
		for(int j = (int)g[i].size()-1; j >= 0; j --) {
			int c = g[i][j];
			if(t_parent[c] == -1 && c != root)
				stk.push_back(c);
			else
				t_parent[i] = c;
		}
	}
}

//Euler tour
vector<int> t_parent;
vi t_ord;
vector<int> t_left, t_right;
 
void tree_eulertour(const vector<vi> &g, int root) {
	int n = g.size();
	t_parent.assign(n, -1);
	t_left.assign(n, -1);
	t_right.assign(n, -1);
 
	vector<int> stk; stk.push_back(root);
	while(!stk.empty()) {
		int i = stk.back(); stk.pop_back();
		if(i < 0) {
			i = -i - 1;
			t_right[i] = t_ord.size();
			continue;
		}
		t_left[i] = t_ord.size();
		t_ord.push_back(i);
		stk.push_back(-(i+1));
		for(int j = (int)g[i].size()-1; j >= 0; j --) {
			int c = g[i][j];
			if(t_parent[c] == -1 && c != root)
				stk.push_back(c);
			else
				t_parent[i] = c;
		}
	}
}

//符号付きEuler Tour
//p→v (pはvの祖先) のパスが[t_left[p],t_left[v]]に対応する
//ただし、t_sign[i]がtrueであるiが存在する頂点は無視する。このときt_sign[i]がtrueである頂点はt_sign[i]がfalseとしても出ているので、引き算ができる。
//列の長さは2N
vector<int> t_parent;
vi t_seq; vector<bool> t_sign;
vector<int> t_left, t_right;
 
void tree_signedeulertour(const vector<vi> &g, int root) {
	int n = g.size();
	t_parent.assign(n, -1);
	t_left.assign(n, -1);
	t_right.assign(n, -1);
	t_seq.assign(n * 2, -1);
	t_sign.assign(n * 2, false);
	int pos = 0;
 
	vector<int> stk; stk.push_back(root);
	while(!stk.empty()) {
		int i = stk.back(); stk.pop_back();
		if(i < 0) {
			i = -i - 1;
			t_seq[pos] = i;
			t_sign[pos] = true;
			pos ++;
			t_right[i] = pos;
			continue;
		}
		t_left[i] = pos;
		t_seq[pos] = i;
		t_sign[pos] = false;
		pos ++;

		stk.push_back(-(i+1));
		for(int j = (int)g[i].size()-1; j >= 0; j --) {
			int c = g[i][j];
			if(t_parent[c] == -1 && c != root)
				stk.push_back(c);
			else
				t_parent[i] = c;
		}
	}
}


//木DPとか用に木を2分木と考えてインデックスをつける。
//ノードの終了条件となるダミーノードにもインデックスを与えている。
//状態数はたかだかN*2

vector<vi> g;
vector<int> parent;
vector<vi> stateidx;
vi t_ord;
int nstates = 0;

void tree_getstateindices(int root) {
	int n = g.size();
	parent.assign(n, -1);
	stateidx.assign(n, vi());
	t_ord.clear();
	nstates = 0;

	vector<int> stk; stk.push_back(root);
	while(!stk.empty()) {
		int i = stk.back(); stk.pop_back();
		t_ord.push_back(i);
		stateidx[i].assign(g[i].size() + 1, -1);
		rep(j, g[i].size()) {
			int c = g[i][j];
			if(parent[c] == -1 && c != root) {
				stk.push_back(c);
				stateidx[i][j] = nstates ++;
			}else {
				stateidx[i][j] = -1;
				parent[i] = c;
			}
		}
		stateidx[i][g[i].size()] = nstates ++;
	}
}

inline int getstateindex(int i, int j) {
	int s = stateidx[i][j];
	return s != -1 ? s : stateidx[i][j+1];
}

//これを使った小さな関数たち

int subtreesize[5003*2];
int goal;
bool togoal[5003*2];

int measure_tree(int i) {
	int r = 1;
	for(int j = g[i].size(); j >= 0; j --) if(stateidx[i][j] != -1) {
		if(j != g[i].size()) r += measure_tree(g[i][j]);
		subtreesize[stateidx[i][j]] = r;
	}
	return r;
}

//rootからゴールへ向かう辺かどうかを取得。一般の場合はLCAとかで出来ると思う
bool get_togoal(int i) {
	bool b = i == goal;
	rep(j, g[i].size()) if(stateidx[i][j] != -1) {
		bool c = get_togoal(g[i][j]);
		togoal[stateidx[i][j]] = c;
		b |= c;
	}
	return b;
}
