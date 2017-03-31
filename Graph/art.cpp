
    int dfsctr,root,child;
    int dfs_num[N],dfs_low[N];
    void dfs_pre(int u,int p)               
    {
        dfs_num[u]=dfs_low[u]=dfsctr++; 
        for(auto it:g[u])
        {
            if(dfs_num[it]==-1)
            {
                if(u==root)++child;
                dfs_pre(it,u);
                dfs_low[u]=min(dfs_low[u],dfs_low[it]);
                if(dfs_low[it]>=dfs_num[u]) 
                is_art[u]=1;
            }
            else if(p!=it)
            {
                dfs_low[u]=min(dfs_low[u],dfs_num[it]);
            }
        }
    }
    void pre(int n){
        dfsctr=0;
        memset(dfs_num,-1,sizeof(dfs_num));
        FOR(i,0,n-1)
        if(dfs_num[i]==-1){
            root=i,child=0;
            dfs_pre(root,-1);
            is_art[root]=child>1;
        }
    }
