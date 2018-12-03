//グリッド上のBFS。非常に頻出なのでライブラリにする価値あるかなって
//<http://wupc2012.contest.atcoder.jp/submissions/134595>

static const int dy[4] = {0,1,0,-1}, dx[4] = {1,0,-1,0};
template<typename Grid, typename Dist>
int solveMaze(const Grid &grid, int h, int w, Dist &dist, char wallc, char sc, char gc = 0) {
	rep(i, h) rep(j, w) dist[i][j] = INF;
	int sy = -1, sx = -1;
	rep(i, h) rep(j, w) if(grid[i][j] == sc) sy = i, sx = j;
	if(sy == -1) return INF;
	vector<pii> q, next;
	next.push_back(make_pair(sy, sx));
	dist[sy][sx] = 0;
	int t = 0;
	while(!next.empty()) {
		q.swap(next);
		while(!q.empty()) {
			int i = q.back().first, j = q.back().second; q.pop_back();
			rep(d, 4) {
				int yy = i + dy[d], xx = j + dx[d];
				if(yy<0||yy>=h||xx<0||xx>=w) continue;
				if(grid[yy][xx] == wallc) continue;
				if(dist[yy][xx] != INF) continue;
				dist[yy][xx] = t + 1;
				next.push_back(make_pair(yy, xx));
			}
		}
		t ++;
	}
	int goal = INF;
	rep(i, h) rep(j, w) if(grid[i][j] == gc) goal = min(goal, dist[i][j]);
	return goal;
}
