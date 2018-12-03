//例: 区間中の区間を数える。mp(-x[i], y[i])をpointsに入れて、mp(-left+1,right)で半開区間クエリ
//半開区間！ [(0,0),(right,bottom))
//ans(i): ans[query[j].second] += sum(...)
template<typename T, typename V>
void twoDimensionalSumQuery(vector<pair<pair<T,T>,V> > points, vector<pair<pair<T,T>,int> > queries, vector<V> &ans) {
	int n = points.size(), m = queries.size();
	vector<T> positions;
	rep(i, n) positions.pb(points[i].first.second);
	sort(all(positions));
	positions.erase(unique(all(positions)), positions.end());
	int p = positions.size();
	FenwickTree<V> t(p);
	sort(all(points)); sort(all(queries));
	int left = 0;
	rep(i, m) {
		T x = queries[i].first.first;
		while(left < n && points[left].first.first < x) {
			int j = lower_bound(all(positions), points[left].first.second) - positions.begin();
			t.add(j, points[left].second);
			left ++;
		}
		int y = lower_bound(all(positions), queries[i].first.second) - positions.begin();
		ans[queries[i].second] += t.sum(y);
	}
}
