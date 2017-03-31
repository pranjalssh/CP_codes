#include <bits/stdc++.h>
using namespace std;

struct treap{
	struct node{
		int B;
		int H,SZ;
		int L,R;
		
		node(const int& val):B(val),H(rand()),SZ(1),L(0),R(0) {}
		node():B(0),H(0),SZ(0),L(0),R(0) {}
	};

	int root;
	static vector<node> T;
	static int ctr;
	treap():root(0){}
	
	inline void upd_sz(int t) {if(t) T[t].SZ = T[T[t].L].SZ + T[T[t].R].SZ + 1;} 
	
	void split(int t, int &l, int &r, const int& val){
		if(!t)	{l=r=0; return;}
		else if(T[t].B <= val) split(T[t].R,T[t].R,r,val),l=t;
		else split(T[t].L,l,T[t].L,val),r=t;
		upd_sz(t);
	}
	
	void merge(int &t, int l, int r){
		if(!l or !r) t=l?l:r;
		else if(T[l].H > T[r].H) merge(T[l].R,T[l].R,r),t=l;
		else merge(T[r].L,l,T[r].L),t=r;
		upd_sz(t);
	}
	
	inline int search(const int&val){return search(root,val);}
	int search(int t,const int& val){
		if(!t)return 0;
		if(val==T[t].B) return t;
		if(val< T[t].B) return search(T[t].L,val);
		return search(T[t].R,val);
	}

	// inline bool insert(const int& val)
	// {
	// 	if(search(val))return 0;
	// 	T.push_back(node(val));
	// 	insert(root,T.size()-1);
	// 	return 1;
	// }
	// void insert(int &t, int it){
	// 	if(!t) t=it;
	// 	else if(T[it].H > T[t].H) split(t,T[it].L,T[it].R,T[it].B),t=it;
	// 	else insert( T[t].B<=T[it].B ? T[t].R : T[t].L , it );
	// 	upd_sz(t);
	// }
	
	// inline void erase(const int&val){erase(root,val);}
	// void erase(int &t, const int& val){
	// 	if(!t) return ;
	// 	else if(T[t].B == val)  merge(t,T[t].L,T[t].R);
	// 	else erase( T[t].B<val ? T[t].R : T[t].L , val );
	// 	upd_sz(t);
	// }

	
	inline int kth(int k){return kth(root,k);}
	int kth(int t,int k)
	{
		int x = T[T[t].L].SZ;
		if(k<=x)return kth(T[t].L,k);
		if(k==x+1) return T[t].B;
		return kth(T[t].R,k-x-1);
	}

	inline int count(int x){return count(root,x);}
	int count(int t,int x)
	{
		if(!t)return 0;
		if(T[t].B>=x) return count(T[t].L,x);
		return T[T[t].L].SZ+1+count(T[t].R,x);
	}

	
		// void print(){print(root);}
		// void print(int t){
		// 	if(!t)return;
		// 	print(T[t].L);cerr<<T[t].B<<" ";print(T[t].R);
		// }
		// inline bool insert2(const int& val){
		// 	T.push_back(node(val));
		// 	if(search(val))return 0;
		// 	int l,r;
		// 	split(root,l,r,val);
		// 	int x;
		// 	merge(x,l,T.size()-1);
		// 	merge(root,x,r);
		// 	return 1;
		// }
		// inline void erase2(const int&val){
		// 	int x = search(val);
		// 	int l,r;
		// 	split(root,l,r,val);
		// 	int l1,r1;
		// 	split(l,l1,r1,val-1);
		// 	merge(root,l1,r);
		// }
	
}; vector<treap::node> treap::T={treap::node()};int treap::ctr=0;

treap t;
int main()
{
	ios_base::sync_with_stdio(0);cin.tie(0);
	srand(time(NULL));
	int q;
	cin>>q;
	
	while(q--){
		char ch;int x;
		cin>>ch>>x;
		if(ch=='I')t.insert(x);
		else if(ch=='D')t.erase(x);
		else if(ch=='C')cout<<t.count(x)<<"\n";
		else{
			if(x>t.T[t.root].SZ)cout<<"invalid\n";
			else cout<<t.kth(x)<<"\n";
		}
	}
	
	
	return 0;
}