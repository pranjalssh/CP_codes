//sum (i=1..n) i^m
//Cは二項係数。テーブルでやっても逆元でやってもよい。
//ただし、C(m+1,*)が必要！！！！(mでなく)
//O(m^2)
mint powersum(mint n, int m) {
	if(m == 0) return n;
	vector<mint> mem(m+1);
	mem[0] = n;
	for(int k = 1; k <= m; k ++){
		mint sum = ((n+1) ^ (k+1)) - 1;
		for(int i = 0; i < k; i ++)
			sum -= mem[i] * C[k+1][i];
		mem[k] = sum / mint(k+1);
	}
	return mem[m];
}
