//インラインで書くべし
//INFのオーバーフローには気を留めること

int wf[N][N];
int n = size

//前処理
rep(i, n) rep(j, n) wf[i][j] = INF;
rep(i, n) wf[i][i] = 0;

//minを取ることに注意
each(e, graph) {
	wf[e->src][e->dst] = min(wf[e->src][e->dst], e->weight);
}

rep(k, n) rep(i, n) rep(j, n) {
	if(wf[i][k] != INF && wf[k][j] != INF)
		wf[i][j] = min(wf[i][j], wf[i][k] + wf[k][j]);
}

if(wf[i][j]==INF)	//到達可能性
dist = wf[i][j];	//最短距離
if(wf[i][i] < 0)	//iを通る負閉路が存在するか
rep(i, n)
	if(wf[origin][i]!=INF && wf[i][destination]!=INF && wf[i][i]<0)
		return "ENDLESS PROFIT";
	//originからdestinationへ向かう道であって、負閉路を通るものが存在するか

