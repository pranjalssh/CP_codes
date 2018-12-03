//！浮動小数点数コスト用！。EPSは調整する必要あるかも
const double InfCost = 1e99;
struct MinimumCostMaximumFlow {
	typedef int Index; typedef int Flow; typedef double Cost;
	static const Flow InfCapacity = INF;
	struct Edge {
		Index to; Index rev;
		Flow capacity; Cost cost;
	};
	vector<vector<Edge> > g;
	void init(Index n) { g.assign(n, vector<Edge>()); }
	void add(Index i, Index j, Flow capacity = InfCapacity, Cost cost = Cost()) {
		Edge e, f; e.to = j, f.to = i; e.capacity = capacity, f.capacity = 0; e.cost = cost, f.cost = -cost;
		g[i].push_back(e); g[j].push_back(f);
		g[i].back().rev = (Index)g[j].size() - 1; g[j].back().rev = (Index)g[i].size() - 1;
	}
	void addB(Index i, Index j, Flow capacity = InfCapacity, Cost cost = Cost()) {
		add(i, j, capacity, cost);
		add(j, i, capacity, cost);
	}
	pair<Cost,Flow> minimumCostMaximumFlow(Index s, Index t, Flow f = InfCapacity, bool bellmanFord = false) {
		int n = g.size();
		vector<Cost> dist(n); vector<Index> prev(n); vector<Index> prevEdge(n);
		pair<Cost,Flow> total = make_pair(0, 0);
		vector<Cost> potential(n);
		while(f > 0) {
			fill(dist.begin(), dist.end(), InfCost);
			if(bellmanFord || total.second == 0) {
				dist[s] = 0;
				rep(k, n) {
					bool update = false;
					rep(i, n) if(dist[i] != InfCost)
					for(Index ei = 0; ei < (Index)g[i].size(); ei ++) {
						const Edge &e = g[i][ei];
						if(e.capacity <= 0) continue;
						Index j = e.to; Cost d = dist[i] + e.cost;
						if(dist[j] > d + EPS) {	//誤差…
							dist[j] = d; prev[j] = i; prevEdge[j] = ei;
							update = true;
						}
					}
					if(!update) break;
				}
			}else {
				vector<bool> vis(n);
				priority_queue<pair<Cost,Index> > q;
				q.push(make_pair(-0, s)); dist[s] = 0;
				while(!q.empty()) {
					Index i = q.top().second; q.pop();
					if(vis[i]) continue;
					vis[i] = true;
					for(Index ei = 0; ei < (Index)g[i].size(); ei ++) {
						const Edge &e = g[i][ei];
						if(e.capacity <= 0) continue;
						Index j = e.to; Cost d = dist[i] + e.cost + potential[i] - potential[j];
						if(d < dist[i])	d = dist[i];	//誤差…
						if(dist[j] > d) {
							dist[j] = d; prev[j] = i; prevEdge[j] = ei;
							q.push(make_pair(-d, j));
						}
					}
				}
			}
			if(dist[t] == InfCost) break;
			if(!bellmanFord) for(Index i = 0; i < n; i ++) potential[i] += dist[i];
			Flow d = f; Cost distt = 0;
			for(Index v = t; v != s; ) {
				Index u = prev[v]; const Edge &e = g[u][prevEdge[v]];
				d = min(d, e.capacity); distt += e.cost; v = u;
			}
			f -= d; total.first += d * distt; total.second += d;
			for(Index v = t; v != s; v = prev[v]) {
				Edge &e = g[prev[v]][prevEdge[v]];
				e.capacity -= d; g[e.to][e.rev].capacity += d;
			}
		}
		return total;
	}
};


//小さいグラフではSPFAのほうが速いかも
//！浮動小数点数の「コスト」でも使うな！誤差でバグる！
struct MinimumCostMaximumFlow {
	typedef int Index; typedef int Flow; typedef int Cost;
	static const Flow InfCapacity = INF; static const Cost InfCost = INF;
	struct Edge {
		Index to; Index rev;
		Flow capacity; Cost cost;
	};
	vector<vector<Edge> > g;
	void init(Index n) { g.assign(n, vector<Edge>()); }
	void add(Index i, Index j, Flow capacity = InfCapacity, Cost cost = Cost()) {
		Edge e, f; e.to = j, f.to = i; e.capacity = capacity, f.capacity = 0; e.cost = cost, f.cost = -cost;
		g[i].push_back(e); g[j].push_back(f);
		g[i].back().rev = (Index)g[j].size() - 1; g[j].back().rev = (Index)g[i].size() - 1;
	}
	void addB(Index i, Index j, Flow capacity = InfCapacity, Cost cost = Cost()) {
		add(i, j, capacity, cost);
		add(j, i, capacity, cost);
	}
	pair<Cost,Flow> minimumCostMaximumFlow(Index s, Index t, Flow f = InfCapacity, bool useSPFA = false) {
		int n = g.size();
		vector<Cost> dist(n); vector<Index> prev(n); vector<Index> prevEdge(n);
		pair<Cost,Flow> total = make_pair(0, 0);
		vector<Cost> potential(n);
		while(f > 0) {
			fill(dist.begin(), dist.end(), InfCost);
			if(useSPFA || total.second == 0) {
				deque<Index> q;
				q.push_back(s); dist[s] = 0; vector<bool> inqueue(n);
				while(!q.empty()) {
					Index i = q.front(); q.pop_front(); inqueue[i] = false;
					for(Index ei = 0; ei < g[i].size(); ei ++) {
						const Edge &e = g[i][ei]; Index j = e.to; Cost d = dist[i] + e.cost;
						if(e.capacity > 0 && d < dist[j]) {
							if(!inqueue[j]) {
								inqueue[j] = true;
								q.push_back(j);
							}
							dist[j] = d; prev[j] = i; prevEdge[j] = ei;
						}
					}
				}
			}else {
				vector<bool> vis(n);
				priority_queue<pair<Cost,Index> > q;
				q.push(make_pair(-0, s)); dist[s] = 0;
				while(!q.empty()) {
					Index i = q.top().second; q.pop();
					if(vis[i]) continue;
					vis[i] = true;
					for(Index ei = 0; ei < (Index)g[i].size(); ei ++) {
						const Edge &e = g[i][ei];
						if(e.capacity <= 0) continue;
						Index j = e.to; Cost d = dist[i] + e.cost + potential[i] - potential[j];
						if(dist[j] > d) {
							dist[j] = d; prev[j] = i; prevEdge[j] = ei;
							q.push(make_pair(-d, j));
						}
					}
				}
			}
			if(dist[t] == InfCost) break;
			if(!useSPFA) for(Index i = 0; i < n; i ++) potential[i] += dist[i];
			
			Flow d = f; Cost distt = 0;
			for(Index v = t; v != s; ) {
				Index u = prev[v]; const Edge &e = g[u][prevEdge[v]];
				d = min(d, e.capacity); distt += e.cost; v = u;
			}
			f -= d; total.first += d * distt; total.second += d;
			for(Index v = t; v != s; v = prev[v]) {
				Edge &e = g[prev[v]][prevEdge[v]];
				e.capacity -= d; g[e.to][e.rev].capacity += d;
			}
		}
		return total;
	}
};
