//木の親→子の辺だけ残す
void direct_tree(const vector<vi> &g, int i, int parent, vector<pii> &out_edges) {
	each(j, g[i]) if(*j != parent) {
		out_edges.pb(mp(i, *j));
		direct_tree(g, *j, i, out_edges);
	}
}
