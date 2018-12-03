int create_tree(int k)
{
	//return root of tree
	set<int> S;
	//get distinct nodes 
	for(int i=0;i<k;i++)S.insert(Q[i]);k=0;
	for(auto it=S.begin();it!=S.end();it++)Q[k++]=*it;
	sort(Q,Q+k,cmp);
	int kk = k;
	//distinct initial nodes, add lca of adjacent pairs
	for(int i=0;i<kk-1;i++)
	{
		int x = lca(Q[i],Q[i+1]);
		if(S.count(x)) continue;
		Q[k++]=x;
		S.insert(x);
	}
	sort(Q,Q+k,cmp);
	//get latest distance from each node in set S to root
	for(int i=0; i<k; i++) currDist[Q[i]] = query(arr[Q[i]]);
	//clear tree vector of every node in auxillary tree
	for(int i=0; i<k; i++) tree[Q[i]].clear();
	stack<int> s;
	//Q[0] is root of auxillary tree as it has min arrival time
	s.push(Q[0]);
	for(int i=1;i<k;i++)
	{
		// find parent of Q[i] in auxillary tree
		while(!anc(s.top(),Q[i])) s.pop();
		// add edge with currDist
		LL d = dist(Q[i],s.top());
		tree[s.top()].PB(MP(Q[i],d));
		tree[Q[i]].PB(MP(s.top(),d));
		s.push(Q[i]);
	}
	return Q[0];
}