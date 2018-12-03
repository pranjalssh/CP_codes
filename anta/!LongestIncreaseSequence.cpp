int longestIncreaseSequence(int a[], int n) {
	vector<int> A(n, INF);
	for(int i = 0; i < n; i ++)
		*lower_bound(A.begin(), A.end(), a[i]) = a[i];
	return find(A.begin(), A.end(), INF) - A.begin();
}

int longestNondecreasingSequence(int a[], int n) {
	vector<int> A(n, INF);
	for(int i = 0; i < n; i ++)
		*upper_bound(A.begin(), A.end(), a[i]) = a[i];
	return find(A.begin(), A.end(), INF) - A.begin();
}
