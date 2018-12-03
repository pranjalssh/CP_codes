//！multigraphに対しては上手く動かないので注意！

void compute_lowlinks_dfs(int i, int parent, int &order, const vector<vector<int> > &g, vector<int> &ord, vector<int> &low) {
	low[i] = ord[i] = order ++;
	each(j, g[i]) {
		if(ord[*j] == -1) {
			compute_lowlinks_dfs(*j, i, order, g, ord, low);
			low[i] = min(low[i], low[*j]);
		}else if(*j != parent)
			low[i] = min(low[i], ord[*j]);
	}
}
void compute_lowlinks(const vector<vector<int> > &g, vector<int> &ord, vector<int> &low) {
	int n = g.size();
	ord.assign(n, -1); low.assign(n, -1);
	int order = 0;
	for(int i = 0; i < n; i ++) if(ord[i] == -1)
		compute_lowlinks_dfs(0, -1, order, g, ord, low);
}
inline bool is_bridge_edge(int i, int j, const vector<int> &ord, const vector<int> &low) {
	return ord[i] < low[j] || ord[j] < low[i];
}


void biconnected_components_dfs(int i, int col, const vector<vector<int>> &g, vector<int> &color, const vector<int> &ord, const vector<int> &low) {
	color[i] = col;
	for(auto j = g[i].begin(); j != g[i].end(); ++ j) {
		if(color[*j] == -1 && !is_bridge_edge(i, *j, ord, low))
			biconnected_components_dfs(*j, col, g, color, ord, low);
	}
}
int biconnected_components(const vector<vector<int>> &g, vector<int> &color, vector<int> &ord, vector<int> &low) {
	int n = g.size();
	compute_lowlinks(g, ord, low);
	int colors = 0;
	color.assign(n, -1);
	for(int i = 0; i < n; i ++) if(color[i] == -1)
		biconnected_components_dfs(i, colors ++, g, color, ord, low);
	return colors;
}
