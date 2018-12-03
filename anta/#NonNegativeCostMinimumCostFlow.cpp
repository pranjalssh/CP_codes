//！注意: 負のコストの辺には対応してない

typedef int Weight;
struct Edge {
  int src, dst;
  Weight capacity, cost;
  Edge(int src_, int dst_, Weight capacity_, Weight cost_) :
    src(src_), dst(dst_), capacity(capacity_), cost(cost_) { }
};
bool operator < (const Edge &e, const Edge &f) {
  return e.cost != f.cost ? e.cost > f.cost : // !!INVERSE!!
    e.src != f.src ? e.src < f.src : e.dst < f.dst;
}
typedef vector<Edge> Edges;
typedef vector<Edges> Graph;

typedef vector<Weight> Array;
typedef vector<Array> Matrix;


#define RESIDUE(u,v) (capacity[u][v] - flow[u][v])
#define RCOST(u,v) (cost[u][v] + h[u] - h[v])
pair<Weight, Weight> minimumCostFlow(const Graph &g, int s, int t) {
  const int n = g.size();
  Matrix capacity(n, Array(n)), cost(n, Array(n)), flow(n, Array(n));
  rep(u,n) each(e,g[u]) {
    capacity[e->src][e->dst] += e->capacity;
    cost[e->src][e->dst] += e->cost;
  }
  pair<Weight, Weight> total; // (cost, flow)
  vector<Weight> h(n);

  for (Weight F = INF; F > 0; ) { // residual flow
    vector<Weight> d(n, INF); d[s] = 0;
    vector<int> p(n, -1);
    priority_queue<Edge> Q; // "e < f" <=> "e.cost > f.cost"
    for (Q.push(Edge(-2, s, 0, 0)); !Q.empty(); ) {
      Edge e = Q.top(); Q.pop();
      if (p[e.dst] != -1) continue;
      p[e.dst] = e.src;
      each(f, g[e.dst]) if (RESIDUE(f->src, f->dst) > 0) {
        if (d[f->dst] > d[f->src] + RCOST(f->src, f->dst)) {
          d[f->dst] = d[f->src] + RCOST(f->src, f->dst);
          Q.push( Edge(f->src, f->dst, 0, d[f->dst]) );
        }
      }
    }
    if (p[t] == -1) break;

    Weight f = F;
    for (int u = t; u != s; u = p[u])
      f = min(f, RESIDUE(p[u], u));
    for (int u = t; u != s; u = p[u]) {
      total.first += f * cost[p[u]][u];
      flow[p[u]][u] += f; flow[u][p[u]] -= f;
    }
    F -= f;
    total.second += f;
    rep(u, n) h[u] += d[u];
  }
  return total;
}