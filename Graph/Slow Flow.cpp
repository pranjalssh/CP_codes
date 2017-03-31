struct Flow
{
	vvi G;
	vvi cap;
	vi from;
	vector<bool>vis;
	int Source,Sink;
	Flow(int n)
	{
		G.rz(n+2);
		Source=0;
		Sink=n+1;
		cap.rz(n+2);
		FOR(i,0,n+1)cap[i].rz(n+2);
		from.rz(n+2);
		vis.rz(n+2);
	}
	void Add(int u, int v, int c, bool undir=0)
	{
		G[u].pb(v);
		G[v].pb(u);
		cap[u][v]=c;
		if(undir)cap[v][u]=c;
	}
	int bfs()
	{
		fill(from,from+Sink+1,-1);
		fill(vis,vis+Sink+1,0);
		queue<int> q;
		q.push(Source);
		vis[Source]=1;
		int u;
		while(!q.empty())
		{
			u=q.front();q.pop();
			//if(u==g+h+1)break;
			tr(G[u],it)
			{
				if((!vis[*it])&&(cap[u][*it]>0))
				{from[*it]=u;q.push(*it);vis[*it]=1;}
			}
		}
		int mn=INF;u=Sink;
		while(from[u]!=-1)
		{
			mn=min(mn,cap[from[u]][u]);
			u=from[u];
		}
		u=Sink;
		while(from[u]!=-1)
		{
			cap[from[u]][u]-=mn;
			cap[u][from[u]]+=mn;
			u=from[u];
		}
		mn=(mn==INF)?0:mn;
		return mn;
	}
	int max_flow(){int ans=0;int temp;while(1){temp=bfs();if(temp==0)break;ans+=temp;}return ans;}
};