struct bit_trie{
	int len;
	struct node{
		int N[2];
		node(){N[0]=N[1]=0;}
	};
	vector<node> T;
	bit_trie(int len):len(len),T(2){};
	void insert(int no, ll x){
		NFOR(i,len,0){
			int b = (x>>i)&1;
			if(!T[no].N[b])
			T.push_back(node()),T[no].N[b]=T.size()-1;
			no=T[no].N[b];
		}
	}
	ll query(int no,ll x){
		ll ans=0;
		NFOR(i,len,0){
			int b = (x>>i)&1;
			if(T[no].N[!b])
				ans^=(1LL<<i),no=T[no].N[!b];
			else
				no=T[no].N[b];
		}
		return ans;
	}

};