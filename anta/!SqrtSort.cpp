//！！区間は閉区間！！

vector<int> sqrtSort(const vector<pair<int,int> > &q, const int S) {
	int n = q.size();
	vector<long long> pack(n);
	for(int i = 0; i < n; i ++) {
		pack[i] =
			(long long)(q[i].first/S) << 50 |
			(long long)((q[i].first/S & 1) == 0 ? q[i].second : (1<<25) - 1 - q[i].second) << 25 |
			i;
	}
	sort(all(pack));
	vector<int> res(n);
	for(int i = 0; i < n; i ++)
		res[i] = pack[i] & ((1 << 25) - 1);
	return res;
}

	//インラインで書く。
	int L = 0, R = -1;
	for(int ii = 0; ii < Q; ii ++) {
		int q = ord[ii];
		int qL = queries[q].first, qR = queries[q].second;
		while(R < qR) {
			R ++;
			//区間を右に拡張する。Rが新たに追加されたインデックス
			//↓に書く
			
		}
		while(L > qL) {
			L --;
			//区間を左に拡張する。Lが新たに追加されたインデックス
			//↓に書く
			
		}
		while(R > qR) {
			//区間の右を短縮する。Rが今削除されるインデックス
			
			//↑に書く
			R --;
		}
		while(L < qL) {
			//区間の左を短縮する。Lが今削除されるインデックス
			
			//↑に書く
			L ++;
		}
		ans[q] = 
	}
